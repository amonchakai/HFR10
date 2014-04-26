#include "LoginController.hpp"


#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QDir>

#include "Globals.h"
#include "CookieJar.hpp"
#include "HFRNetworkAccessManager.hpp"

#include <bb/system/SystemToast>

QString LoginController::m_User = "";

LoginController::LoginController(QObject *parent)
	: QObject(parent) {

	loadUserName();

}

void LoginController::login(const QString &login, const QString &password) {
	m_User = login;

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

				// save cookies on disk
				CookieJar::get()->saveToDisk();
				saveUserName();
				emit complete();
			}
		} else {
			connectionTimedOut();
		}

	    reply->deleteLater();
	}

	if (response.trimmed().isEmpty()) {
		errorMessage();
    }
}

void LoginController::errorMessage() {
	bb::system::SystemToast *toast = new bb::system::SystemToast(this);

	toast->setBody(tr("Login failed, please check if your user name and password are correct"));
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();
}

void LoginController::connectionTimedOut() {
	bb::system::SystemToast *toast = new bb::system::SystemToast(this);

	toast->setBody(tr("Connection timed out"));
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();
}

bool LoginController::isLogged() {
	return CookieJar::get()->areThereCookies();
}

void LoginController::logOut() {
	CookieJar::get()->deleteCookies();

	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return;
	}

	QFile file(directory + "/UserID.txt");
	file.remove();
}


void LoginController::saveUserName() {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return;
	}

	QFile file(directory + "/UserID.txt");

	if (file.open(QIODevice::WriteOnly)) {
		QDataStream stream(&file);
		stream << m_User;

		file.close();
	}
}

void LoginController::loadUserName() {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return;
	}

	QFile file(directory + "/UserID.txt");

	if (file.open(QIODevice::ReadOnly)) {
		QDataStream stream(&file);
		stream >> m_User;

		file.close();
	}
}

void LoginController::clearImageCache() {
	QString directory = QDir::homePath() + "/Cache/";
	if (QFile::exists(directory)) {
		QDir dir(directory);
		dir.setNameFilters(QStringList() << "*.*");
		dir.setFilter(QDir::Files);
		foreach(QString dirFile, dir.entryList()) {
		    dir.remove(dirFile);
		}
	}
}
