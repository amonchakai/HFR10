/*
 * ExploreCategoryController.cpp
 *
 *  Created on: 27 mars 2014
 *      Author: PierreL
 */

#include "ExploreCategoryController.hpp"

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


ExploreCategoryController::ExploreCategoryController(QObject *parent)
	: QObject(parent), m_ListView(NULL), m_Datas(new QList<ThreadListItem*>()) {

}


void ExploreCategoryController::listTopics(const QString &url_string) {
	m_Url = url_string;
	showTopicList(url_string);
}

void ExploreCategoryController::showTopicList(const QString &url_string) {

	// list green + yellow flags
	const QUrl url(url_string);
	m_LastLoadedUrl = url_string;

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

}

void ExploreCategoryController::refresh() {
	showTopicList(m_LastLoadedUrl);
}


void ExploreCategoryController::checkReply() {
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



void ExploreCategoryController::parse(const QString &page) {

	m_Datas->clear();

	QRegExp andAmp("&amp;");
	QRegExp quote("&#034;");
	QRegExp euro("&euro;");
	QRegExp inf("&lt;");
	QRegExp sup("&gt;");


	// ----------------------------------------------------------------------------------------------
	// drap filter

	// The URL for filtering topic by flags is highly similar:
	// "/forum1.php?config=hfr.inc&cat=25&page=1&subcat=0&sondage=0&owntopic=1&trash=0&trash_post=0&moderation=0&new=0&nojs=0&subcatgroup=0" Blue
	// "/forum1.php?config=hfr.inc&cat=25&page=1&subcat=0&sondage=0&owntopic=2&trash=0&trash_post=0&moderation=0&new=0&nojs=0&subcatgroup=0" Red
	// "/forum1.php?config=hfr.inc&cat=25&page=1&subcat=0&sondage=0&owntopic=3&trash=0&trash_post=0&moderation=0&new=0&nojs=0&subcatgroup=0" Star

	// Just "owntopic" changes...
	// => retrieve one URL (using either blue or red flag), then we will be able to tune the URL depending on our needs.

	QRegExp filterDrapRegexp("<a accesskey=\"b\" href=\"(.+)\" class=\"onglet\" id=\"onglet3\"");
	filterDrapRegexp.setCaseSensitivity(Qt::CaseSensitive);
	filterDrapRegexp.setMinimal(true);

	if(filterDrapRegexp.indexIn(page, 0) != -1) {		// can we catch the URL from blue flag
		m_GeneralUrl = filterDrapRegexp.cap(1);
		m_GeneralUrl.replace(andAmp, "&");
	} else {											// if not, it means we were on the blue flag, then red one must be available.

		filterDrapRegexp = QRegExp("<a accesskey=\"r\" href=\"(.+)\" class=\"onglet\" id=\"onglet4\"");
		filterDrapRegexp.setCaseSensitivity(Qt::CaseSensitive);
		filterDrapRegexp.setMinimal(true);

		if(filterDrapRegexp.indexIn(page, 0) != -1) {
			m_GeneralUrl = filterDrapRegexp.cap(1);
			m_GeneralUrl.replace(andAmp, "&");
		}
	}

	// ----------------------------------------------------------------------------------------------
	// Parse categories using regexp

	// Get favorite topics
	QRegExp regexp(QString("<td class=\"sujetCase1 cBackCouleurTab[0-9] \"><img src=\".*\" title=\".*\" alt=\"(Off|On)\" /></td>.*<a href=\".*\" class=\"cCatTopic\" title=\"Sujet n.[0-9]+\">(.+)</a></td>"));  	// topics' name


	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


	QString today = QDateTime::currentDateTime().toString("dd-MM-yyyy");

	int pos = 0;
	int lastPos = regexp.indexIn(page, pos);
	QString caption;

	if(lastPos != -1) {
		caption = regexp.cap(2);
		caption.replace(andAmp,"&");
		caption.replace(quote,"\"");
		caption.replace(euro, "e");
		caption.replace(inf, "<");
		caption.replace(sup, ">");
	}

	while((pos = regexp.indexIn(page, lastPos)) != -1) {
		pos += regexp.matchedLength();

		// parse each post individually
		parseThreadListing(caption, regexp.cap(1).compare("Off") == 0, page.mid(lastPos, pos-lastPos));

		lastPos = pos;
		caption = regexp.cap(2);
		caption.replace(andAmp,"&");
		caption.replace(quote,"\"");
		caption.replace(euro, "e");
		caption.replace(inf, "<");
		caption.replace(sup, ">");
	}
	parseThreadListing(caption, regexp.cap(1).compare("Off") == 0, page.mid(lastPos, pos-lastPos));


	updateView();
	emit complete();

}


void ExploreCategoryController::parseThreadListing(const QString &caption, bool read, const QString &threadListing) {
	ThreadListItem *item = new ThreadListItem();
	QRegExp andAmp("&amp;");
	QRegExp nbsp("&nbsp;");

	item->setTitle(caption);
	item->setRead(read);

	QRegExp firstPostUrlRegexp("<td class=\"sujetCase4\">.*<a href=\"(.+)\" class=\"cCatTopic\">([0-9]+)</a></td>");
	firstPostUrlRegexp.setCaseSensitivity(Qt::CaseSensitive);
	firstPostUrlRegexp.setMinimal(true);

	if(firstPostUrlRegexp.indexIn(threadListing, 0) != -1) {
		QString s = firstPostUrlRegexp.cap(1);
		s.replace(andAmp, "&");
		item->setUrlFirstPage(s);

		item->setPages(firstPostUrlRegexp.cap(2));
	} else {
		item->setPages("1");
	}

	QRegExp lastReadPost("<td class=\"sujetCase9 cBackCouleurTab[0-9] \"><a href=\"(.*)\" class=\"Tableau\">(.*)<br /><b>(.*)</b>");
	lastReadPost.setCaseSensitivity(Qt::CaseSensitive);
	lastReadPost.setMinimal(true);

	if(lastReadPost.indexIn(threadListing, 0) != -1) {
		QString s = lastReadPost.cap(2);
		s.replace(nbsp, " ");
		item->setTimestamp(s);

		s = lastReadPost.cap(1);
		s.replace(andAmp, "&");
		item->setUrlLastPage(s);

		item->setLastAuthor(lastReadPost.cap(3));
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
}


void ExploreCategoryController::updateView() {

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

void ExploreCategoryController::filterByFlag(int flag) {
	QRegExp locateFlagInURL("owntopic=([0-3])");

	int pos = locateFlagInURL.indexIn(m_GeneralUrl, 0);
	if(pos != -1) {
		switch(flag) {
			case Flag::NONE:
				showTopicList(m_Url);
				return;

			case Flag::PARTICIPATE:
				m_GeneralUrl[pos+9] = '1';
				break;

			case Flag::READ:
				m_GeneralUrl[pos+9] = '2';
				break;

			case Flag::FAVORITE:
				m_GeneralUrl[pos+9] = '3';
				break;
		}

		showTopicList(DefineConsts::FORUM_URL + m_GeneralUrl);


	}
}


void ExploreCategoryController::firstPage() {
	QRegExp locatePage("page=([0-9]+)");
	int pos = locatePage.indexIn(m_GeneralUrl, 0);
	if(pos != -1) {
		int length = pos+locatePage.matchedLength();

		m_GeneralUrl = m_GeneralUrl.mid(0,pos) + "page=1" + m_GeneralUrl.mid(length,m_GeneralUrl.length()-length);

		showTopicList(DefineConsts::FORUM_URL + m_GeneralUrl);

	}
}


void ExploreCategoryController::nextPage() {
	QRegExp locatePage("page=([0-9]+)");
	int pos = locatePage.indexIn(m_GeneralUrl, 0);
	if(pos != -1) {
		int length = pos+locatePage.matchedLength();

		m_GeneralUrl = m_GeneralUrl.mid(0,pos) + "page=" + QString::number(locatePage.cap(1).toInt()+1) + m_GeneralUrl.mid(length,m_GeneralUrl.length()-length);

		showTopicList(DefineConsts::FORUM_URL + m_GeneralUrl);
	}
}

void ExploreCategoryController::prevPage() {
	QRegExp locatePage("page=([0-9]+)");
	int pos = locatePage.indexIn(m_GeneralUrl, 0);
	if(pos != -1) {
		int length = pos+locatePage.matchedLength();

		m_GeneralUrl = m_GeneralUrl.mid(0,pos) + "page=" + QString::number(std::max(locatePage.cap(1).toInt()-1,1)) + m_GeneralUrl.mid(length,m_GeneralUrl.length()-length);

		showTopicList(DefineConsts::FORUM_URL + m_GeneralUrl);
	}
}

