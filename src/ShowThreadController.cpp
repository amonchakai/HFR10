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

#include <bb/cascades/ListView>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>
#include <bb/system/SystemToast>

#include  "Globals.h"
#include  "HFRNetworkAccessManager.hpp"
#include  "Network/WebResourceManager.h"
#include  "DataObjects.h"
#include  "Settings.hpp"


ShowThreadController::ShowThreadController(QObject *parent)
	: QObject(parent), m_ListView(NULL), m_Datas(new QList<PostDetailItem*>), m_AddSignature(false), m_ScrollAtLocation(false), m_NbWebviewLoaded(0) {
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

	if(m_ListView != NULL) {
		bb::cascades::GroupDataModel* dataModel = dynamic_cast<bb::cascades::GroupDataModel*>(m_ListView->dataModel());
		if (dataModel) {
			dataModel->clear();
		}
	}

}



void ShowThreadController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			qDebug() << "number of bytes retrieved: " << reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				parse(response);
			}
		} else {
			connectionTimedOut();
		}

		reply->deleteLater();
	}
}

void ShowThreadController::connectionTimedOut() {

	bb::system::SystemToast *toast = new bb::system::SystemToast(this);

	toast->setBody(tr("Connection timed out"));
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();

	emit complete();
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

/*
	qDebug() << m_UrlFirstPage;
	qDebug() << m_UrlLastPage;
	qDebug() << m_UrlPrevPage;
	qDebug() << m_UrlNextPage;
	qDebug() << m_Url;
*/

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
	// Parse posts

	QRegExp regexp = QRegExp(QString("<td class=\"messCase1\" width=\"180\" valign=\"top\" rowspan=\"1\"><a name=\"t([0-9]+)\">")  	// post index
								   + ".*</a></div><div><b class=\"s2\">(.+)</b></div>");											// pseudo

	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


	int pos = 0;
	int lastPos = regexp.indexIn(page, pos);

	QString lastPseudo;
	QString lastPostIndex;
	if(lastPos != -1) {
		lastPostIndex = regexp.cap(1);
		lastPseudo = regexp.cap(2);

		parseSurvey(page.mid(0, lastPos));
	}


	m_Datas->clear();				// cleanup data before loading new data

	// Cut the entire page into posts
	while((pos = regexp.indexIn(page, lastPos)) != -1) {
		pos += regexp.matchedLength();

		// parse each post individually
		parsePost(lastPostIndex, lastPseudo, page.mid(lastPos, pos-lastPos));


		lastPos = pos;
		lastPostIndex = regexp.cap(1);
		lastPseudo = regexp.cap(2);

	}

	parsePost(lastPostIndex, lastPseudo, page.mid(lastPos, pos-lastPos));
	parseDataForReply(page.mid(lastPos, pos-lastPos));

	PostDetailItem *item = m_Datas->last();

	if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
		item->getPost() += "<br /><br /><div style=\"background-color:#262626; text-align:center; \">" + currentPageNumber + "/" + lastPageNumber + "</div>";
	} else {
		item->getPost() += "<br /><br /><div style=\"background-color:#f5f5f5; text-align:center; \">" + currentPageNumber + "/" + lastPageNumber + "</div>";
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
		m_Datas->last()->setAvatar("asset:///images/default_avatar.png");
	else
		m_Datas->last()->setAvatar(avatar);

	// add information about number of quotes
	if(quoteUrl.indexIn(post, 0) != -1) {
		QString s = quoteUrl.cap(1);
		s.replace(QRegExp("&amp;"), "&");

		if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
			postContent += "<div style=\"background-image: url('local:///assets/images/icon_quoted_white.png'); width:48px; height:44px; float:right; \" ><p onclick=\"sendURL(\'" + s + "\')\" style=\"text-align:center; color:black; margin-top:0px \" >" + quoteUrl.cap(2) +"</p></div>";
		} else {
			postContent += "<div style=\"background-image: url('local:///assets/images/icon_quoted.png'); width:48px; height:44px; float:right; \" ><p onclick=\"sendURL(\'" + s + "\')\" style=\"text-align:center; color:white; margin-top:0px; \" >" + quoteUrl.cap(2) +"</p></div>";
		}

	}

	// parse the post so it can be rendered in HTML within a listitem
	cleanupPost(postContent);

	m_Datas->last()->setAuthor(author);
	m_Datas->last()->setTimestamp(timestamp);
	m_Datas->last()->setPost(postContent);
	m_Datas->last()->setIndex(postIdex.toInt());

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

		pos += question.matchedLength();

		QRegExp answers("<div class=\"barre\" style=\"width:[0-9.]+%\">&nbsp;</div><div style=\"position:absolute;left:0px\" class=\"sondageTop\">[0-9.]+&nbsp;%</div><div class=\"sondageTop\">[&nbsp;]+([0-9]+) vote[s]*</div></div><div class=\"sondageRight\">(.+)</div>");
		answers.setCaseSensitivity(Qt::CaseSensitive);
		answers.setMinimal(true);

		QString colorHandling = "} ";
		if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
			colorHandling = "background-color:#000000; color:#FFFFFF; } ";
		}

		m_Survey = QString("<!DOCTYPE html><html><head><style type=\"text/css\">")
							 + "body {font-size:" + QString::number(Settings::fontSize())  + "px; " + colorHandling
							 + "p {font-size:" + QString::number(Settings::fontSize()) + "px;} "
							 + "#parent { overflow: hidden; } .right { float:right; width:400px; background-color: steelblue; text-align: right; padding: 3px; margin: 1px; color: white; } .left { overflow: hidden; } </style></head><body><div class=\"survey\"><p style=\"text-decoration:underline; font-weight:bold;\">"
							 + question.cap(1) + "</p>";

		QList<QString> options;
		QList<int> nbVotes;

		int overallVoteNumber = 0;
		while((pos = answers.indexIn(page, pos)) != -1) {
			options.push_back(answers.cap(2));
			nbVotes.push_back(answers.cap(1).toInt());

			overallVoteNumber += nbVotes.last();
			pos += answers.matchedLength();
		}

		for(int i = 0 ; i < options.length() ; ++i) {
			m_Survey += "<div id=\"parent\"><div class=\"right\" style=\"width: " + QString::number(static_cast<int>(nbVotes[i]*600/overallVoteNumber)) + "px;\">" + QString::number(nbVotes[i]) + "</div><div class=\"left\">" + options[i] + "</div></div>";
		}

		m_Survey += "</body></html>";

	} else {
		pos = 0;
		QRegExp newQuestion("<input type=\"hidden\" name=\"numeropost\" value=\"([0-9]+)\" /><input type=\"hidden\" name=\"hash_check\" value=\"[a-zA-Z0-9]+\" /><b class=\"s2\">(.*)</b>");
		newQuestion.setCaseSensitivity(Qt::CaseSensitive);
		newQuestion.setMinimal(true);

		QString colorHandling = "} ";
		if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
			colorHandling = "background-color:#000000; color:#FFFFFF; } ";
		}

		if((pos = newQuestion.indexIn(page, pos)) != -1) {
			m_Survey = QString("<!DOCTYPE html><html><head><style type=\"text/css\">")
										 + "body {font-size:" + QString::number(Settings::fontSize())  + "px; " + colorHandling
										 + "p {font-size:" + QString::number(Settings::fontSize()) + "px;} "
										 + "</style></head><body><div class=\"survey\"><p style=\"text-decoration:underline; font-weight:bold;\">"
										 + newQuestion.cap(2) + "</p><ol type=\"1\">";

			//<input class="checkbox" type="checkbox" value="1" id="sond1" name="reponse1" /><label for="sond1">option 1</label>
			QRegExp answers("type=\"([radiochekbx]+)\" value=\"[0-9]+\" id=\"sond[0-9]+\" name=\"reponse[0-9]*\" /><label for=\"sond[0-9]+\">(.+)</label>");
			answers.setCaseSensitivity(Qt::CaseSensitive);
			answers.setMinimal(true);

			int respIDX = 1;
			bool dataType = false;
			QString listSelectedItemsFunctor("function getSelectedItems() { var ret=0; ");
			while((pos = answers.indexIn(page, pos)) != -1) {
				dataType = answers.cap(1).at(0) == 'r';
				qDebug() << answers.cap(1) <<  answers.cap(2);
				m_Survey += "<li><input type=\"" + answers.cap(1) + "\" value=\"" + QString::number(respIDX) + "\" id=\"sond" + QString::number(respIDX) + "\" name=\"reponse\" />" + answers.cap(2) + "</li>";
				listSelectedItemsFunctor += "if(document.getElementById(\"sond" + QString::number(respIDX) + "\").checked) { ret += Math.pow(2," + QString::number(respIDX-1) +"); } ";
				pos += answers.matchedLength();
				++respIDX;
			}
			listSelectedItemsFunctor += " return ret; }";
			m_Survey += QString("</ol><br /><input type=\"submit\" onclick=\"navigator.cascades.postMessage(\'") + (dataType ? "1" : "0") + "\' + getSelectedItems().toString())\" name=\"sondage_submit\" value=\"" + tr("Vote") + "\" /><script>"+ listSelectedItemsFunctor +"</script></body></html>";

		} else {
			QString colorHandling = "} ";
			if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
				colorHandling = "background-color:#000000; color:#FFFFFF; } ";
			}

			m_Survey = QString("<!DOCTYPE html><html><head><style type=\"text/css\">")
							 + "body {font-size:" + QString::number(Settings::fontSize())  + "px; " + colorHandling
							 + "p {font-size:" + QString::number(Settings::fontSize()) + "px;} "
							 + "#parent { overflow: hidden; } .right { float:right; width:400px; background-color: steelblue; text-align: right; padding: 3px; margin: 1px; color: white; } .left { overflow: hidden; } </style></head><body></body></html>";
		}
	}


	emit surveyUpdated();
}

