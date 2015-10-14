/*
 * ShowThreadController.cpp
 *
 *  Created on: 20 mars 2014
 *      Author: PierreL
 */

#include "ShowThreadController.hpp"


#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>
#include <QDateTime>
#include <QMap>

#include <bb/cascades/WebStorage>
#include <bb/cascades/ListView>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>
#include <bb/system/SystemToast>
#include <bb/platform/Notification>
#include <bb/system/Clipboard>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>
#include <bb/system/SystemDialog>

#include  "Globals.h"
#include  "Network/HFRNetworkAccessManager.hpp"
#include  "Network/WebResourceManager.h"
#include  "DataObjects.h"
#include  "Settings.hpp"
#include  "Easter.hpp"
#include  "DialogSearch.hpp"

ShowThreadController::ShowThreadController(QObject *parent)
	: QObject(parent), m_WebView(NULL), m_ListView(NULL), m_Datas(new QList<PostDetailItem*>), m_AddSignature(false), m_ScrollAtLocation(false), m_NbWebviewLoaded(0), m_ActionSurvey(false), m_DialogSearch(NULL) {
}



bool ShowThreadController::isOwnMessage(const QString &name) {
    return name == m_Pseudo;
}


void ShowThreadController::showThread(const QString &url) {

	// -----------------------------------------------------------------------------------------
	// request page
	m_Url = url;
	m_ScrollAtLocation = false;
	m_NbWebviewLoaded = 0;

	QNetworkRequest request(DefineConsts::FORUM_URL+url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

	loadBlackList();

}


void ShowThreadController::loadBlackList() {
    m_BlackList.clear();

    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
    QFile file(directory + "/blacklist.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);

        QString line;
        line = stream.readLine();
        while(!line.isEmpty()) {
            m_BlackList.insert(line.toLower());

            line = stream.readLine();
        }
        file.close();
    }
}


void ShowThreadController::intraSearch(const QString& keywords, const QString& author, bool filter) {
    if(keywords.isEmpty() && author.isEmpty()) {
        bb::system::SystemToast *toast = new bb::system::SystemToast(this);

        toast->setBody(tr("Nothing to search"));
        toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
        toast->show();

        return;
    }

    QString currentPageNumber;
    {   // get information about current page number & last page number

        QRegExp pageNumber("page=([0-9]+)");
        pageNumber.setCaseSensitivity(Qt::CaseSensitive);

        QRegExp pageNumberCleanUrl("([0-9]+).htm");
        pageNumberCleanUrl.setCaseSensitivity(Qt::CaseSensitive);

        if(pageNumber.indexIn(m_Url, 0) != -1) {
            currentPageNumber = pageNumber.cap(1);
        } else {
            if(pageNumberCleanUrl.indexIn(m_Url, 0) != -1) {
                currentPageNumber = pageNumberCleanUrl.cap(1);
            } else {
                currentPageNumber = "1";
            }
        }

    }

    const QUrl url(DefineConsts::FORUM_URL + "/transsearch.php?config=hfr.inc");


    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


    QUrl params;
    params.addQueryItem("hash_check", m_HashCheck);
    params.addQueryItem("p", "1");
    params.addQueryItem("post", m_PostID);
    params.addQueryItem("cat", m_CatID);
    params.addQueryItem("firstnum", m_FirstNum);
    params.addQueryItem("currentnum", "");
    params.addQueryItem("word", keywords);
    params.addQueryItem("spseudo", author);
    if(filter)
        params.addQueryItem("filter", "1");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplySearch()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    emit searchStarted();

}

void ShowThreadController::checkReplySearch() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {

            QList<QByteArray> headerList = reply->rawHeaderList();
            bool found = false;
            foreach(QByteArray head, headerList) {
                if(head == "Location") {
                    found = true;
                    showThread(reply->rawHeader(head));
                }
            }

            if(!found) {
                bb::system::SystemToast *toast = new bb::system::SystemToast(this);

                toast->setBody(tr("Sorry, no posts could be found."));
                toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
                toast->show();

                emit complete();
            }
        }

        reply->deleteLater();
    }
}


void ShowThreadController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);


                QRegExp searchFailed("D.sol. aucune r.ponse n.a .t. trouv.e\t<br /><br />");
                if(searchFailed.indexIn(response) == -1) {
                    parse(response);
                } else {
                    bb::system::SystemToast *toast = new bb::system::SystemToast(this);

                    toast->setBody(tr("Sorry, no posts could be found."));
                    toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
                    toast->show();

                    emit complete();
                }

			} else {

			    QList<QByteArray> headerList = reply->rawHeaderList();
                bool found = false;
                foreach(QByteArray head, headerList) {
                    if(head == "Location") {
                        found = true;
                        showThread(reply->rawHeader(head));
                    }
                }

                if(!found) {
                    bb::system::SystemToast *toast = new bb::system::SystemToast(this);

                    toast->setBody(tr("Nothing more to display"));
                    toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
                    toast->show();
                }

                emit complete();
			}
		} else {
			connectionTimedOut();
		}

		reply->deleteLater();
	} else {
	    qDebug() << reply->errorString();
	}
}

void ShowThreadController::connectionTimedOut() {

	bb::system::SystemToast *toast = new bb::system::SystemToast(this);

	toast->setBody(tr("Connection timed out"));
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();

	emit complete();
}

void ShowThreadController::invokeBrowser(const QString& url) {
    QSettings settings("Amonchakai", "HFR10");

    if(settings.value("ConfirmLeavingApp", true).toBool()) {
        using namespace bb::cascades;
        using namespace bb::system;

        SystemDialog *dialog = new SystemDialog(tr("Yes"), tr("No"));

        dialog->setTitle(tr("Friendly warning"));
        dialog->setBody(tr("You are going to leave the application, do you want to continue?"));

        bool success = connect(dialog,
             SIGNAL(finished(bb::system::SystemUiResult::Type)),
             this,
             SLOT(onPromptLeavingApp(bb::system::SystemUiResult::Type)));

        if (success) {
            m_TentativeNewBrowserUrl = url;
            dialog->show();
        } else {
            dialog->deleteLater();
        }
    } else {
        openBrowser(url);
    }
}

void ShowThreadController::onPromptLeavingApp(bb::system::SystemUiResult::Type result) {
    using namespace bb::cascades;
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        openBrowser(m_TentativeNewBrowserUrl);
    }

    sender()->deleteLater();

}


void ShowThreadController::openBrowser(const QString& url) {
    if(m_InvokeManager == NULL) {
        m_InvokeManager = new bb::system::InvokeManager(this);
    }

    bb::system::InvokeRequest request;
    request.setAction("bb.action.OPEN");
    request.setUri(url);

    m_InvokeManager->invoke(request);
}

