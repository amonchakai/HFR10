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
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}



void ShowThreadController::parse(const QString &page) {

	// ----------------------------------------------------------------------------------------------
	// Navigation within the topic


	QRegExp amp("&amp;");
	QRegExp pageURL("</b>&nbsp;&nbsp;<a href=\"(.+)\" class=\"cHeader\">1</a>");
	pageURL.setCaseSensitivity(Qt::CaseSensitive);
	pageURL.setMinimal(true);

	if(pageURL.indexIn(page, 0) != -1) {
		m_UrlFirstPage = pageURL.cap(1);
		m_UrlFirstPage.replace(amp, "&");
	} else
		m_UrlFirstPage = "";


	pageURL = QRegExp("</b>&nbsp;&nbsp;<a href=\"(.+)\" class=\"cHeader\">[0-9]+</a></div><div class=\"pagepresuiv\">");
	pageURL.setCaseSensitivity(Qt::CaseSensitive);
	pageURL.setMinimal(true);

	if(pageURL.indexIn(page, 0) != -1) {
		m_UrlLastPage = pageURL.cap(1);
		m_UrlLastPage.replace(amp, "&");
	} else
		m_UrlLastPage = "";

	pageURL = QRegExp("<div class=\"pagepresuiv\"><a href=\"(.+)\" class=\"cHeader\" accesskey=\"w\">Page Pr");
	pageURL.setCaseSensitivity(Qt::CaseSensitive);
	pageURL.setMinimal(true);

	if(pageURL.indexIn(page, 0) != -1) {
		m_UrlPrevPage = pageURL.cap(1);
		m_UrlPrevPage.replace(amp, "&");
	} else
		m_UrlPrevPage = "";

	pageURL = QRegExp("<div class=\"pagepresuiv\"><a href=\"(.+)\" class=\"cHeader\" accesskey=\"x\">Page Suivante</a></div>");
	pageURL.setCaseSensitivity(Qt::CaseSensitive);
	pageURL.setMinimal(true);

	if(pageURL.indexIn(page, 0) != -1) {
		m_UrlNextPage = pageURL.cap(1);
		m_UrlNextPage.replace(amp, "&");
	} else
		m_UrlNextPage = "";



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
			qDebug() << response;
		}

		reply->deleteLater();
	}
}

void ShowThreadController::cleanupPost(QString &post) {
	//qDebug() << post;
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

	int lastPos = 0;
	int pos = 0;
	while((pos = quoteRegexp.indexIn(post, pos)) != -1) {
		cleanPost += "<p>" + post.mid(lastPos, pos-lastPos) + "</p>";

		cleanPost += "<div class=\"quote\"><div class=\"header\" onclick=\"sendURL(\'" + quoteRegexp.cap(1) + "\')\">" + quoteRegexp.cap(2) + "</div>" + quoteRegexp.cap(3) + "</div>";
		pos += quoteRegexp.matchedLength();
		lastPos = pos;
	}
	cleanPost += "<p>" + post.mid(lastPos, post.length()-lastPos) + "</p>";

	if(Settings::smileySize() != 2) {

		// resize default smileys
		QRegExp smileys("<img src=\"http://forum-images.hardware.fr/icones/");
		cleanPost.replace(smileys, "<img width=\"" + QString::number(Settings::smileySize()) + "%\" height=\"" + QString::number(Settings::smileySize())  + "%\" src=\"http://forum-images.hardware.fr/icones/");

	}

	post = cleanPost;

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
