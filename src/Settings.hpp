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
	Q_PROPERTY( int theme	 	READ getTheme	    WRITE setTheme			NOTIFY themeChanged)
	Q_PROPERTY( int autoRefresh	 READ getAutoRefresh WRITE setAutoRefresh   NOTIFY autoRefreshChanged)

private:


	static int m_FontSize;
	static int m_SmileySize;
	static int m_Theme;
	static int m_AutoRefresh;


public:
	Settings(QObject *parent = 0) : QObject(parent) {}
	virtual ~Settings() {}


	inline void setFontSize(int s) 						{ m_FontSize = s; }
	inline int  getFontSize() const						{ return m_FontSize; }

	inline void setTheme(int s) 						{ m_Theme = s; }
	inline int  getTheme() const						{ return m_Theme; }

	inline void setSmileySize(int s) 						{ m_SmileySize = s; }
	inline int  getSmileySize() const						{ return m_SmileySize; }

	inline void setAutoRefresh(int s) 					{ m_AutoRefresh = s; }
	inline int  getAutoRefresh() const					{ return m_AutoRefresh; }

	static int  smileySize() 							{ return m_SmileySize; }
	static int  fontSize()								{ return m_FontSize; }

	static int  &themeValue()							{ return m_Theme; }

	static void	loadSettings();

public Q_SLOTS:
	void saveSettings() const;

	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void fontSizeChanged();
		void smileySizeChanged();
		void themeChanged();
		void autoRefreshChanged();

};


#endif /* SETTINGS_HPP_ */