void ShowThreadController::parse(const QString &page) {

	// ----------------------------------------------------------------------------------------------
	// Navigation within the topic


	QRegExp amp("&amp;");
	QRegExp pageURL("<a href=\"([^\"]+)\" class=\"cHeader\">1</a>");
	pageURL.setCaseSensitivity(Qt::CaseSensitive);
	pageURL.setMinimal(true);

	if(pageURL.indexIn(page, 0) != -1) {
		m_UrlFirstPage = pageURL.cap(1);
		m_UrlFirstPage.replace(amp, "&");
	} else
		m_UrlFirstPage = "";


	pageURL = QRegExp("<a href=\"([^\"]+)\" class=\"cHeader\">([0-9]+)</a></div><div class=\"pagepresuiv\">");
	pageURL.setCaseSensitivity(Qt::CaseSensitive);
	pageURL.setMinimal(true);

	QString lastPageNumber;
	if(pageURL.indexIn(page, 0) != -1) {
		m_UrlLastPage = pageURL.cap(1);
		m_UrlLastPage.replace(amp, "&");

		lastPageNumber = pageURL.cap(2);
	} else
		m_UrlLastPage = "";

	pageURL = QRegExp("<div class=\"pagepresuiv\"><a href=\"([^\"]+)\" class=\"cHeader\" accesskey=\"w\">Page Pr");
	pageURL.setCaseSensitivity(Qt::CaseSensitive);
	pageURL.setMinimal(true);

	if(pageURL.indexIn(page, 0) != -1) {
		m_UrlPrevPage = pageURL.cap(1);
		m_UrlPrevPage.replace(amp, "&");
	} else
		m_UrlPrevPage = "";

	pageURL = QRegExp("<div class=\"pagepresuiv\"><a href=\"([^\"]+)\" class=\"cHeader\" accesskey=\"x\">Page Suivante</a></div>");
	pageURL.setCaseSensitivity(Qt::CaseSensitive);
	pageURL.setMinimal(true);

	if(pageURL.indexIn(page, 0) != -1) {
		m_UrlNextPage = pageURL.cap(1);
		m_UrlNextPage.replace(amp, "&");
	} else
		m_UrlNextPage = "";


	// ----------------------------------------------------------------------------------------------
	QString currentPageNumber;
	{	// get information about current page number & last page number

		QRegExp pageNumber("page=([0-9]+)");
		pageNumber.setCaseSensitivity(Qt::CaseSensitive);

		QRegExp pageNumberCleanUrl("([0-9]+).htm");
		pageNumberCleanUrl.setCaseSensitivity(Qt::CaseSensitive);

		if(pageNumber.indexIn(m_Url, 0) != -1) {
			currentPageNumber = pageNumber.cap(1);
		} else {
			if(pageNumberCleanUrl.indexIn(m_Url, 0) != -1) {
				currentPageNumber = pageNumberCleanUrl.cap(1);
			} else {
				currentPageNumber = "1";
			}
		}

		if(lastPageNumber == "")
			lastPageNumber = currentPageNumber;

	}

    // ----------------------------------------------------------------------------------------------
    // Parse new MP, and notify if needed!
    QRegExp newMP("Vous avez ([0-9]+) nouveau[x]* message[s]*");
    newMP.setMinimal(true);
    if(newMP.indexIn(page) != -1) {
        if(Settings::getMPNotificationUp) {
            Settings::setMPNotificationUp(true);

            bb::platform::Notification *notif = new bb::platform::Notification();
            notif->notify();

            Settings s;
            s.saveSettings();
        }
    }

    // ----------------------------------------------------------------------------------------------
    // Parse info for intra search!
    QRegExp firstNumRegExp("<input type=\"hidden\" value=\"([0-9]+)\" name=\"firstnum\" />");
    if(firstNumRegExp.indexIn(page) != -1)
        m_FirstNum = firstNumRegExp.cap(1);


	// ----------------------------------------------------------------------------------------------
	// Parse posts

	QRegExp regexp = QRegExp(QString("<td class=\"(messCase1|messCase1 messageModo )\" width=\"180\" valign=\"top\" rowspan=\"1\"><a name=\"t([0-9]+)\">")  	// post index
								   + ".*</a></div><div><b class=\"s2\">(.+)</b></div>");											// pseudo

	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


	int pos = 0;
	int lastPos = regexp.indexIn(page, pos);

	QString lastPseudo;
	QString lastPostIndex;
	if(lastPos != -1) {
		lastPostIndex = regexp.cap(2);
		lastPseudo = regexp.cap(3);

		parseSurvey(page.mid(0, lastPos));
	}


	m_Datas->clear();				// cleanup data before loading new data

	// Cut the entire page into posts
	while((pos = regexp.indexIn(page, lastPos)) != -1) {
		pos += regexp.matchedLength();

		// parse each post individually
		if(m_BlackList.find(lastPseudo.toLower()) == m_BlackList.end()) {
		    parsePost(lastPostIndex, lastPseudo, page.mid(lastPos, pos-lastPos));
		}


		lastPos = pos;
		lastPostIndex = regexp.cap(2);
		lastPseudo = regexp.cap(3);

	}

	parsePost(lastPostIndex, lastPseudo, page.mid(lastPos, pos-lastPos));
	parseDataForReply(page.mid(lastPos, pos-lastPos));

	if(!m_Datas->isEmpty()) {
	    PostDetailItem *item = m_Datas->last();

        if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
            item->getPost() += "<br /><br /><p style=\"background-color:#262626; text-align:center; \">" + currentPageNumber + "/" + lastPageNumber + "</p>";
        } else {
            item->getPost() += "<br /><br /><p style=\"background-color:#f5f5f5; text-align:center; \">" + currentPageNumber + "/" + lastPageNumber + "</p>";
        }
	}

	updateView();

	emit complete();
}