void ShowThreadController::addToFavorite(int responseID) {
	const QUrl url(DefineConsts::FORUM_URL + "/user/addflag.php?config=hfr.inc&cat=" + m_CatID + "&post=" + m_PostID + "&numreponse=" + QString::number(responseID));


	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	qDebug() << url.toString();

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
			qDebug() << "number of bytes retrieved: " << reply->bytesAvailable();
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

void ShowThreadController::checkSuccessDeletePost() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			qDebug() << "number of bytes retrieved: " << reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				qDebug() << response;
				showThread(m_Url);
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}


void ShowThreadController::cleanupPost(QString &post) {
	// ----------------------------------------------------
	// declare regexp used to clean the posts

	QString cleanPost;
	QRegExp quoteRegexp(QString( "<div class=\"container\"><table class=\"citation\"><tr class=\"none\"><td><b class=\"s1\"><a href=\"(.*[0-9]+)\" class=\"Topic\">")
								+"(.+)"														// author
								+"</a></b><br /><br />[&nbsp;]*<p>(.+)</p></td></tr></table></div>"	// message
			);
	quoteRegexp.setCaseSensitivity(Qt::CaseSensitive);
	quoteRegexp.setMinimal(true);

	QRegExp spoilerRegExp("<div class=\"container\"><table class=\"spoiler\".*class=\"s1Topic\">Spoiler :</b><br /><br /><div class=\"Topic masque\"><p>(.*)</p></div></td></tr></table></div>");
	spoilerRegExp.setCaseSensitivity(Qt::CaseSensitive);
	spoilerRegExp.setMinimal(true);

	QRegExp simpleQuoteRegexp("<div class=\"container\"><table class=\"quote\"><tr class=\"none\"><td><b class=\"s1\">Citation :</b><br /><br /><p>(.*)</p></td></tr></table></div>");
	simpleQuoteRegexp.setCaseSensitivity(Qt::CaseSensitive);
	simpleQuoteRegexp.setMinimal(true);

	QRegExp codeRegexp("<table class=\"code\"><tr class=\"none\"><td><b class=\"s1\" style=\"font-family: Verdana, Helvetica, Arial, Sans-serif;\">Code :</b><br />(.*)</td></tr></table>");
	codeRegexp.setCaseSensitivity(Qt::CaseSensitive);
	codeRegexp.setMinimal(true);

	// ----------------------------------------------------
	// replace quotes

	int lastPos = 0;
	int pos = 0;
	while((pos = quoteRegexp.indexIn(post, pos)) != -1) {
		cleanPost += "<p>" + post.mid(lastPos, pos-lastPos) + "</p>";

		cleanPost += "<div class=\"quote\"><div class=\"header\" onclick=\"sendURL(\'" + quoteRegexp.cap(1) + "\')\">" + quoteRegexp.cap(2) + "</div>" + quoteRegexp.cap(3) + "</div>";
		pos += quoteRegexp.matchedLength();
		lastPos = pos;
	}
	cleanPost += "<p>" + post.mid(lastPos, post.length()-lastPos) + "</p>";



	// ----------------------------------------------------
	// resize images if needed

	if(Settings::smileySize() != 2) {

		// resize default smileys
		QRegExp smileys("<img src=\"http://forum-images.hardware.fr/icones/");
		cleanPost.replace(smileys, "<img width=\"" + QString::number(Settings::smileySize()) + "%\" height=\"" + QString::number(Settings::smileySize())  + "%\" src=\"local:///assets/images/smiley/");

	}

	post = cleanPost;



	// ----------------------------------------------------
	// handle spoilers

	cleanPost = "";
	lastPos = 0;
	pos = 0;
	while((pos = spoilerRegExp.indexIn(post, pos)) != -1) {
		cleanPost += "<p>" + post.mid(lastPos, pos-lastPos) + "</p>";

		cleanPost += "<div class=\"c1\" style=\"border:1px solid; border-color:gray;\" onclick=\"javascript:montrer_spoiler(\'spoiler" + QString::number(pos) + "\')\"><u><strong>Spoiler :</strong></u><script type=\"text/javascript\"> function montrer_spoiler(value){var actual=document.getElementById(value).style.visibility;if (actual==\'visible\'){document.getElementById(value).style.visibility=\'hidden\';}else{document.getElementById(value).style.visibility=\'visible\';}} </script><dl style=\"visibility: hidden;\" id=\"spoiler" + QString::number(pos) + "\"><dd>" + spoilerRegExp.cap(1) + "</dd></dl></div>";
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

		cleanPost += "<div class=\"quote\"><div class=\"header\" >Citation :</div>" + simpleQuoteRegexp.cap(1) + "</div>";
		pos += simpleQuoteRegexp.matchedLength();
		lastPos = pos;
	}
	cleanPost += "<p>" + post.mid(lastPos, post.length()-lastPos) + "</p>";
	post = cleanPost;

}

