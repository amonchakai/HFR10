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
#include  "DataObjects.h"


ListFavoriteController::ListFavoriteController(QObject *parent)
	: QObject(parent), m_ListView(NULL), m_Datas(new QList<ThreadListItem*>()) {

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

	m_Datas->clear();

	QRegExp andAmp("&amp;");
	QRegExp quote("&#034;");
	QRegExp euro("&euro;");
	QRegExp inf("&lt;");
	QRegExp sup("&gt;");

	// ----------------------------------------------------------------------------------------------
	// Parse categories using regexp

	// get categories name & position into the stream
	QRegExp regexp("<th class=\"padding\".*class=\"cHeader\"?>(.+)</a></th>"); // match categories' name
	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


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
	regexp = QRegExp(QString("<td.*class=\"sujetCase3\"?.*class=\"cCatTopic\".*>(.+)</a></td>"));  	// topics' name



	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


	QString today = QDateTime::currentDateTime().toString("dd-MM-yyyy");


	pos = 0;
	int catIndex = 0;
	int lastPos = regexp.indexIn(page, pos);
	QString caption;
	QString category;

	if(lastPos != -1) {
		caption = regexp.cap(1);
		caption.replace(andAmp,"&");
		caption.replace(quote,"\"");
		caption.replace(euro, "e");
		caption.replace(inf, "<");
		caption.replace(sup, ">");

		lastPos += regexp.matchedLength();
		for( ; catIndex < indexCategories.length() && lastPos > indexCategories[catIndex] ; ++catIndex) {}
		category = categoriesLabels[catIndex-1];

	}

	while((pos = regexp.indexIn(page, lastPos)) != -1) {
		pos += regexp.matchedLength();

		// parse each post individually
		parseThreadListing(category, caption, page.mid(lastPos, pos-lastPos), today);

		catIndex = 0;
		for( ; catIndex < indexCategories.length() && pos > indexCategories[catIndex] ; ++catIndex) {}
		category = categoriesLabels[catIndex-1];

		lastPos = pos;
		caption = regexp.cap(1);
		caption.replace(andAmp,"&");
		caption.replace(quote,"\"");
		caption.replace(euro, "e");
		caption.replace(inf, "<");
		caption.replace(sup, ">");
	}
	parseThreadListing(category, caption, page.mid(lastPos, pos-lastPos), today);


	updateView();
	emit complete();

}

void ListFavoriteController::parseThreadListing(const QString &category, const QString &caption, const QString &threadListing, const QString &today) {
	//						   + ".*<td class=\"sujetCase4\"><a href=\"(.+)\" class=\"cCatTopic\">"		// link to first post
	//						   + "([0-9]+)</a></td>"													// overall number of pages
	//						   + ".*<td class=\"sujetCase5\"><a href=\"(.+)\"><img src"					// index to last read post
	//						   + ".*p.([0-9]+)"										// last page read number
	//						   + ".*<td class=\"sujetCase9.*class=\"Tableau\">(.+)" 					// time stamp
	//						   + "<br /><b>(.+)</b></a></td><td class=\"sujetCase10\"><input type");	// last contributor

	QRegExp andAmp("&amp;");


	ThreadListItem *item = new ThreadListItem();
	item->setCategory(category);
	item->setTitle(caption);

	QRegExp firstPostAndPage(QString("<td class=\"sujetCase4\"><a href=\"(.+)\" class=\"cCatTopic\">")		// link to first post
								 + "([0-9]+)</a></td>");													// overall number of pages

	firstPostAndPage.setCaseSensitivity(Qt::CaseSensitive);
	firstPostAndPage.setMinimal(true);

	if(firstPostAndPage.indexIn(threadListing, 0) != -1) {
		QString s = firstPostAndPage.cap(1); s.replace(andAmp, "&");
		item->setUrlFirstPage(s);

		item->setPages(firstPostAndPage.cap(2));
	}



	QRegExp lastReadAndPage(QString("<td class=\"sujetCase5\"><a href=\"(.+)\"><img src.*p.([0-9]+).*\" alt=\"flag\"")); // index to last read post + last page read number
	lastReadAndPage.setCaseSensitivity(Qt::CaseSensitive);
	lastReadAndPage.setMinimal(true);

	if(lastReadAndPage.indexIn(threadListing, 0) != -1) {
		qDebug() << lastReadAndPage.cap(2) << lastReadAndPage.cap(1);
		QString s = lastReadAndPage.cap(1); s.replace(andAmp, "&");
		item->setUrlLastPage(s);

		item->setPages(lastReadAndPage.cap(2)  + "/" + item->getPages());
	}


	QRegExp lastContribution("<td class=\"sujetCase9.*class=\"Tableau\">(.+)<br /><b>(.+)</b></a></td><td class=\"sujetCase10\"><input type"); // timestamp + last contributor
	lastContribution.setCaseSensitivity(Qt::CaseSensitive);
	lastContribution.setMinimal(true);

	if(lastContribution.indexIn(threadListing, 0) != -1) {
		QString s = lastContribution.cap(1);
		if(s.mid(0,10).compare(today) == 0)
			item->setTimestamp(s.mid(23,5));
		else
			item->setTimestamp(s.mid(0,10));


		item->setLastAuthor(lastContribution.cap(2));
	}

	m_Datas->append(item);

}


void ListFavoriteController::updateView() {
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
				QStringList() << "title"
							  << "timestamp"
							  << "lastAuthor"
							  << "urlFirstPage"
							  << "urlLastPage"
							  << "pages"
							  << "flagType"
							  << "read"
				);
		m_ListView->setDataModel(dataModel);
	}
	dataModel->setGrouping(ItemGrouping::ByFullValue);

	// ----------------------------------------------------------------------------------------------
	// push data to the view

	QList<QObject*> datas;
	for(int i = m_Datas->length()-1 ; i >= 0 ; --i) {
		datas.push_back(m_Datas->at(i));
	}

	dataModel->clear();
	dataModel->insertList(datas);
}