void ShowThreadController::parsePost(const QString &postIdex, const QString &author, const QString &post) {

	QRegExp editURLRegexp("title=\"Editer le message\" alt=\"edit\" /></span><a rel=\"nofollow\" href=\"(.+#formulaire)\" onclick=");
	editURLRegexp.setCaseSensitivity(Qt::CaseSensitive);
	editURLRegexp.setMinimal(true);

	QRegExp moodRegexp = QRegExp("<span class=\"MoodStatus\">(.+)</span>");
	moodRegexp.setCaseSensitivity(Qt::CaseSensitive);
	moodRegexp.setMinimal(true);

	QRegExp avatarRegexp = QRegExp("<div class=\"avatar_center\" style=\"clear:both\"><img src=\"(.*|)\" alt=\"\" /></div>");
	avatarRegexp.setCaseSensitivity(Qt::CaseSensitive);
	avatarRegexp.setMinimal(true);

	QRegExp profilRegexp = QRegExp("<a href=\"(/hfr/profil-[0-9]+.htm)\" target=\"_blank\">");
	profilRegexp.setCaseSensitivity(Qt::CaseSensitive);
	profilRegexp.setMinimal(true);

	QRegExp urlNonFiltered("<a href=\"([^\"]+)\" class=\"cLink\"><b>Voir ce message dans le sujet non filtr.</b>");
	urlNonFiltered.setCaseSensitivity(Qt::CaseSensitive);
	urlNonFiltered.setMinimal(true);


	QRegExp timestampRegexp = QRegExp(QString( "<div class=\"left\">(.+)<a href="));
	timestampRegexp.setCaseSensitivity(Qt::CaseSensitive);
	timestampRegexp.setMinimal(true);

	QRegExp postContentRegexp = QRegExp(QString("<div id=\"para[0-9]+\"><p>(.*)<div style=\"clear: both;\">"));
	postContentRegexp.setCaseSensitivity(Qt::CaseSensitive);
	postContentRegexp.setMinimal(true);

	QRegExp quoteUrl("<div class=\"edited\"><a href=\"(.*)\" class=\"cLink\" rel=\"nofollow\">Message.*([0-9]+) fois");
	quoteUrl.setCaseSensitivity(Qt::CaseSensitive);
	quoteUrl.setMinimal(true);


	QString editURL = "";
	if(editURLRegexp.indexIn(post, 0) != -1) {
		editURL = editURLRegexp.cap(1);
		editURL.replace(QRegExp("&amp;"), "&");
	}

	QString profileUrl = "";
    if(profilRegexp.indexIn(post, 0) != -1) {
        profileUrl = profilRegexp.cap(1);
        profileUrl.replace(QRegExp("&amp;"), "&");
    }

    //qDebug() << post;
    QString topicNonFilteredUrl = "";
    if(urlNonFiltered.indexIn(post, 0) != -1) {
        topicNonFilteredUrl = urlNonFiltered.cap(1);
        topicNonFilteredUrl.replace(QRegExp("&amp;"), "&");
    }


	QString mood = "";
	if(moodRegexp.indexIn(post, 0) != -1)
		mood = moodRegexp.cap(1);

	QString avatar = "";
	if(avatarRegexp.indexIn(post, 0) != -1)
		avatar = avatarRegexp.cap(1);

	QString postContent;
	QString timestamp;

	if(timestampRegexp.indexIn(post, 0) != -1) {
		timestamp = timestampRegexp.cap(1);
		timestamp = timestamp.mid(9,31);
		timestamp.replace(QRegExp("&nbsp;"), " ");

	}

	if(postContentRegexp.indexIn(post, 0) != -1) {
		postContent = postContentRegexp.cap(1);
	}

	if(postContent.isEmpty())
		return;

	m_Datas->push_back(new PostDetailItem());
	m_Datas->last()->setEditUrl(editURL);

	if(avatar.isEmpty())
		m_Datas->last()->setAvatar("images/default_avatar.png");
	else {
		m_Datas->last()->setAvatar((avatar));
	}

	// add information about number of quotes
	if(quoteUrl.indexIn(post, 0) != -1) {
		QString s = quoteUrl.cap(1);
		s.replace(QRegExp("&amp;"), "&");

		if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
			postContent += "<div style=\"background-image: url('local:///assets/images/icon_quoted_white.png'); width:48px; height:44px; float:right; \" ><p onclick=\"sendURL(\'REDIRECT:" + s + "\')\" style=\"text-align:center; color:black; margin-top:0px; font-size:25px; \" >" + quoteUrl.cap(2) +"</p></div>";
		} else {
			postContent += "<div style=\"background-image: url('local:///assets/images/icon_quoted.png'); width:48px; height:44px; float:right; \" ><p onclick=\"sendURL(\'REDIRECT:" + s + "\')\" style=\"text-align:center; color:white; margin-top:0px; font-size:25px; \" >" + quoteUrl.cap(2) +"</p></div>";
		}

	}

	// parse the post so it can be rendered in HTML within a listitem
	cleanupPost(postContent, postIdex.toInt());

	m_Datas->last()->setAuthor(author);
	m_Datas->last()->setTimestamp(timestamp);
	m_Datas->last()->setPost(postContent);
	m_Datas->last()->setIndex(postIdex.toInt());
	m_Datas->last()->setProfileUrl(profileUrl);
	m_Datas->last()->setNotFilteredUrl(topicNonFilteredUrl);
}

void ShowThreadController::parseDataForReply(const QString &page) {
	QRegExp postData(QString("<input type=\"hidden\" name=\"hash_check\" value=\"([0-9a-z]+)\" /><div class=\"s2Ext\" id=\"md_fast_search\">")
						+ 	".*<input type=\"hidden\" name=\"post\" value=\"([0-9]+)\" />"
						+ 	".*<input type=\"hidden\" name=\"cat\" value=\"([0-9a-z]+)\" />"
						+	".*<input type=\"hidden\" name=\"page\" value=\"([0-9]+)\" />"
						+ 	".*<input type=\"hidden\" name=\"pseudo\" value=\"(.+)\" />"
						+	".*<input type=\"hidden\" name=\"sujet\" value=\"(.+)\" />"
						+	".*<input type=\"hidden\" value=\"([0-1])\" name=\"signature\" />"
			);

	postData.setCaseSensitivity(Qt::CaseSensitive);
	postData.setMinimal(true);

	if(postData.indexIn(page, 0) != -1) {
		m_HashCheck = postData.cap(1);
		m_PostID = postData.cap(2);
		m_CatID = postData.cap(3);
		m_Page = postData.cap(4);
		m_Pseudo = postData.cap(5);
		m_ThreadTitle = postData.cap(6);
		m_AddSignature = postData.cap(7).length() >  0 && postData.cap(7).at(0) == '1';
	}
}

