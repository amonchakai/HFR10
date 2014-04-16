/*
 * SearchKeyRetriever.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: pierre
 */

#include "SearchKeyRetriever.hpp"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>

#include "Globals.h"
#include "HFRNetworkAccessManager.hpp"


void SearchKeyRetriever::getKey() {
	const QUrl url(DefineConsts::FORUM_URL + "/search.php?config=hardwarefr.inc&cat=&subcat=0");

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}

void SearchKeyRetriever::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				QRegExp hashKeyRegexp("<input type=\"hidden\" name=\"hash_check\" value=\"([a-zA-Z0-9]+)\" />");

				if(hashKeyRegexp.indexIn(response, 0) != -1) {
					m_HashKey = hashKeyRegexp.cap(1);
					emit hashKeyChanged();
				} else
					qWarning() << "[FAILED to get the hashkey!!]";
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}