void ShowThreadController::updateView() {

	// ----------------------------------------------------------------------------------------------
	// get the dataModel of the listview if not already available
	using namespace bb::cascades;


	if(m_ListView == NULL) {
		qWarning() << "did not received the listview. quit.";
		return;
	}

	GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
	if (dataModel) {
		dataModel->clear();
	} else {
		qDebug() << "create new model";
		dataModel = new GroupDataModel(
					QStringList() << "author"
								  << "avatar"
								  << "timestamp"
								  << "post"
								  << "editUrl"
				);
		m_ListView->setDataModel(dataModel);
	}

	// ----------------------------------------------------------------------------------------------
	// push data to the view

	QList<QObject*> datas;
	for(int i = m_Datas->length()-1 ; i >= 0 ; --i) {
		datas.push_back(m_Datas->at(i));
	}

	dataModel->clear();
	dataModel->insertList(datas);

}

void ShowThreadController::notifyItemLoaded() {
	++m_NbWebviewLoaded;
	if(m_NbWebviewLoaded > m_Datas->length()*.2 && !m_ScrollAtLocation) {
		scrollToItem();
	}
}

void ShowThreadController::scrollToItem() {
	m_ScrollAtLocation = true;

	QRegExp goToPost("#t([0-9]+)");
	if(goToPost.indexIn(m_Url, 0) != -1) {
		int lookIdx = goToPost.cap(1).toInt();

		int gotoItem = 0;
		for(int i = m_Datas->length()-1 ; i >= 0 ; --i) {
			if(lookIdx == m_Datas->at(i)->getIndex()) {
				gotoItem = i;
				break;
			}
		}

		m_ListView->scrollToItem(QVariantList() << 0 << gotoItem, bb::cascades::ScrollAnimation::None);

	} else {
		QRegExp goToEnd("#bas");
		if(goToEnd.indexIn(m_Url, 0) != -1) {
			m_ListView->scrollToItem(QVariantList() << 0 << m_Datas->length()-1, bb::cascades::ScrollAnimation::None);
		}
	}


}


// -----------------------------------------------------------------------------------------------------
// navigation within page

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

void ShowThreadController::lastPage(bool bas) {
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
			qDebug() << "number of bytes retrieved: " << reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);

				refreshSurvey();
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}


void ShowThreadController::refreshSurvey() {
	m_ScrollAtLocation = false;
	m_NbWebviewLoaded = 0;

	QNetworkRequest request(DefineConsts::FORUM_URL+m_Url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkSurvey()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}


void ShowThreadController::checkSurvey() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			qDebug() << "number of bytes retrieved: " << reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);

				parseSurvey(response);
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}
