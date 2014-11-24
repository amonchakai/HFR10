/*
 * SearchController.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: pierre
 */

#include "SearchController.hpp"


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
#include  "Network/HFRNetworkAccessManager.hpp"
#include  "DataObjects.h"



SearchController::SearchController(QObject *parent)
	: QObject(parent), m_ListView(NULL), m_Datas(new QList<ThreadListItem*>()) {
}


void SearchController::search(const QString &search,
							  const QString &author,
							  const QString &hashKey,
							  int searchType,
							  int categoryType,
							  int searchIn,
							  int numberOfMessages,
							  int sortBy) {

	const QUrl url(DefineConsts::FORUM_URL + "/search.php?config=hfr.inc");

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QUrl params;
	params.addQueryItem("hash_check", hashKey);
	params.addQueryItem("cat", QString::number(categoryType));
	params.addQueryItem("titre", QString::number(searchIn));
	params.addQueryItem("search", search);
	params.addQueryItem("searchtype", QString::number(searchType));
	params.addQueryItem("pseud", author);
	params.addQueryItem("resSearch", QString::number(numberOfMessages));
	params.addQueryItem("orderSearch", QString::number(sortBy));

	QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

}



void SearchController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				QRegExp loading("Votre recherche est en cours, merci de bien vouloir patienter");
				if(loading.indexIn(response, 0) != -1)
					waitMore(response);
				else
					parse(response);
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}

void SearchController::waitMore(const QString &page) {

	QRegExp nextUrl("<meta http-equiv=\"Refresh\" content=\"1; url=(.*)\" />");
	nextUrl.setMinimal(true);
	nextUrl.setCaseSensitivity(Qt::CaseSensitive);

	if(nextUrl.indexIn(page, 0) != -1) {
		QString s = nextUrl.cap(1);
		s.replace(QRegExp("&amp;"), "&");

		QTime dieTime= QTime::currentTime().addSecs(1);
		while( QTime::currentTime() < dieTime ) {}



		const QUrl url(DefineConsts::FORUM_URL + s);

		QNetworkRequest request(url);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

		QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
		bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
		Q_ASSERT(ok);
		Q_UNUSED(ok);

	}

}

void SearchController::parse(const QString &page) {

	for(int i = 0 ; i < m_Datas->length() ; ++i)
		(*m_Datas)[i]->deleteLater();
	m_Datas->clear();

	QRegExp andAmp("&amp;");
	QRegExp quote("&#034;");
	QRegExp euro("&euro;");
	QRegExp inf("&lt;");
	QRegExp sup("&gt;");

	// ----------------------------------------------------------------------------------------------
	// Parse categories using regexp

	// Get favorite topics
	QRegExp regexp(QString("<td class=\"sujetCase1 cBackCouleurTab[0-9] \"><img src=\".*\" title=\".*\" alt=\"(Off|On)\" /></td>.*<a href=\"(.+)\" class=\"cCatTopic\" title=\"Sujet n.[0-9]+\">(.+)</a></td>"));  	// topics' name


	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


	QString today = QDateTime::currentDateTime().toString("dd-MM-yyyy");

	int pos = 0;
	int lastPos = regexp.indexIn(page, pos);
	QString caption;
	QString urlFirstPage;
	bool read = regexp.cap(1).compare("On") == 0;

	if(lastPos != -1) {
		caption = regexp.cap(3);
		caption.replace(andAmp,"&");
		caption.replace(quote,"\"");
		caption.replace(euro, "e");
		caption.replace(inf, "<");
		caption.replace(sup, ">");

		urlFirstPage = regexp.cap(2);
		urlFirstPage.replace(andAmp,"&");

	}

	while((pos = regexp.indexIn(page, lastPos)) != -1) {
		pos += regexp.matchedLength();

		// parse each post individually
		parseThreadListing(caption, urlFirstPage, read, page.mid(lastPos, pos-lastPos));

		lastPos = pos;
		caption = regexp.cap(3);
		caption.replace(andAmp,"&");
		caption.replace(quote,"\"");
		caption.replace(euro, "e");
		caption.replace(inf, "<");
		caption.replace(sup, ">");

		urlFirstPage = regexp.cap(2);
		urlFirstPage.replace(andAmp,"&");

		read = regexp.cap(1).compare("On") == 0;
	}
	parseThreadListing(caption, urlFirstPage, read, page.mid(lastPos, pos-lastPos));


	updateView();
	emit complete();

}

