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

		file.close();
	}
}

