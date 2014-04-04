#include "PrivateMessageController.hpp"


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



PrivateMessageController::PrivateMessageController(QObject *parent)
	: QObject(parent), m_ListView(NULL), m_Datas(new QList<PrivateMessageListItem*>()) {

}



void PrivateMessageController::getMessages() {

	// list green + yellow flags
	const QUrl url(DefineConsts::FORUM_URL + "/forum1.php?config=hfr.inc&cat=prive&page=1&subcat=&sondage=0&owntopic=0&trash=0&trash_post=0&moderation=0&new=0&nojs=0&subcatgroup=0");

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

}



void PrivateMessageController::checkReply() {
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



void PrivateMessageController::parse(const QString &page) {

	for(int i = 0 ; i < m_Datas->length() ; ++i)
		(*m_Datas)[i]->deleteLater();
	m_Datas->clear();

	QRegExp andAmp("&amp;");
	QRegExp quote("&#034;");
	QRegExp euro("&euro;");
	QRegExp inf("&lt;");
	QRegExp sup("&gt;");

	qDebug() << "start parser";


	// ----------------------------------------------------------------------------------------------
	// Parse unread MP using regexp

	QRegExp regexp("<td class=\"sujetCase1 cBackCouleurTab[0-9] \"><img src=\".*\" title=\".*\" alt=\"(Off|On)\" /></td>"); // topic read?

	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


	int pos = 0;
	int lastPos = regexp.indexIn(page, pos);
	bool read = regexp.cap(1).compare("On") == 0;

	while((pos = regexp.indexIn(page, lastPos)) != -1) {
		pos += regexp.matchedLength();
		// parse each post individually
		parseMessageListing(read, page.mid(lastPos, pos-lastPos));


		read = regexp.cap(1).compare("On") == 0;
		lastPos = pos;
	}
	parseMessageListing(read, page.mid(lastPos, pos-lastPos));

	qDebug() << "end parser";

	updateView();
	emit complete();


}

void PrivateMessageController::parseMessageListing(bool read, const QString &threadListing) {
	PrivateMessageListItem *item = new PrivateMessageListItem();

	QRegExp andAmp("&amp;");
	QRegExp quote("&#034;");
	QRegExp euro("&euro;");
	QRegExp inf("&lt;");
	QRegExp sup("&gt;");
	QRegExp nbsp("&nbsp;");

	item->setRead(read);

	QRegExp addresseeRegexp("<td class=\"sujetCase6 cBackCouleurTab[0-9] \"><a rel=\"nofollow\" href=\".*\" class=\"Tableau\">(.+)</a></td>");
	addresseeRegexp.setCaseSensitivity(Qt::CaseSensitive);
	addresseeRegexp.setMinimal(true);

	if(addresseeRegexp.indexIn(threadListing, 0) != -1) {
		item->setAddressee(addresseeRegexp.cap(1));
	} else {
		item->deleteLater();
		return;
	}

	QRegExp addresseeRead("<span class=\"red\" title");
	addresseeRead.setCaseSensitivity(Qt::CaseSensitive);
	addresseeRead.setMinimal(true);


	if(addresseeRead.indexIn(threadListing, 0) != -1)
		item->setAddresseeRead(false);
	else
		item->setAddresseeRead(true);


	QRegExp mpTitle("class=\"sujetCase3\".*<a href=\"(.+)\" class=\"cCatTopic\" title=\"Sujet n.[0-9]+\">(.+)</a></td><td class=\"sujetCase4\">");
	mpTitle.setCaseSensitivity(Qt::CaseSensitive);
	mpTitle.setMinimal(true);


	if(mpTitle.indexIn(threadListing, 0) != -1) {
		QString s = mpTitle.cap(2);
		s.replace(andAmp, "&");
		s.replace(quote,"\"");
		s.replace(euro, "e");
		s.replace(inf, "<");
		s.replace(sup, ">");

		item->setTitle(s);

		s = mpTitle.cap(1);
		s.replace(andAmp, "&");
		item->setUrlFirstPage(s);
	}


	QRegExp lastPostInfo("<td class=\"sujetCase9 cBackCouleurTab[0-9] \"><a href=\"(.+)\" class=\"Tableau\">(.+)<br /><b>(.+)</b></a></td><td class=\"sujetCase10\">");
	lastPostInfo.setCaseSensitivity(Qt::CaseSensitive);
	lastPostInfo.setMinimal(true);

	if(lastPostInfo.indexIn(threadListing, 0) != -1) {
		QString s = lastPostInfo.cap(1);
		s.replace(andAmp, "&");
		item->setUrlLastPage(s);

		s = lastPostInfo.cap(2);
		s.replace(nbsp, " ");
		item->setTimestamp(s);

		item->setLastAuthor(lastPostInfo.cap(3));
	}

	m_Datas->append(item);

}


void PrivateMessageController::updateView() {
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


