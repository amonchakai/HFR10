#include "LoginController.hpp"


#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "Globals.h"
#include "CookieJar.hpp"
#include "HFRNetworkAccessManager.hpp"

LoginController::LoginController(QObject *parent)
	: QObject(parent) {

}

void LoginController::login(const QString &login, const QString &password) {
	const QUrl url(DefineConsts::HARDWARE_FR_URL + "/membres/popupLogin.php");


	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QUrl params;
	params.addQueryItem("pseudo", login);
	params.addQueryItem("pwd", password);
	params.addQueryItem("action", "send");
	params.addQueryItem("login", "Se connecter");

	QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}



void LoginController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);

				qDebug() << "Login OK";

				// save cookies on disk
				CookieJar::get()->saveToDisk();
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

	    reply->deleteLater();
	}

	if (response.trimmed().isEmpty()) {
        response = tr("Login failed");
        qDebug() << response;
    }

	emit complete(response);
}




