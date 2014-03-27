#include "ListFavoriteController.hpp"


#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>
#include <QDateTime>

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>

#include  "Globals.h"
#include  "HFRNetworkAccessManager.hpp"



ListFavoriteController::ListFavoriteController(QObject *parent)
	: QObject(parent), m_ListView(NULL) {

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



void ListFavoriteController::parse(const QString &page) {


	// ----------------------------------------------------------------------------------------------
	// get the dataModel of the listview if not already available

	if(m_ListView == NULL) {
		qWarning() << "the list view is either not provided or not a listview...";
		return;
	}

	using namespace bb::cascades;

	GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
	if (dataModel) {
		dataModel->clear();
	} else {
		dataModel = new GroupDataModel(
						QStringList() << "category"
									  << "caption"
									  << "timestamp"
									  << "lastAuthor"
									  << "urlFirstPost"
									  << "indexLastPost"
									  << "pages"
					);
		m_ListView->setDataModel(dataModel);
	}
	dataModel->setGrouping(ItemGrouping::ByFullValue);


	// ----------------------------------------------------------------------------------------------
	// Parse categories using regexp

	// get categories name & position into the stream
	QRegExp regexp("<th class=\"padding\".*class=\"cHeader\"?>(.+)</a></th>"); // match categories' name
	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);

	QRegExp andAmp("&amp;");

	QList<int> indexCategories;
	QList<QString> categoriesLabels;

	int pos = 0;
	while((pos = regexp.indexIn(page, pos)) != -1) {
		pos += regexp.matchedLength();
		indexCategories.push_back(pos);					// Store position of each category into the stream

		QString s(regexp.cap(1)); s.replace(andAmp, "&");
		categoriesLabels.push_back(s);		// Store the matching label
	}


	// Get favorite topics
	regexp = QRegExp(QString("<td.*class=\"sujetCase3\"?.*class=\"cCatTopic\".*>(.+)</a></td>")  	// topics' name
						   + ".*<td class=\"sujetCase4\"><a href=\"(.+)\" class=\"cCatTopic\">"		// link to first post
						   + "([0-9]+)</a></td>"													// overall number of pages
						   + ".*<td class=\"sujetCase5\"><a href=\"(.+)\"><img src"					// index to last read post
						   + ".*p.([0-9]+)"										// last page read number
						   + ".*<td class=\"sujetCase9.*class=\"Tableau\">(.+)" 					// time stamp
						   + "<br /><b>(.+)</b></a></td><td class=\"sujetCase10\"><input type");	// last contributor

	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


	QString today = QDateTime::currentDateTime().toString("dd-MM-yyyy");
	pos = 0;
	while((pos = regexp.indexIn(page, pos)) != -1) {
		pos += regexp.matchedLength();
		int catIndex = 0;
		for( ; catIndex < indexCategories.length() && pos > indexCategories[catIndex] ; ++catIndex) {}	// use the position of the category into the stream to find the category's index

		//qDebug() << regexp.cap(1) << " Cat Index: " << catIndex << " last post idx " << regexp.cap(3) << " time: " << regexp.cap(4) << " author: " << regexp.cap(5);

		QString s = regexp.cap(1);
		s.replace(andAmp, "&");			// replace "&amp;"  by  "&"

		QVariantMap topic;
		topic["category"] = categoriesLabels[catIndex-1];
		topic["caption"] = s;

		s = regexp.cap(2); s.replace(andAmp, "&");  				// replace "&amp;"  by  "&"
		topic["urlFirstPost"]  = s;
		topic["pages"] = regexp.cap(5) + " / " + regexp.cap(3);

		s = regexp.cap(4);
		s.replace(andAmp, "&");
		topic["indexLastPost"] = s;



		s = regexp.cap(6);
		if(s.mid(0,10).compare(today) == 0)
			topic["timestamp"] = s.mid(23,5);
		else
			topic["timestamp"] = s.mid(0,10);


		topic["lastAuthor"] = regexp.cap(7);


		dataModel->insert(topic);

	}

	emit complete();

}