void ShowThreadController::parseSurvey(const QString &page) {

	QRegExp question("<div class=\"sondage\"><b class=\"s2\">(.*)</b><br />");
	question.setCaseSensitivity(Qt::CaseSensitive);
	question.setMinimal(true);

	int pos = 0;
	if((pos = question.indexIn(page, 0)) != -1) {
	    m_ActionSurvey = false;
		pos += question.matchedLength();

		QRegExp answers("<div class=\"barre\" style=\"width:[0-9.]+%\">&nbsp;</div><div style=\"position:absolute;left:0px\" class=\"sondageTop\">[0-9.]+&nbsp;%</div><div class=\"sondageTop\">[&nbsp;]+([0-9]+) vote[s]*</div></div><div class=\"sondageRight\">(.+)</div>");
		answers.setCaseSensitivity(Qt::CaseSensitive);
		answers.setMinimal(true);


		m_Survey = QString("<div id=\"survey\" class=\"survey\"><p style=\"text-decoration:underline; font-weight:bold;\">")  + question.cap(1) + "</p>";

		QList<QString> options;
		QList<int> nbVotes;

		int overallVoteNumber = 0;
		while((pos = answers.indexIn(page, pos)) != -1) {
			options.push_back(answers.cap(2));
			nbVotes.push_back(answers.cap(1).toInt());

			overallVoteNumber += nbVotes.last();
			pos += answers.matchedLength();
		}

		if(overallVoteNumber == 0)  // avoid dividing by 0
		    overallVoteNumber = 1;

		for(int i = 0 ; i < options.length() ; ++i) {
			m_Survey += "<div id=\"parentSurvey\"><div class=\"rightSurvey\" style=\"width: " + QString::number(static_cast<int>(nbVotes[i]*600/overallVoteNumber)) + "px;\">" + QString::number(nbVotes[i]) + "</div><div class=\"leftSurvey\">" + options[i] + "</div></div>";
		}

		m_Survey += "</div>";

	} else {
		pos = 0;
		m_ActionSurvey = true;
		QRegExp newQuestion("<input type=\"hidden\" name=\"numeropost\" value=\"([0-9]+)\" /><input type=\"hidden\" name=\"hash_check\" value=\"[a-zA-Z0-9]+\" /><b class=\"s2\">(.*)</b>");
		newQuestion.setCaseSensitivity(Qt::CaseSensitive);
		newQuestion.setMinimal(true);

		QString colorHandling = "} ";
		if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
			colorHandling = "background-color:#000000; color:#FFFFFF; } ";
		}

		if((pos = newQuestion.indexIn(page, pos)) != -1) {
			m_Survey = QString("<div id=\"survey\" class=\"survey\"><p style=\"text-decoration:underline; font-weight:bold;\">")
										 + newQuestion.cap(2) + "</p>";

			//<input class="checkbox" type="checkbox" value="1" id="sond1" name="reponse1" /><label for="sond1">option 1</label>
			QRegExp answers("type=\"([radiochekbx]+)\" value=\"[0-9]+\" id=\"sond[0-9]+\" name=\"reponse[0-9]*\" /><label for=\"sond[0-9]+\">(.+)</label>");
			answers.setCaseSensitivity(Qt::CaseSensitive);
			answers.setMinimal(true);

			int respIDX = 1;
			bool dataType = false;
			QString listSelectedItemsFunctor("function getSelectedItems() { var ret=0; ");
			while((pos = answers.indexIn(page, pos)) != -1) {
				dataType = answers.cap(1).at(0) == 'r';
				//qDebug() << answers.cap(1) <<  answers.cap(2);
				m_Survey += "<input type=\"" + answers.cap(1) + "\" value=\"" + QString::number(respIDX) + "\" id=\"sond" + QString::number(respIDX) + "\" name=\"reponse\" class=\"custom\" /><label for=\"sond" + QString::number(respIDX) + "\" ><span></span>" + answers.cap(2) + "</label><br/><br/>" ;

				listSelectedItemsFunctor += "if(document.getElementById(\"sond" + QString::number(respIDX) + "\").checked) { ret += Math.pow(2," + QString::number(respIDX-1) +"); } ";
				pos += answers.matchedLength();
				++respIDX;
			}
			listSelectedItemsFunctor += " return ret; }";
			m_Survey += QString("<br /><input type=\"submit\" onclick=\"navigator.cascades.postMessage(\'SURVEY:") + (dataType ? "1" : "0") + "\' + getSelectedItems().toString())\" name=\"sondage_submit\" value=\"" + tr("Vote") + "\" /><script>"+ listSelectedItemsFunctor +"</script>";

			m_Survey += "</div>";


		} else {

			m_Survey = QString("");
		}
	}

	if(!m_Survey.isEmpty()) {
	    QFile htmlTemplateFile(QDir::currentPath() + "/app/native/assets/template.html");
	    if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
	        htmlTemplateFile.setFileName(QDir::currentPath() + "/app/native/assets/template_black.html");
	    }
	    QFile htmlEndTemplateFile(QDir::currentPath() + "/app/native/assets/template_end.html");
	    QString htmlTemplate;
	    if (htmlTemplateFile.open(QIODevice::ReadOnly) && htmlEndTemplateFile.open(QIODevice::ReadOnly)) {
	        htmlTemplate = htmlTemplateFile.readAll();


	        if(Settings::fontSize() != 25) {
	            htmlTemplate.replace("p { font-size: 25px;", "p { font-size: " + QString::number(Settings::fontSize()) + "px;");
	        }
	    }

	    m_Survey = htmlTemplate + m_Survey + htmlEndTemplateFile.readAll();
	}

	emit surveyUpdated();
}

void ShowThreadController::addToFavorite(int responseID) {

	const QUrl url(DefineConsts::FORUM_URL + "/user/addflag.php?config=hfr.inc&cat=" + m_CatID + "&post=" + m_PostID + "&numreponse=" + QString::number(responseID));


	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkSuccessAddAddFavorite()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

}

void ShowThreadController::checkSuccessAddAddFavorite() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();

			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				qDebug() << response;
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
		}

		reply->deleteLater();
	}
}

void ShowThreadController::deletePost(int messageID) {
	const QUrl url(DefineConsts::FORUM_URL + "/bdd.php?config=hfr.inc");


	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");




	QUrl params;
	params.addQueryItem("hash_check", m_HashCheck);
	params.addQueryItem("post", m_PostID);
	params.addQueryItem("numreponse", QString::number(messageID));
	params.addQueryItem("cat", m_CatID);
	params.addQueryItem("pseudo", m_Pseudo);
	params.addQueryItem("delete", "1");

	QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkSuccessDeletePost()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}

QString ShowThreadController::getEditUrl(int messageID) const {
    for(int i = 0 ; i < m_Datas->length() ; ++i) {
        if(m_Datas->at(i)->getIndex() == messageID)
            return m_Datas->at(i)->getEditUrl();
    }

    return "";
}

QString ShowThreadController::getProfileUrl(int messageID) const {
    for(int i = 0 ; i < m_Datas->length() ; ++i) {
        if(m_Datas->at(i)->getIndex() == messageID)
            return m_Datas->at(i)->getProfileUrl();
    }

    return "";
}

QString ShowThreadController::getMessageAuthor(int messageID) const {
    for(int i = 0 ; i < m_Datas->length() ; ++i) {
        if(m_Datas->at(i)->getIndex() == messageID)
            return m_Datas->at(i)->getAuthor();
    }

    return "";
}

void ShowThreadController::checkSuccessDeletePost() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				showThread(m_Url);
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}

