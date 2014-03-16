#include "ListFavoriteController.hpp"


#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>
#include <QtXml/QDomDocument>

#include  "Globals.h"
#include  "HFRNetworkAccessManager.hpp"

ListFavoriteController::ListFavoriteController(QObject *parent)
	: QObject(parent) {

}


void ListFavoriteController::getFavorite() {

	// list green + yellow flags
	const QUrl url(DefineConsts::FORUM_URL + "/forum1f.php?owntopic=1");

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

}



void ListFavoriteController::checkReply() {
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



void ListFavoriteController::parse(const QString &page) {

	qDebug() << "start parser";

	// ----------------------------------------------------------------------------------------------
	// Parse categories using regexp

	// get categories name & position into the stream
	QRegExp regexp("<th class=\"padding\".*class=\"cHeader\"?>(.+)</a></th>"); // match categories' name
	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);

	QList<int> indexCategories;

	int pos = 0;
	while((pos = regexp.indexIn(page, pos)) != -1) {
		pos += regexp.matchedLength();
		indexCategories.push_back(pos);					// Store position of each category into the stream
	}


	// Get favorite topics
	//QRegExp regexp(QString("<td.*(class=\"sujetCase3\")?.*class=\"cCatTopic\".*title=\"Sujet n°([0-9]+)\">(.+)?</a></td>"));
	regexp = QRegExp("<td.*class=\"sujetCase3\"?.*class=\"cCatTopic\".*>(.+)</a></td>"); // match topics' name
	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


	pos = 0;
	while((pos = regexp.indexIn(page, pos)) != -1) {
		pos += regexp.matchedLength();
		int catIndex = 0;
		for( ; catIndex < indexCategories.length() && pos > indexCategories[catIndex] ; ++catIndex) {}	// use the position of the category into the stream to find the category's index

		qDebug() << regexp.cap(1) << " Cat Index: " << catIndex << " pos in file " << pos;
	}


}