void SearchController::parseThreadListing(const QString &caption, const QString &urlFirstPage, bool read, const QString &threadListing) {
	ThreadListItem *item = new ThreadListItem();
	QRegExp andAmp("&amp;");
	QRegExp nbsp("&nbsp;");


	{
		bool inf = false;
		bool slash = false;
		int idx = 0;
		for(int i = 0 ; i < caption.length() ; ++i) {
			if(caption[i] == '<') {
				inf = true;
				continue;
			}
			if(inf && caption[i] == '/') {
				slash = true;
				continue;
			} else {
				inf = false;
			}

			if(slash && caption[i] == 'a') {
				idx = i-2;
				break;
			}
		}

		if(idx != 0)
			item->setTitle(caption.mid(0,idx));
		else
			item->setTitle(caption);
	}

	item->setRead(read);
	item->setUrlFirstPage(urlFirstPage);

	QRegExp lastPostReadRegexp("</td><td class=\"sujetCase5\"><a href=\"(.*#t[0-9bas]+)\"><img src=");
	lastPostReadRegexp.setCaseSensitivity(Qt::CaseSensitive);
	lastPostReadRegexp.setMinimal(true);

	if(lastPostReadRegexp.indexIn(threadListing, 0) != -1) {
		QString s = lastPostReadRegexp.cap(1);
		s.replace(andAmp, "&");
		item->setUrlLastPostRead(s);
	}

	QRegExp pageNumberRegExp("<td class=\"sujetCase4\"><a href=\"(.*)\" class=\"cCatTopic\">([0-9]+)</a></td><td class=\"sujetCase5\">");
	pageNumberRegExp.setCaseSensitivity(Qt::CaseSensitive);
	pageNumberRegExp.setMinimal(true);

	if(pageNumberRegExp.indexIn(threadListing, 0) != -1) {
		item->setPages(pageNumberRegExp.cap(2));

		if(item->getUrlLastPostRead() == "") {
			QString s = pageNumberRegExp.cap(1);
			s.replace(andAmp, "&");
			item->setUrlLastPostRead(s+"#bas");
		}
	} else {
		item->setPages("1");
	}

	QRegExp lastPageRegExp("<td class=\"sujetCase9 cBackCouleurTab[0-9] \"><a href=\"(.*)\" class=\"Tableau\">(.*)<br /><b>(.*)</b>");
	lastPageRegExp.setCaseSensitivity(Qt::CaseSensitive);
	lastPageRegExp.setMinimal(true);

	if(lastPageRegExp.indexIn(threadListing, 0) != -1) {
		QString s = lastPageRegExp.cap(2);
		s.replace(nbsp, " ");
		item->setTimestamp(s);

		s = lastPageRegExp.cap(1);
		s.replace(andAmp, "&");
		item->setUrlLastPage(s);

		item->setLastAuthor(lastPageRegExp.cap(3));
	}

	QRegExp flagTypeRegexp("<img src=\"http://forum-images.hardware.fr/themes_static/images_forum/1/favoris.gif\"");
	if(flagTypeRegexp.indexIn(threadListing, 0) != -1)
		item->setFlagType(Flag::FAVORITE);

	flagTypeRegexp = QRegExp("<img src=\"http://forum-images.hardware.fr/themes_static/images_forum/1/flag([0-1]).gif\"");
	if(flagTypeRegexp.indexIn(threadListing, 0) != -1) {
		switch(flagTypeRegexp.cap(1).toInt()) {
			case 0:
				item->setFlagType(Flag::READ);
				break;

			case 1:
				item->setFlagType(Flag::PARTICIPATE);
				break;
		}
	}


	if(!item->getLastAuthor().isEmpty())
		m_Datas->append(item);
	else
		item->deleteLater();

}


void SearchController::updateView() {
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
							  << "addressee"
							  << "timestamp"
							  << "lastAuthor"
							  << "urlFirstPage"
							  << "urlLastPage"
							  << "read"
							  << "addresseeRead"
				);
		m_ListView->setDataModel(dataModel);
	}

	// ----------------------------------------------------------------------------------------------
	// push data to the view

	QList<QObject*> datas;
	for(int i = m_Datas->length()-1 ; i >= 0 ; --i) {
		datas.push_back(m_Datas->at(i));
	}

	dataModel->clear();
	dataModel->insertList(datas);
}




