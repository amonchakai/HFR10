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



void Settings::loadSettings() {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return;
	}

	QFile file(directory + "/Settings.txt");

	if (file.open(QIODevice::ReadOnly)) {
		qDebug() << "loadSettings";
		QDataStream stream(&file);
		stream >> m_FontSize;
		stream >> m_SmileySize;

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
		qDebug() << "writeSettings";
		QDataStream stream(&file);
		stream << m_FontSize;
		stream << m_SmileySize;

		file.close();
	}
}