void ShowThreadController::addBookmark(int messageID) {

    const PostDetailItem *post;
    for(int i = 0 ; i < m_Datas->length() ; ++i) {
        if(m_Datas->at(i)->getIndex() == messageID)
            post = m_Datas->at(i);
        }

    QMap<int, QString> cvtCatName;
    cvtCatName[1] = "Hardware";
    cvtCatName[16] = "Hardware - Peripheriques";
    cvtCatName[15] = "Ordinateurs portables";
    cvtCatName[23] = "Technologies Mobiles";
    cvtCatName[2] = "Overclocking, Cooling & Modding";
    cvtCatName[25] = "Apple";
    cvtCatName[3] = "Video & Son";
    cvtCatName[14] = "Photo numerique";
    cvtCatName[5] = "Jeux Video";
    cvtCatName[4] = "Windows & Software";
    cvtCatName[22] = "Reseaux grand public / SoHo";
    cvtCatName[21] = "Systemes & Reseaux Pro";
    cvtCatName[11] = "Linux et OS Alternatifs";
    cvtCatName[10] = "Programmation";
    cvtCatName[12] = "Graphisme";
    cvtCatName[6] = "Achats & Ventes";
    cvtCatName[8] = "Emploi & Etudes";
    cvtCatName[9] = "Seti et projets distribues";
    cvtCatName[13] = "Discussions";

    ThreadListItem item;
    item.setTitle(m_ThreadTitle);
    item.setCategory(cvtCatName[m_CatID.toInt()]);
    item.setTimestamp(post->getTimestamp());
    item.setLastAuthor(post->getAuthor());
    item.setPages(m_Page);
    item.setUrlFirstPage(m_UrlFirstPage);

    if(m_UrlLastPage.isEmpty())
        item.setUrlLastPage(m_Url);
    else
        item.setUrlLastPage(m_UrlLastPage);

    QString urlLastPostRead = m_Url;
    QRegExp redirect("#[t0-9bas]+");
    if(redirect.indexIn(urlLastPostRead) != -1) {
        urlLastPostRead.replace(redirect, "#t"+QString::number(messageID));
    } else {
        urlLastPostRead += "#t"+QString::number(messageID);
    }
    item.setUrlLastPostRead(urlLastPostRead);


    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
    if (!QFile::exists(directory)) {
        return;
    }

    QFile file(directory + "/Bookmarks.txt");

    if (file.open(QIODevice::Append)) {
        QDataStream stream(&file);

        stream << item;

        file.close();
    }
}


