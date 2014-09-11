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
#include <bb/system/SystemToast>

#include  "Globals.h"
#include  "Network/HFRNetworkAccessManager.hpp"
#include  "DataObjects.h"



PrivateMessageController::PrivateMessageController(QObject *parent)
	: QObject(parent), m_ListView(NULL), m_Datas(new QList<PrivateMessageListItem*>()) {

}



void PrivateMessageController::getMessages() {

	// list green + yellow flags
	const QUrl url(DefineConsts::FORUM_URL + "/forum1.php?config=hfr.inc&cat=prive&owntopic=0");

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
				checkErrorMessage(response);
				parse(response);
				save();
			}
		} else {
			connectionTimedOut();
		}

		reply->deleteLater();
	}
}

void PrivateMessageController::checkErrorMessage(const QString &page) {

	QRegExp message("vous ne faites pas partie des membres ayant");
	message.setCaseSensitivity(Qt::CaseSensitive);
	message.setMinimal(true);

	QString error;

	if(message.indexIn(page, 0) != -1) {
		error = QString(tr("You are not logged in."));
	}

	if(error == "")
		return;

	bb::system::SystemToast *toast = new bb::system::SystemToast(this);

	toast->setBody(error);
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();
}

void PrivateMessageController::connectionTimedOut() {

	bb::system::SystemToast *toast = new bb::system::SystemToast(this);

	toast->setBody(tr("Connection timed out"));
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();

	emit complete();
}



void PrivateMessageController::deletePrivateMessage(const QString &urlFirstPage) {

    QRegExp catIDRegExp("cat=([0-9prive]+)");
    if(catIDRegExp.indexIn(urlFirstPage) == -1)
        return;

    const QUrl url(DefineConsts::FORUM_URL + "/modo/manageaction.php?config=hfr.inc&cat=" + catIDRegExp.cap(1) + "&type_page=forum1&moderation=0");

    QRegExp postIDRegExp("post=([0-9]+)");
    if(postIDRegExp.indexIn(urlFirstPage) == -1)
        return;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl params;
    params.addQueryItem("action_reaction", "valid_eff_prive");
    params.addQueryItem("topic1", postIDRegExp.cap(1));
    params.addQueryItem("hash_check", m_HashCheck);

    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkMessageDeleted()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void PrivateMessageController::checkMessageDeleted() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);
                //qDebug() << response;
            }
        } else {
            connectionTimedOut();
        }

        reply->deleteLater();
    }
}


void PrivateMessageController::parse(const QString &page) {

	//for(int i = 0 ; i < m_Datas->length() ; ++i)
	//	(*m_Datas)[i]->deleteLater();
	m_Datas->clear();

	QRegExp andAmp("&amp;");
	QRegExp quote("&#034;");
	QRegExp euro("&euro;");
	QRegExp inf("&lt;");
	QRegExp sup("&gt;");


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

	QRegExp hashCheckRegExp("name=\"hash_check\" value=\"([0-9a-zA-Z]+)\"");
	hashCheckRegExp.setCaseSensitivity(Qt::CaseSensitive);
	if(hashCheckRegExp.indexIn(page.mid(lastPos)) != -1) {
	    m_HashCheck = hashCheckRegExp.cap(1);
	}

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

	//qDebug() << threadListing;

	QRegExp addresseeRegexp("<td class=\"sujetCase6 cBackCouleurTab[0-9] \"><a rel=\"nofollow\" href=\"[^\"]+pseudo=([^\"]+)\" class=\"Tableau\">([^\"]+)</a></td>");
	addresseeRegexp.setCaseSensitivity(Qt::CaseSensitive);
	//addresseeRegexp.setMinimal(true);

	if(addresseeRegexp.indexIn(threadListing, 0) != -1) {
		item->setAddressee(addresseeRegexp.cap(1));
		//qDebug() << addresseeRegexp.cap(1) << addresseeRegexp.cap(2) << addresseeRegexp.cap(0) ;
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
		//qDebug() << "create new model";
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



void PrivateMessageController::save() {
    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
    if (!QFile::exists(directory)) {
        return;
    }

    QFile file(directory + "/PrivateMessageCache.txt");

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);

        stream << m_Datas->length();
        for(int i = 0 ; i < m_Datas->length() ; ++i) {
            stream << *(m_Datas->at(i));

        }

        file.close();
    }
}



void PrivateMessageController::load() {
    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
    if (!QFile::exists(directory)) {
        return;
    }

    QFile file(directory + "/PrivateMessageCache.txt");

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);

        int nbFav = 0;
        stream >> nbFav;
        m_Datas->clear();
        for(int i = 0 ; i < nbFav ; ++i) {
            PrivateMessageListItem *item = new PrivateMessageListItem();
            stream >> (*item);

            m_Datas->push_back(item);
        }

        file.close();

        if(m_Datas->empty()) {
            getMessages();
        } else {
            updateView();
        }

    } else {
        getMessages();
    }
}
