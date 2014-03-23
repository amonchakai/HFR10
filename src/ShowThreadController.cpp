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


#include  "Globals.h"
#include  "HFRNetworkAccessManager.hpp"
#include  "Network/WebResourceManager.h"
#include  "DataObjects.h"



ShowThreadController::ShowThreadController(QObject *parent)
	: QObject(parent), m_ListView(NULL), m_Datas(new QList<PostDetailItem*>) {
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
}



void ShowThreadController::parse(const QString &page) {

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

	updateView();

	emit complete();
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

	m_Datas->push_back(new PostDetailItem());

	if(avatar.isEmpty())
		m_Datas->last()->setAvatar("asset:///images/default_avatar.png");
	else
		m_Datas->last()->setAvatar(avatar);

	// parse the post so it can be rendered in HTML within a listitem
	cleanupPost(postContent);

	m_Datas->last()->setAuthor(author);
	m_Datas->last()->setTimestamp(timestamp);
	m_Datas->last()->setPost(postContent);

}

void ShowThreadController::cleanupPost(QString &post) {

//	post = "<p><strong>tototot</strong>fdhjfbsd</p>";
//	return;
//	post = post.mid(0,184);

	QString cleanPost;
	QRegExp quoteRegexp(QString( "</p><div class=\"container\"><table class=\"citation\"><tr class=\"none\"><td><b class=\"s1\"><a href=\".+t([0-9]+)\" class=\"Topic\">")
								+"(.+)"														// author
								+"</a></b><br /><br /><p>(.+)</p></td></tr></table></div><p>"		// message
			);

	quoteRegexp.setCaseSensitivity(Qt::CaseSensitive);
	quoteRegexp.setMinimal(true);

	int lastPos = 0;
	int pos = 0;
	while((pos = quoteRegexp.indexIn(post, pos)) != -1) {
		cleanPost += "<p>" + post.mid(lastPos, pos-lastPos) + "</p>";

		cleanPost += "<table class=\"citation\" post=\"" + quoteRegexp.cap(1) + "\"><tr><th>" + quoteRegexp.cap(2) + "</th></tr><tr><td>" + quoteRegexp.cap(3) + "</td></tr></table>";
		//qDebug() << quoteRegexp.cap(1) << quoteRegexp.cap(2) << quoteRegexp.cap(3);

		pos += quoteRegexp.matchedLength();
		//qDebug() << quoteRegexp.matchedLength();
		lastPos = pos;
	}
	cleanPost += "<p>" + post.mid(lastPos, post.length()-lastPos) + "</p>";

	post = cleanPost;
	qDebug() << post;


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
				);
		m_ListView->setDataModel(dataModel);
	}

	// ----------------------------------------------------------------------------------------------
	// push data to the view

	QList<QObject*> datas;
	for(int i = m_Datas->length()-1 ; i >= 0 ; --i) {
		datas.push_back(m_Datas->at(i));
	}

	dataModel->insertList(datas);

}