void ShowThreadController::cleanupPost(QString &post, int messageID) {
	// ----------------------------------------------------
	// declare regexp used to clean the posts

	QString cleanPost;
	QRegExp quoteRegexp(QString( "<div class=\"container\"><table class=\"citation\"><tr class=\"none\"><td><b class=\"s1\"><a href=\"(.*[0-9]+)\" class=\"Topic\">")
								+"(.+)"														// author
								+"</a></b><br /><br />[&nbsp;]*(.*)(</p><div class=\"container\"><table class=\"citation\">|<div class=\"container\"><table class=\"citation\">|</td></tr></table></div>)"	// message
			);
	quoteRegexp.setCaseSensitivity(Qt::CaseSensitive);
	quoteRegexp.setMinimal(true);

	QRegExp spoilerRegExp("<div class=\"container\"><table class=\"spoiler\".*class=\"s1Topic\">Spoiler :</b><br /><br /><div class=\"Topic masque\">[&nbsp;]*<p>(.*)</p>[&nbsp;]*</div></td></tr></table></div>");
	spoilerRegExp.setCaseSensitivity(Qt::CaseSensitive);
	spoilerRegExp.setMinimal(true);

	QRegExp simpleQuoteRegexp("<div class=\"container\"><table class=\"quote\"><tr class=\"none\"><td><b class=\"s1\">Citation :</b><br /><br /><p>(.*)</p>(<div class=\"container\"><table class=\"quote\"><tr class=\"none\"><td><b class=\"s1\">Citation :</b>|</td></tr></table></div>)"); //<div class=\"container\"><table class=\"quote\">|
	simpleQuoteRegexp.setCaseSensitivity(Qt::CaseSensitive);
	simpleQuoteRegexp.setMinimal(true);

	QRegExp codeRegexp("<table class=\"code\"><tr class=\"none\"><td><b class=\"s1\" style=\"font-family: Verdana, Helvetica, Arial, Sans-serif;\">Code :</b><br />(.*)</td></tr></table>");
	codeRegexp.setCaseSensitivity(Qt::CaseSensitive);
	codeRegexp.setMinimal(true);


	// ----------------------------------------------------
	// handle spoilers

	cleanPost = "";
	int lastPos = 0;
	int pos = 0;
	while((pos = spoilerRegExp.indexIn(post, pos)) != -1) {
		cleanPost += "<p>" + post.mid(lastPos, pos-lastPos) + "</p>";

		cleanPost += "<div class=\"c1\" style=\"border:1px solid; border-color:gray;\" onclick=\"javascript:montrer_spoiler(\'spoiler_" + QString::number(messageID) + "_" + QString::number(pos) + "\')\"><u><strong>Spoiler :</strong></u><script type=\"text/javascript\"> function montrer_spoiler(value){var actual=document.getElementById(value).style.visibility;if (actual==\'visible\'){document.getElementById(value).style.visibility=\'hidden\';}else{document.getElementById(value).style.visibility=\'visible\';}} </script><dl style=\"visibility: hidden;\" id=\"spoiler_" + QString::number(messageID) + "_" + QString::number(pos) + "\"><dd>" + spoilerRegExp.cap(1) + "</dd></dl></div>";
		pos += spoilerRegExp.matchedLength();
		lastPos = pos;
	}
	cleanPost += "<p>" + post.mid(lastPos, post.length()-lastPos) + "</p>";
	post = cleanPost;


	// ----------------------------------------------------
	// handle [quote][/quote]

	cleanPost = "";
	lastPos = 0;
	pos = 0;
	while((pos = codeRegexp.indexIn(post, pos)) != -1) {
		cleanPost += "<p>" + post.mid(lastPos, pos-lastPos) + "</p>";

		cleanPost += "<div class=\"quote\"><div class=\"header\" >Code :</div>" + codeRegexp.cap(1) + "</div>";
		pos += codeRegexp.matchedLength();
		lastPos = pos;
	}
	cleanPost += "<p>" + post.mid(lastPos, post.length()-lastPos) + "</p>";
	post = cleanPost;


	// ----------------------------------------------------
	// handle [cpp][/cpp]

    cleanPost = "";
    lastPos = 0;
    pos = 0;
    while((pos = simpleQuoteRegexp.indexIn(post, pos)) != -1) {
        cleanPost += "<p>" + post.mid(lastPos, pos-lastPos) + "</p>";



        if(simpleQuoteRegexp.cap(2) == "<div class=\"container\"><table class=\"quote\"><tr class=\"none\"><td><b class=\"s1\">Citation :</b>" ) {
            cleanPost += "<div class=\"quote\"><div class=\"header\" >Citation :</div>" + simpleQuoteRegexp.cap(1) + "</div>";
            pos += simpleQuoteRegexp.matchedLength()-105;
            // recursive quote...

            QRegExp nextClose("</td></tr></table></div>");
            nextClose.setCaseSensitivity(Qt::CaseSensitive);
            int indexNextClose = nextClose.indexIn(post, pos);
            int lastMatchingPos = 0;
            while((pos = simpleQuoteRegexp.indexIn(post, pos)) != -1 && indexNextClose != -1 ) {
                if(pos > indexNextClose)
                    break;

                cleanPost += "<div class=\"quote\"><div class=\"header\" >Citation :</div>" + simpleQuoteRegexp.cap(1);
                pos += simpleQuoteRegexp.matchedLength();
                lastMatchingPos = pos;
                indexNextClose = nextClose.indexIn(post, pos);
            }
            cleanPost += post.mid(lastMatchingPos, indexNextClose-lastMatchingPos) + "</div>";

            pos = indexNextClose + nextClose.matchedLength();

        } else {
            cleanPost += "<div class=\"quote\"><div class=\"header\" >Citation :</div>" + simpleQuoteRegexp.cap(1) + "</div>";
            pos += simpleQuoteRegexp.matchedLength();
        }


        lastPos = pos;

    }
    cleanPost += "<p>" + post.mid(lastPos, post.length()-lastPos) + "</p>";
    post = cleanPost;


    // ----------------------------------------------------
    // replace quotes

	cleanPost = "";
    lastPos = 0;
    pos = 0;
    while((pos = quoteRegexp.indexIn(post, pos)) != -1) {
        cleanPost += "<p>" + post.mid(lastPos, pos-lastPos) + "</p>";



        if(quoteRegexp.cap(4) == "</p><div class=\"container\"><table class=\"citation\">" || quoteRegexp.cap(4) == "<div class=\"container\"><table class=\"citation\">") {
            if(m_BlackList.find(quoteRegexp.cap(2).mid(0, quoteRegexp.cap(2).length()-10)) == m_BlackList.end())
                cleanPost += "<div class=\"quote\"><div class=\"header\"><p onclick=\"sendURL(\'REDIRECT:" + quoteRegexp.cap(1) + "\')\">" + quoteRegexp.cap(2) + "</p></div>" + quoteRegexp.cap(3) + "</p>";
            else cleanPost += "<div>";
            pos += quoteRegexp.matchedLength() - 51 + (quoteRegexp.cap(4) == "<div class=\"container\"><table class=\"citation\">" ? 4 : 0);
            // recursive quote...

            QRegExp nextClose("</td></tr></table></div>");
            nextClose.setCaseSensitivity(Qt::CaseSensitive);
            int indexNextClose = nextClose.indexIn(post, pos);
            int lastMatchingPos = 0;
            while((pos = quoteRegexp.indexIn(post, pos)) != -1 && indexNextClose != -1 ) {
                if(pos > indexNextClose)
                    break;

                cleanPost += "<div class=\"quote\"><div class=\"header\"><p onclick=\"sendURL(\'REDIRECT:" + quoteRegexp.cap(1) + "\')\">" + quoteRegexp.cap(2) + "</p></div>" + quoteRegexp.cap(3) + "</div>";
                pos += quoteRegexp.matchedLength();
                lastMatchingPos = pos;
                indexNextClose = nextClose.indexIn(post, pos);
            }
            cleanPost += post.mid(lastMatchingPos, indexNextClose-lastMatchingPos) + "</div>";

            pos = indexNextClose + nextClose.matchedLength();

        } else {
            if(m_BlackList.find(quoteRegexp.cap(2).mid(0, quoteRegexp.cap(2).length()-10)) == m_BlackList.end())
                cleanPost += "<div class=\"quote\"><div class=\"header\"><p onclick=\"sendURL(\'REDIRECT:" + quoteRegexp.cap(1) + "\')\">" + quoteRegexp.cap(2) + "</p></div>" + quoteRegexp.cap(3) + "</div>";
            pos += quoteRegexp.matchedLength();
        }


        lastPos = pos;

    }
    cleanPost += "<p>" + post.mid(lastPos, post.length()-lastPos) + "</p>";


    // ----------------------------------------------------
    // click on image open new view

    QRegExp imageWithHiddenLink("<a rel=\"nofollow\" href=\"([^\"]+)\" target=\"_blank\" class=\"cLink\"><img src=\"([^\"]+)\" alt=\"[^\"]+\" title=\"[^\"]+\" onload=\"[^\"]+\" style=\"margin: 5px\"/></a>");
    imageWithHiddenLink.setCaseSensitivity(Qt::CaseSensitive);
    imageWithHiddenLink.setMinimal(true);
    pos = 0;
    while((pos = imageWithHiddenLink.indexIn(cleanPost, pos)) != -1) {
        QString urlFullSizeImg = imageWithHiddenLink.cap(1);
        if(urlFullSizeImg.mid(0, 25) == "http://reho.st/view/self/") {
            urlFullSizeImg = QString("http://reho.st/self/") + urlFullSizeImg.mid(25); // if the image comes from reho.st, then I can show the biggest picture
        } else {
            urlFullSizeImg = imageWithHiddenLink.cap(2);                              // otherwise don't know how to get full size img -> use the src value in tag img...
        }

        cleanPost = cleanPost.mid(0, pos) + "<img onclick=\"sendURL('OPEN_IMAGE:" + urlFullSizeImg + "')\" src=\"" + imageWithHiddenLink.cap(2) + "\" /> "  + cleanPost.mid(pos+imageWithHiddenLink.matchedLength());

        pos += imageWithHiddenLink.cap(1).length();
    }


    QRegExp imageRegExp("<img src=\"([^\"]+)\"");           imageRegExp.setCaseSensitivity(Qt::CaseSensitive);
    QRegExp imageFromHFR("forum-images.hardware.fr");       imageFromHFR.setCaseSensitivity(Qt::CaseSensitive);
    pos = 0;
    while((pos = imageRegExp.indexIn(cleanPost, pos)) != -1) {
        if(imageFromHFR.indexIn(imageRegExp.cap(1), 0) == -1) {
            cleanPost = cleanPost.mid(0, pos) + "<img onclick=\"sendURL(\'OPEN_IMAGE:\' + this.src)\" src=\"" + imageRegExp.cap(1) + "\"" + cleanPost.mid(pos + imageRegExp.matchedLength());
        }

        pos += imageRegExp.matchedLength();
    }

    // ----------------------------------------------------
    // resize smiley if needed

    if(Settings::smileySize() != 2) {

        // resize default smileys
        QRegExp smileys("<img src=\"http://forum-images.hardware.fr/icones/");
        cleanPost.replace(smileys, "<img width=\"" + QString::number(Settings::smileySize()) + "%\" height=\"auto\" src=\"local:///assets/images/smiley/");

    }

    post = cleanPost;

}

