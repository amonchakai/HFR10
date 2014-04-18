/*
 * Settings.hpp
 *
 *  Created on: 6 avr. 2014
 *      Author: pierre
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <QtCore/QObject>

class Settings : public QObject {

	Q_OBJECT

	Q_PROPERTY( int fontSize 	READ getFontSize    WRITE setFontSize		NOTIFY fontSizeChanged)
	Q_PROPERTY( int smileySize 	READ getSmileySize  WRITE setSmileySize		NOTIFY smileySizeChanged)

private:


	static int m_FontSize;
	static int m_SmileySize;


public:
	Settings(QObject *parent = 0) : QObject(parent) {}
	virtual ~Settings() {}


	inline void setFontSize(int s) 						{ m_FontSize = s; }
	inline int  getFontSize() const						{ return m_FontSize; }

	inline void setSmileySize(int s) 					{ m_SmileySize = s; }
	inline int  getSmileySize() const					{ return m_SmileySize; }
	static int  smileySize() 							{ return m_SmileySize; }
	static int  fontSize()								{ return m_FontSize; }


	static void	loadSettings();

public Q_SLOTS:
	void saveSettings() const;

	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void fontSizeChanged();
		void smileySizeChanged();

};


#endif /* SETTINGS_HPP_ */
