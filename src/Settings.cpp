/*
 * Settings.cpp
 *
 *  Created on: 6 avr. 2014
 *      Author: pierre
 */


#include "Settings.hpp"
#include <QDebug>
#include <QFile>
#include <QDir>

#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>
#include <bb/system/InvokeTimerRequest>
#include <bb/system/InvokeDateTime>
#include <bb/system/InvokeRecurrenceRule>

int Settings::m_FontSize = 25;
int Settings::m_SmileySize = 3;
int Settings::m_Theme = 0;
int Settings::m_AutoRefresh = 0;
bool Settings::m_HubIntegration = false;
int Settings::m_HubRefreshRate = 5;

bool Settings::m_NotifGreen = false;
bool Settings::m_NotifBlue = false;
bool Settings::m_NotifOrange = false;
bool Settings::m_NotifPink = false;
bool Settings::m_NotifPurple = false;

bool Settings::m_MPNotificationUp = false;
QSettings *Settings::m_Settings = NULL;

QMap<QString, int>   Settings::m_TopicTags;

Settings::Settings(QObject *parent)  : QObject(parent) {

    m_Settings = new QSettings("Amonchakai", "HFR10");

}

void Settings::loadSettings() {

	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return;
	}

	if(m_Settings == NULL)
	    m_Settings = new QSettings("Amonchakai", "HFR10");

	m_FontSize   = m_Settings->value("FontSize", 25).toInt();
	m_SmileySize = m_Settings->value("SimleySize", 3).toInt();
	m_Theme      = m_Settings->value("Theme", 0).toInt();
	m_AutoRefresh= m_Settings->value("AutoRefresh", 0).toInt();
	m_HubIntegration= m_Settings->value("HubIntegration", false).toBool();
	m_HubRefreshRate= m_Settings->value("HubRefreshRate", 5).toInt();

	m_NotifGreen= m_Settings->value("NotifGreen", false).toBool();
	m_NotifBlue= m_Settings->value("NotifBlue", false).toBool();
	m_NotifOrange= m_Settings->value("NotifOrange", false).toBool();
	m_NotifPink= m_Settings->value("NotifPink", false).toBool();
	m_NotifPurple= m_Settings->value("NotifPurple", false).toBool();

	m_MPNotificationUp= m_Settings->value("MPNotificationUp", false).toBool();

	QFile file(directory + "/NotificationList.txt");

	if (file.open(QIODevice::ReadOnly)) {
		QDataStream stream(&file);

		int nbTags = 0;
		stream >> nbTags;
		for(int i = 0 ; i < nbTags ; ++i) {
		    QString key;
		    int value;

		    stream >> key;
		    stream >> value;
		    m_TopicTags[key] = value;
	     }

		file.close();
	}
}

void Settings::updateHub() const {
    if(m_HubIntegration) {

        bb::system::InvokeRequest request;
        request.setTarget("com.amonchakai.HFR10Service");
        request.setAction("bb.action.START");
        bb::system::InvokeManager *invokeManager = new bb::system::InvokeManager();
        bb::system::InvokeTargetReply *reply = invokeManager->invoke(request);
        if (!reply) {
            qWarning() << "failed to start headless " << reply->errorCode();
            reply->deleteLater();
            invokeManager->deleteLater();
        }
    }
}

void Settings::saveSettings() const {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return;
	}


    m_Settings->setValue("FontSize", m_FontSize);
    m_Settings->setValue("SimleySize", m_SmileySize);
    m_Settings->setValue("Theme", m_Theme);
    m_Settings->setValue("AutoRefresh", m_AutoRefresh);

    if(!m_Settings->value("HubIntegration", false).toBool()) {
        if(m_HubIntegration) {

            bb::system::InvokeRecurrenceRule recurrence(bb::system::InvokeRecurrenceRuleFrequency::Minutely);
            recurrence.setInterval(m_HubRefreshRate);
            bb::system::InvokeTimerRequest request("HFR10ServiceStart", recurrence, "com.amonchakai.HFR10Service");

            bb::system::InvokeManager *invokeManager = new bb::system::InvokeManager();
            bb::system::InvokeReply *reply = invokeManager->registerTimer(request);
            if (!reply) {
                qWarning() << "failed to start headless " << reply->errorCode();
                reply->deleteLater();
                invokeManager->deleteLater();
            }

        }

    } else {
        if(!m_HubIntegration) {
            // qDebug() << "Stop Hub!";

            bb::system::InvokeManager *invokeManager = new bb::system::InvokeManager();
            bb::system::InvokeReply * reply = invokeManager->deregisterTimer("HFR10ServiceStart");
            if (!reply) {
                qWarning() << "failed to stop headless " << reply->errorCode();
                reply->deleteLater();
                invokeManager->deleteLater();
            }
        } else {
            // update time...
            bb::system::InvokeManager *invokeManager = new bb::system::InvokeManager();
            bb::system::InvokeReply * reply = invokeManager->deregisterTimer("HFR10ServiceStart");
            if (!reply) {
                qWarning() << "failed to stop headless " << reply->errorCode();
                reply->deleteLater();
                invokeManager->deleteLater();
            }

            bb::system::InvokeRecurrenceRule recurrence(bb::system::InvokeRecurrenceRuleFrequency::Minutely);
            recurrence.setInterval(m_HubRefreshRate);
            bb::system::InvokeTimerRequest request("HFR10ServiceStart", recurrence, "com.amonchakai.HFR10Service");

            reply = invokeManager->registerTimer(request);
            if (!reply) {
                qWarning() << "failed to start headless " << reply->errorCode();
                reply->deleteLater();
                invokeManager->deleteLater();
            }


        }
    }

    m_Settings->setValue("HubIntegration", m_HubIntegration);
    m_Settings->setValue("HubRefreshRate", m_HubRefreshRate);

    saveColors();
}

void Settings::saveColors() const {
    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
    if (!QFile::exists(directory)) {
        return;
    }


    m_Settings->setValue("NotifGreen", m_NotifGreen);
    m_Settings->setValue("NotifBlue", m_NotifBlue);
    m_Settings->setValue("NotifOrange", m_NotifOrange);
    m_Settings->setValue("NotifPink", m_NotifPink);
    m_Settings->setValue("NotifPurple", m_NotifPurple);

    m_Settings->setValue("MPNotificationUp", m_MPNotificationUp);

    QFile file(directory + "/NotificationList.txt");

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);

        int nbTags = 0;
        for(QMap<QString, int>::iterator it = m_TopicTags.begin() ; it != m_TopicTags.end() ; ++it) {
            ++nbTags;
        }
        stream << nbTags;
        for(QMap<QString, int>::iterator it = m_TopicTags.begin() ; it != m_TopicTags.end() ; ++it) {
            stream << it.key();
            stream << it.value();
         }

        file.close();
    }
}


void Settings::setTag(const QString &topicID, int color) {
    if(color == 0 && m_TopicTags.find(topicID) == m_TopicTags.end()) {
        return;
    }

    if(color == 0 && m_TopicTags.find(topicID) != m_TopicTags.end()) {
        m_TopicTags.remove(topicID);
        return;
    }

    m_TopicTags[topicID] = color;
}

int  Settings::getTag(const QString &topicID) const {
    QMap<QString, int>::const_iterator it = m_TopicTags.find(topicID);
    if(it == m_TopicTags.end()) {
        return 0;
    }
    return it.value();
}


int Settings::getTagValue(const QString& topicID) {
    QMap<QString, int>::const_iterator it = m_TopicTags.find(topicID);
    if(it == m_TopicTags.end()) {
        return 0;
    }
    return it.value();
}