void ShowThreadController::updateView() {

	// ----------------------------------------------------------------------------------------------
	// get the dataModel of the listview if not already available
	using namespace bb::cascades;


	if(m_WebView == NULL) {
		qWarning() << "did not received the listview. quit.";
		return;
	}

	QFile htmlTemplateFile(QDir::currentPath() + "/app/native/assets/template.html");
	if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
	    htmlTemplateFile.setFileName(QDir::currentPath() + "/app/native/assets/template_black.html");
	}
	QFile htmlEndTemplateFile(QDir::currentPath() + "/app/native/assets/template_end.html");
	if (htmlTemplateFile.open(QIODevice::ReadOnly) && htmlEndTemplateFile.open(QIODevice::ReadOnly)) {
	    QString htmlTemplate = htmlTemplateFile.readAll();


	    if(Settings::fontSize() != 25) {
	        htmlTemplate.replace("p { font-size: 25px;", "p { font-size: " + QString::number(Settings::fontSize()) + "px;");
	    }



	    QString endTemplate = htmlEndTemplateFile.readAll();

	    QString blackTheme = "style=\"font-size:25px; \"";
	    if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
	        blackTheme = " style=\"background:#2E2E2E; font-size:25px; \" ";
	    }

	    QString pageContent;
	    for(int i = 0 ; i < m_Datas->length() ; ++i) {

	        QString RenderInNonFiltered = "";

	        if(!m_Datas->at(i)->getNotFilteredUrl().isEmpty()) {
	            RenderInNonFiltered = "<br/>&nbsp;<div class=\"showUnfiltered\" onclick=\"sendURL(\'REDIRECT:" + m_Datas->at(i)->getNotFilteredUrl() + "\')\"><p>" + tr("Show post in a non-filtered thread") + "</p></div>";
	        }

	        QRegExp isModo("Mod.ration");
	        if(isModo.indexIn(m_Datas->at(i)->getAuthor()) != -1) {
	            pageContent +=
	            QString("<div class=\"PostHeader moderator\" ontouchstart=\"itemTapped(" + QString::number(m_Datas->at(i)->getIndex()) + ", " + QString::number(isOwnMessage(m_Datas->at(i)->getAuthor())) + ")\" ontouchend=\"itemReleased();\" id=\"postHeader" + QString::number(m_Datas->at(i)->getIndex()) + "\">")
	                        + "<div style=\"height:80%; width:auto; position:relative; top:10%; left:5px; width:100px; display: inline-block;\" ><img onclick=\"addItemTapped(" + QString::number(m_Datas->at(i)->getIndex()) + ")\"  src=\"" + m_Datas->at(i)->getAvatar() + "\" style=\"height:100%; width:auto; position:relative; margin-left: auto; margin-right: auto; display: inline-block;\" /></div>"
	                        + "<div class=\"PostHeader-Text moderator\">"
	                            + "<div style=\"position:relative; top:-20px;\"><p class=\"moderator\" style=\"font-size:25px; \">" + m_Datas->at(i)->getAuthor() + "</p></div>"
	                            + "<div style=\"position:relative; top:-35px; font-size:small;\"><p class=\"moderator\" style=\"font-size:25px; \">" + m_Datas->at(i)->getTimestamp() + "</p></div>"
	                        + RenderInNonFiltered +" </div>"
	                     + "</div><p>" + m_Datas->at(i)->getPost() + "</p>";
	        } else {
	            pageContent +=
	            QString("<div class=\"PostHeader\" ontouchstart=\"itemTapped(" + QString::number(m_Datas->at(i)->getIndex()) + ", " + QString::number(isOwnMessage(m_Datas->at(i)->getAuthor())) + ")\" ontouchend=\"itemReleased();\" id=\"postHeader" + QString::number(m_Datas->at(i)->getIndex()) + "\">")
                            + "<div style=\"height:80%; width:auto; position:relative; top:10%; left:5px; width:100px; display: inline-block;\" ><img onclick=\"addItemTapped(" + QString::number(m_Datas->at(i)->getIndex()) + ")\"  src=\"" + m_Datas->at(i)->getAvatar() + "\" style=\"height:100%; width:auto; margin-left: auto; margin-right: auto; display: block;\" /></div>"
	                        + "<div class=\"PostHeader-Text\">"
	                            + "<div style=\"position:relative; top:-20px;\"><p " + blackTheme +">" + m_Datas->at(i)->getAuthor() + "</p></div>"
	                            + "<div style=\"position:relative; top:-35px; font-size:small;\"><p " + blackTheme +">" + m_Datas->at(i)->getTimestamp() + "</p></div>"
	                        + "</div>"
	                        + "</div>" + RenderInNonFiltered +" <p>" + m_Datas->at(i)->getPost() + "</p>";
	        }





// In case we use jQuery mobile...
/*
            pageContent +=
            QString("<div class=\"PostHeader\" onclick=\"addItemTapped(" + QString::number(m_Datas->at(i)->getIndex()) + ")\" id=\"postHeader" + QString::number(m_Datas->at(i)->getIndex()) + "\">")
                        + "<img onclick=\"addItemTapped(" + QString::number(m_Datas->at(i)->getIndex()) + ")\"  src=\"" + m_Datas->at(i)->getAvatar() + "\" style=\"height:80%; width:auto; position:relative; top:10%; left:5px; max-width:100px; display: inline-block;\" />"
                        + "<div class=\"PostHeader-Text\">"
                            + "<div style=\"position:relative; top:-20px;\"><p " + blackTheme +">" + m_Datas->at(i)->getAuthor() + "</p></div>"
                            + "<div style=\"position:relative; top:-35px; font-size:small;\"><p " + blackTheme +">" + m_Datas->at(i)->getTimestamp() + "</p></div>"
                        + "</div>"
                     + "</div><p>" + m_Datas->at(i)->getPost() + "</p>";

            pageContent +=
               QString("<script type=\"text/javascript\"> $(function(){ $( \"#postHeader" + QString::number(m_Datas->at(i)->getIndex()) + "\").bind( \"taphold\", tapholdHandler ); function tapholdHandler( event ){ $( event.target ).addClass( \"taphold\" ); toggleSelectItem(" + QString::number(m_Datas->at(i)->getIndex()) + "); $( \"#right\" ).panel( \"open\" , \"" + QString::number(m_Datas->at(i)->getIndex()) + "\" ); }}); </script>");
*/

	    }

	    m_WebView->setHtml(htmlTemplate + pageContent + endTemplate, QUrl("local:///assets/"));
/*
	    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	        QFile file(directory + "/tmp.txt");
	        if (file.open(QIODevice::WriteOnly)) {
	            QTextStream stream(&file);

	            stream << (htmlTemplate + pageContent + endTemplate);

	            file.close();
	        }*/

	} else {
	    qDebug() << "file not found";
	}

}

void ShowThreadController::notifyItemLoaded() {
	scrollToItem();
}

void ShowThreadController::scrollToItem() {
    if(m_WebView == NULL) return;

	m_ScrollAtLocation = true;

	QRegExp goToPost("#t([0-9]+)");
	if(goToPost.indexIn(m_Url, 0) != -1) {
		m_WebView->evaluateJavaScript("scrollToElement(\'postHeader" + goToPost.cap(1) + "\');");

	} else {
		QRegExp goToEnd("#bas");
		if(goToEnd.indexIn(m_Url, 0) != -1) {
		    m_WebView->evaluateJavaScript("scrollToEndPage();");
		}
	}


}


// -----------------------------------------------------------------------------------------------------
// navigation within page

