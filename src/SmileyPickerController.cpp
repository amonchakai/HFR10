/*
 * SmileyPickerController.cpp
 *
 *  Created on: 5 avr. 2014
 *      Author: pierre
 */
#include "SmileyPickerController.hpp"



#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>

#include  "Globals.h"
#include  "DataObjects.h"
#include  "Network/HFRNetworkAccessManager.hpp"
#include  "Network/WebResourceManager.h"

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>


SmileyPickerController::SmileyPickerController(QObject *parent) : QObject(parent), m_ListView(NULL), m_lastId(0), m_Pushing(false) {

    bool check = connect(WebResourceManager::get(), SIGNAL(onImageReady(const QString &, const QString &)), this, SLOT(onImageReady(const QString &, const QString &)));
    Q_ASSERT(check);
    Q_UNUSED(check);

};

void SmileyPickerController::loadDefautSmiley() {
    m_Emoticons.clear();

    Emoticon *e;
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/pfff.gif"); e->setTag(":pfff:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smile.gif");        e->setTag(":)");     m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/mad.gif");          e->setTag(":fou:");  m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/love.gif"); e->setTag(":love:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/ddr555.gif");       e->setTag("[:ddr555]"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/ange.gif"); e->setTag(":ange:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/frown.gif");        e->setTag(":("); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/confused.gif");     e->setTag(":??:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/heink.gif");e->setTag(":heink:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/biggrin.gif");      e->setTag(":D"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/tongue.gif");       e->setTag(":p"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/lol.gif");  e->setTag(":lol:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/cry.gif");  e->setTag(":cry:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/sleep.gif");e->setTag(":sleep:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/sweat.gif");e->setTag(":sweat:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/wink.gif");         e->setTag(";)"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/na.gif");   e->setTag(":na:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/wahoo.gif");e->setTag(":wahoo:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/bounce.gif");e->setTag(":bounce:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/ouch.gif");  e->setTag(":ouch:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/sarcastic.gif"); e->setTag(":sarcastic:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/kaola.gif");  e->setTag(":kaola:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/sol.gif");    e->setTag(":sol:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/hello.gif");  e->setTag(":hello:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/non.gif");    e->setTag(":non:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/pouah.gif");  e->setTag(":pouah:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/whistle.gif");e->setTag(":whistle:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/smilies/miam.gif");   e->setTag(":miam:"); m_Emoticons.push_back(e);
    e = new Emoticon; e->setLocalUrl("asset:///images/smiley/hebe.gif");           e->setTag(":hebe:"); m_Emoticons.push_back(e);

    m_Mutex.lockForWrite();
    m_Pushing = false;
    m_Page = "";
    m_Mutex.unlock();

    updateView();
}

void SmileyPickerController::getSmiley(const QString &keyword) {

    if(keyword.isEmpty())
        return;

	// list green + yellow flags
	const QUrl url(DefineConsts::FORUM_URL + "/message-smi-mp-aj.php?config=hfr.inc&findsmilies=" + keyword);

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    dataModel->clear();

}



void SmileyPickerController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				m_Page = response;
				m_IndexSubpagesInFile.clear();
				m_IndexSubpagesInFile.append(0);
				m_lastId = 0;
				m_Mutex.lockForWrite();
				m_Pushing = true;
				m_Mutex.unlock();
				parse(response);
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}



void SmileyPickerController::parse(const QString &page, int startingPosition) {
    m_Emoticons.clear();

	QRegExp smiley("<img src=\"([^\"]+)\" alt=\"([^\"]+)\".*/>");
	qDebug() << "start parsing";

	smiley.setCaseSensitivity(Qt::CaseSensitive);
	smiley.setMinimal(true);

	int nbSmileyPage = 0;
	int pos = startingPosition;
	while((pos = smiley.indexIn(page, pos)) != -1 && nbSmileyPage < 60) {
	    qDebug() << smiley.cap(2);
	    m_Mutex.lockForWrite();
	    Emoticon *e = new Emoticon;
	    e->setTag(smiley.cap(2));
	    e->setDistUrl(smiley.cap(1));
	    m_Emoticons.push_back(e);
	    m_Mutex.unlock();

	    WebResourceManager::get()->getImage(smiley.cap(1));


		pos += smiley.matchedLength();

		++nbSmileyPage;
	}
	qDebug() << "end parsing";


	// -------------------------------------------------------------
	// build index in file for next/previous nav.

	if(pos != -1) {
		bool indexFound = false;
		for(int i = 0 ; i < m_IndexSubpagesInFile.length() ; ++i)
			if(m_IndexSubpagesInFile[i] == pos) {
				indexFound = true;
				break;
			}

		if(!indexFound) {
			m_IndexSubpagesInFile.push_back(pos);
		}
	}

	emit complete();

}


void SmileyPickerController::getNextPage() {
	if(m_IndexSubpagesInFile.length() == 0)
		return;

	if(m_Page.isEmpty())
	    return;

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    dataModel->clear();

	m_lastId = std::min(m_lastId+1, m_IndexSubpagesInFile.length()-1);

	parse(m_Page, m_IndexSubpagesInFile[m_lastId]);
}

void SmileyPickerController::getPrevPage() {
	if(m_IndexSubpagesInFile.length() == 0)
		return;

	if(m_Page.isEmpty())
	    return;

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    dataModel->clear();

	m_lastId = std::max(m_lastId-1, 0);

	parse(m_Page, m_IndexSubpagesInFile[m_lastId]);
}


void SmileyPickerController::updateView() {
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
                QStringList() << "tag"
                              << "localUrl"
                 );
        m_ListView->setDataModel(dataModel);
    }

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    QList<QObject*> datas;
    for(int i = 0 ; i < m_Emoticons.size() ; ++i) {

        Emoticon *e = new Emoticon;
        e->setLocalUrl(m_Emoticons.at(i)->getLocalUrl());
        e->setTag(m_Emoticons.at(i)->getTag());
        datas.push_back(e);

    }

    dataModel->clear();
    dataModel->insertList(datas);


}



void SmileyPickerController::onImageReady(const QString &url, const QString &diskPath) {

    m_Mutex.lockForWrite();
    if(!m_Pushing) {
        m_Mutex.unlock();
        return;
    }
    m_Mutex.unlock();


    if(diskPath == "loading")
        return;

    if(url[0] == '/')
        return;

    qDebug() << url << diskPath;

    m_Mutex.lockForWrite();
    Emoticon *e = NULL;
    bool alreadyLoaded = false;
    for(int i = 0 ; i < m_Emoticons.size() ; ++i) {
        if(m_Emoticons.at(i)->getDistUrl() == url) {
            if(m_Emoticons.at(i)->getLocalUrl().isEmpty())
                m_Emoticons.at(i)->setLocalUrl(diskPath);
            else
                alreadyLoaded = true;
            e = m_Emoticons[i];
            break;
        }
    }

    if(!alreadyLoaded)
        pushToView(e);

    m_Mutex.unlock();

}

void SmileyPickerController::pushToView(Emoticon *e) {
    if(e == NULL)
        return;

    // ----------------------------------------------------------------------------------------------
        // get the dataModel of the listview if not already available
        using namespace bb::cascades;

        if(m_ListView == NULL) {
            qWarning() << "did not received the listview. quit.";
            return;
        }

        GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
        if (!dataModel) {

            qDebug() << "create new model";
            dataModel = new GroupDataModel(
                    QStringList() << "tag"
                                  << "localUrl"
                     );
            m_ListView->setDataModel(dataModel);
        }

        // ----------------------------------------------------------------------------------------------
        // push data to the view

        dataModel->insert(e);

}


