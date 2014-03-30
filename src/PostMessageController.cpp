/*
 * PostMessageController.cpp
 *
 *  Created on: 30 mars 2014
 *      Author: PierreL
 */

#include "PostMessageController.hpp"


#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "Globals.h"
#include "HFRNetworkAccessManager.hpp"

void PostMessageController::postMessage(const QString &hashCheck,
										const QString &postID,
										const QString &catID,
										const QString &page,
										const QString &pseudo,
										const QString &message,
										const QString &threadTitle,
										bool signature) {

	const QUrl url(DefineConsts::FORUM_URL + "/bddpost.php?config=hfr.inc");


	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");




	QUrl params;
	params.addQueryItem("hash_check", hashCheck);
	params.addQueryItem("post", postID);
	params.addQueryItem("cat", catID);
	params.addQueryItem("verifrequet", "1100");
	params.addQueryItem("MsgIcon", "20");
	params.addQueryItem("page", page);
	params.addQueryItem("pseudo", pseudo);
	params.addQueryItem("content_form", message);
	params.addQueryItem("sujet", threadTitle);
	params.addQueryItem("signature", signature ? "1" : "0");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}


void PostMessageController::checkReply() {
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
			qDebug() << response;
		}

	    reply->deleteLater();
	}

	if (response.trimmed().isEmpty()) {
        response = tr("Post failed");
        qDebug() << response;
    }

	emit complete();
}