void ShowThreadController::doAction(int code) {
    switch(code) {

        case 1:
            m_WebView->evaluateJavaScript("scrollToTop();");
            break;

        case 2:
            m_WebView->evaluateJavaScript("scrollToEndPage();");
            break;

        case 3:
            nextPage();
            break;

        case 4:
            prevPage();
            break;

        case 5:
            lastPage();
            break;

        case 6:
            firstPage();
            break;

        case 7:
        {
            QRegExp goToPost("#t([0-9]+)");
            int pos = 0;
            if((pos = goToPost.indexIn(m_Url, 0)) != -1) {
                showThread(m_Url.mid(0, pos) + "#bas");
            } else {
                QRegExp goToEnd("#bas");
                if(goToEnd.indexIn(m_Url, 0) != -1) {
                    showThread(m_Url);
                    return;
                }
            }

            showThread(m_Url + "#bas");
            break;
        }

        case 10:
            m_WebView->storage()->clearCache();
            break;


        case 11:
        {
            bb::system::Clipboard clipboard;
            clipboard.clear();
            clipboard.insert("text/plain", (DefineConsts::FORUM_URL + m_Url).toAscii());

            bb::system::SystemToast *toast = new bb::system::SystemToast(this);
            toast->setBody(tr("Link to current page copied to clipboard"));
            toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
            toast->show();
            break;
        }

        case 12:
        {
            if(m_DialogSearch == NULL) {
                m_DialogSearch = new DialogSearch();
                bool check = QObject::connect(m_DialogSearch, SIGNAL(validated(const QString&, const QString&, bool)), this, SLOT(intraSearch(const QString&, const QString&, bool)));
                Q_ASSERT(check);

            }
            m_DialogSearch->setVisible(true);
        }

    }
}


void ShowThreadController::nextPage() {
	if(!m_UrlNextPage.isEmpty())
		showThread(m_UrlNextPage);
	else {
		QRegExp goToPost("#t([0-9]+)");
		int pos = 0;
		if((pos = goToPost.indexIn(m_Url, 0)) != -1) {
			showThread(m_Url.mid(0, pos) + "#bas");
		} else {
			QRegExp goToEnd("#bas");
			if(goToEnd.indexIn(m_Url, 0) != -1) {
				showThread(m_Url);
				return;
			}
		}

		showThread(m_Url + "#bas");
	}
}


void ShowThreadController::prevPage() {
	if(!m_UrlPrevPage.isEmpty())
		showThread(m_UrlPrevPage);
	else
		showThread(m_Url);
}


void ShowThreadController::firstPage() {
	if(!m_UrlFirstPage.isEmpty())
		showThread(m_UrlFirstPage);
	else
		showThread(m_Url);
}

void ShowThreadController::lastPage(bool bas, bool afterNewPost) {

    qDebug() << "GOTO LAST PAGE";
    EASTER3

	if(!m_UrlLastPage.isEmpty()) {
		if(!bas)
			showThread(m_UrlLastPage);
		else {
			QRegExp goToPost("#t([0-9]+)");
			int pos = 0;
			if((pos = goToPost.indexIn(m_UrlLastPage, 0)) != -1) {
					showThread(m_UrlLastPage.mid(0, pos) + "#bas");
			} else {
				QRegExp goToEnd("#bas");
				if(goToEnd.indexIn(m_UrlLastPage, 0) != -1) {
					showThread(m_UrlLastPage);
					return;
				}
			}
			showThread(m_UrlLastPage + "#bas");
		}
	} else {
		QRegExp goToPost("#t([0-9]+)");
		int pos = 0;
		if((pos = goToPost.indexIn(m_Url, 0)) != -1) {
			showThread(m_Url.mid(0, pos) + "#bas");
		} else {
			QRegExp goToEnd("#bas");
			if(goToEnd.indexIn(m_Url, 0) != -1) {
				showThread(m_Url);
				return;
			}
		}

		showThread(m_Url + "#bas");
	}
}



void ShowThreadController::vote(const QString &vote) {

	const QUrl url(DefineConsts::FORUM_URL + "/user/vote.php?config=hfr.inc");


	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	// <form method="post" action="/user/vote.php?config=hfr.inc"><input type="hidden" name="hash_check" value="867ea14e7a8080bb5a9b490844ea2b20" />
	// <div class="sondage"><input type="hidden" name="cat" value="23" /><input type="hidden" name="p" value="1" />
	// <input type="hidden" name="page" value="1" /><input type="hidden" name="sondage" value="1" /><input type="hidden" name="owntopic" value="1" />
	// <input type="hidden" name="subcat" value="529" /><input type="hidden" name="numeropost" value="27709" />

	QUrl params;
	params.addQueryItem("hash_check", m_HashCheck);
	params.addQueryItem("p", "1");
	params.addQueryItem("cat", m_CatID);
	params.addQueryItem("sondage", "1");
	params.addQueryItem("page", "1");
	params.addQueryItem("owntopic", "1");
	params.addQueryItem("numeropost", m_PostID);


	if(vote[0] == '1') {
		int selected = 0;
		int value = vote.mid(1).toInt();
		while(value != 0) {
			value /= 2;
			selected++;
		}

		params.addQueryItem("reponse", QString::number(selected));
	} else {
		int value = vote.mid(1).toInt();
		int i = 1;
		while(value != 0) {
			if(value % 2) {
				params.addQueryItem("reponse"+ QString::number(i), "1");
			}
			value /= 2;
			++i;
		}
	}



	QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkSurveyReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

}


void ShowThreadController::checkSurveyReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);

				showThread(m_Url);
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}


void ShowThreadController::setActionListView(QObject *list) {
    using namespace bb::cascades;

    m_ListView = dynamic_cast<bb::cascades::ListView*>(list);

    if(m_ListView == NULL)
        return;


    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if (dataModel) {
        dataModel->clear();
    } else {
        dataModel = new GroupDataModel(
                QStringList() << "image"
                              << "category"
                              << "caption"
                              << "action"
                 );
        m_ListView->setDataModel(dataModel);
    }



    // ----------------------------------------------------------------------------------------------
    // push data to the view




    QFile actionFile(QDir::currentPath() + tr("/app/native/assets/model/action_list.xml"));
//    if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
//        htmlTemplateFile.setFileName(QDir::currentPath() + "/app/native/assets/template_black.html");
//    }



    QRegExp caption("title=\"([^\"]+)\"");
    QRegExp category("category=\"([^\"]+)\"");
    QRegExp image("url=\"([^\"]+)\"");
    QRegExp actionReg("action=\"([0-9]+)\"");

    if (actionFile.open(QIODevice::ReadOnly)) {
        QString actionList = actionFile.readAll();

        int pos = 0;
        while(pos != -1) {
            ActionComposerItem *action = NULL;
            pos = caption.indexIn(actionList, pos);
            if(pos != -1) {
                action = new ActionComposerItem(this);
                action->setCaption(caption.cap(1));
                pos += caption.matchedLength();
            } else break;

            pos = category.indexIn(actionList, pos);
            pos += category.matchedLength();
            action->setCategory(category.cap(1));

            pos = image.indexIn(actionList, pos);
            pos += image.matchedLength();
            action->setImage(image.cap(1));

            pos = actionReg.indexIn(actionList, pos);
            pos += actionReg.matchedLength();
            action->setAction(actionReg.cap(1).toInt());



            dataModel->insert(action);

        }


    }

}



