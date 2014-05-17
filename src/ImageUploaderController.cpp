/*
 * ImageUploaderController.cpp
 *
 *  Created on: 1 mai 2014
 *      Author: pierre
 */

#include "ImageUploaderController.hpp"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QDir>

#include "Globals.h"
#include "Network/CookieJar.hpp"
#include "Network/HFRNetworkAccessManager.hpp"

#include <bb/system/SystemToast>


ImageUploaderController::ImageUploaderController(QObject *parent)
	: QObject(parent), m_uploading(false), m_ProgressIndicator(NULL) {

}

void ImageUploaderController::upload(const QString &image) {
//	m_User = login;

	if(m_uploading)
		return;

	QUrl url(DefineConsts::REHOST_URL + "/upload");

	QFile file(image);
	if (!file.open(QIODevice::ReadOnly))
	    return;


	QString boundary = "---------------------------193971182219750";
	QByteArray datas(QString("--" + boundary + "\r\n").toAscii());
	datas += "Content-Disposition: form-data; name=\"fichier\"; filename=\"" + file.fileName() + "\"\r\n";
	datas += "Content-Type: image/jpeg\r\n\r\n";

	datas += file.readAll();
	datas += "\r\n";
	datas += QString("--" + boundary + "\r\n").toAscii();
	datas += "Content-Disposition: form-data; name=\"submit\"\r\n\r\n";
	datas += "Envoyer\r\n";
	datas += QString("--" + boundary + "--\r\n").toAscii();

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, tr("multipart/form-data; boundary=") + boundary);
	file.close();


	m_uploading = true;

	QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, datas);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(uploading(qint64, qint64)));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}

void ImageUploaderController::uploading(qint64 stage, qint64 end) {
	if(m_ProgressIndicator == NULL)
		return;

	m_ProgressIndicator->setValue(100.f * static_cast<float>(stage)/static_cast<float>(end));
}


void ImageUploaderController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				parse(response);

				emit complete();
			}
		} else {

			connectionTimedOut();
		}

	    reply->deleteLater();
	}

	m_uploading = false;
}


void ImageUploaderController::connectionTimedOut() {
	bb::system::SystemToast *toast = new bb::system::SystemToast(this);

	toast->setBody(tr("Connection timed out"));
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();
}


void ImageUploaderController::parse(const QString &page) {

	QRegExp imageUrl("<code>([^\"]+)</code>");
	imageUrl.setMinimal(true);
	imageUrl.setCaseSensitivity(Qt::CaseSensitive);

	int pos = 0;

	if((pos = imageUrl.indexIn(page, pos)) != -1) {
		m_Original = imageUrl.cap(1);
		pos += imageUrl.matchedLength();
	}

	if((pos = imageUrl.indexIn(page, pos)) != -1) {
		m_Preview = imageUrl.cap(1);
		pos += imageUrl.matchedLength();
	}

	if((pos = imageUrl.indexIn(page, pos)) != -1) {
		m_Mini = imageUrl.cap(1);
		pos += imageUrl.matchedLength();
	}

}


