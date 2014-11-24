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

int Settings::m_FontSize = 25;
int Settings::m_SmileySize = 3;
int Settings::m_Theme = 0;
int Settings::m_AutoRefresh = 0;
bool Settings::m_HubIntegration = true;
int Settings::m_HubRefreshRate = 5;

bool Settings::m_NotifGreen = false;
bool Settings::m_NotifBlue = false;
bool Settings::m_NotifOrange = false;
bool Settings::m_NotifPink = false;
bool Settings::m_NotifPurple = false;

bool Settings::m_MPNotificationUp = false;

QMap<QString, int>   Settings::m_TopicTags;

void Settings::loadSettings() {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return;
	}

	QFile file(directory + "/Settings.txt");

	if (file.open(QIODevice::ReadOnly)) {
		QDataStream stream(&file);
		stream >> m_FontSize;
		stream >> m_SmileySize;
		stream >> m_Theme;
		stream >> m_AutoRefresh;
		stream >> m_HubIntegration;
		stream >> m_HubRefreshRate;
		stream >> m_NotifGreen;
		stream >> m_NotifBlue;
		stream >> m_NotifOrange;
		stream >> m_NotifPink;
		stream >> m_NotifPurple;

		int nbTags = 0;
		stream >> nbTags;
		for(int i = 0 ; i < nbTags ; ++i) {
		    QString key;
		    int value;

		    stream >> key;
		    stream >> value;
		    m_TopicTags[key] = value;
	     }

		stream >> m_MPNotificationUp;

		file.close();
	}
}


void Settings::saveSettings() const {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return;
	}

	QFile file(directory + "/Settings.txt");

	if (file.open(QIODevice::WriteOnly)) {
		QDataStream stream(&file);
		stream << m_FontSize;
		stream << m_SmileySize;
		stream << m_Theme;
		stream << m_AutoRefresh;
		stream << m_HubIntegration;
		stream << m_HubRefreshRate;
		stream << m_NotifGreen;
		stream << m_NotifBlue;
		stream << m_NotifOrange;
        stream << m_NotifPink;
        stream << m_NotifPurple;

        int nbTags = 0;
        for(QMap<QString, int>::iterator it = m_TopicTags.begin() ; it != m_TopicTags.end() ; ++it) {
            ++nbTags;
        }
        stream << nbTags;
        for(QMap<QString, int>::iterator it = m_TopicTags.begin() ; it != m_TopicTags.end() ; ++it) {
            stream << it.key();
            stream << it.value();
         }

        stream << m_MPNotificationUp;

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
