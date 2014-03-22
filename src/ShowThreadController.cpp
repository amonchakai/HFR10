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

#include  "Globals.h"
#include  "HFRNetworkAccessManager.hpp"
#include  "Network/WebResourceManager.h"
#include  "DataObjects.h"

bb::cascades::AbstractPane *ShowThreadController::m_Pane = NULL;


ShowThreadController::ShowThreadController(QObject *parent)
	: QObject(parent), m_DataModel(NULL), m_Datas(new QList<PostDetailItem>) {
}


void ShowThreadController::setAbstractPane(bb::cascades::AbstractPane *root) {
	m_Pane = root;
}


void ShowThreadController::showThread(const QString &url) {

	QNetworkRequest request(DefineConsts::FORUM_URL+url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

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

	emit complete(response);
}



void ShowThreadController::parse(const QString &page) {

	qDebug() << "start parser";

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

	while((pos = regexp.indexIn(page, lastPos)) != -1) {
		pos += regexp.matchedLength();

		parsePost(lastPostIndex, lastPseudo, page.mid(lastPos, pos-lastPos));


		lastPos = pos;
		lastPostIndex = regexp.cap(1);
		lastPseudo = regexp.cap(2);

	}
	parsePost(lastPostIndex, lastPseudo, page.mid(lastPos, pos-lastPos));


	qDebug() << "end parsing";

	updateView();
}


void ShowThreadController::parsePost(const QString &postIdex, const QString &author, const QString &post) {
	QRegExp moodRegexp = QRegExp("<span class=\"MoodStatus\">(.+)</span>");
	moodRegexp.setCaseSensitivity(Qt::CaseSensitive);
	moodRegexp.setMinimal(true);

	QRegExp avatarRegexp = QRegExp("<div class=\"avatar_center\" style=\"clear:both\"><img src=\"(.*|)\" alt=\"\" /></div>");
	avatarRegexp.setCaseSensitivity(Qt::CaseSensitive);
	avatarRegexp.setMinimal(true);

	QRegExp timestampRegexp = QRegExp(QString( "<div class=\"left\">(.+)<a href="));
	timestampRegexp.setCaseSensitivity(Qt::CaseSensitive);
	timestampRegexp.setMinimal(true);

	QRegExp postContentRegexp = QRegExp(QString("</div></div><div id=\"para[0-9]+\"><p>(.*)<div style=\"clear: both;\">"));
	postContentRegexp.setCaseSensitivity(Qt::CaseSensitive);
	postContentRegexp.setMinimal(true);

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

	m_Datas->push_back(PostDetailItem());

	if(avatar.isEmpty())
		m_Datas->last().avatar_url = "asset:///images/default_avatar.png";
	else
		m_Datas->last().avatar_url = avatar;


	m_Datas->last().author = author;
	m_Datas->last().timestamp = timestamp;
	m_Datas->last().post = postContent;

}

void ShowThreadController::updateView() {

	// ----------------------------------------------------------------------------------------------
	// get the dataModel of the listview if not already available

	if(m_DataModel == NULL) {
		using namespace bb::cascades;

		ListView *listView = NULL;
		if(m_Pane != NULL) {
			listView = m_Pane->findChild<ListView *>("threadView");
			if(listView == NULL) {
				qWarning() << "Object null: list view not found in the QML tree!";
				return;
			}
		} else {
			qWarning() << "setAbstractPane was called too late!";
			return;
		}

		m_DataModel = dynamic_cast<GroupDataModel*>(listView->dataModel());
		if (m_DataModel) {
			m_DataModel->clear();
		} else {
			m_DataModel = new GroupDataModel(
						QStringList() << "author"
									  << "avatar"
									  << "timestamp"
									  << "post"
					);
			listView->setDataModel(m_DataModel);
		}
	} else {
		m_DataModel->clear();
	}

	// ----------------------------------------------------------------------------------------------
	// push data to the view

	for(int i = m_Datas->length()-1 ; i >= 0 ; --i) {
		QVariantMap post;
		post["author"] = m_Datas->at(i).author;
		post["timestamp"] = m_Datas->at(i).timestamp;
		post["avatar"] = m_Datas->at(i).avatar_url;
		post["post"] = m_Datas->at(i).post;

		m_DataModel->insert(post);
	}

}

