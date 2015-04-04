/*
 * Settings.hpp
 *
 *  Created on: 6 avr. 2014
 *      Author: pierre
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <QtCore/QObject>
#include <QSettings>

class Settings : public QObject {

	Q_OBJECT

	Q_PROPERTY( int fontSize 	READ getFontSize    WRITE setFontSize		NOTIFY fontSizeChanged)
	Q_PROPERTY( int smileySize 	READ getSmileySize  WRITE setSmileySize		NOTIFY smileySizeChanged)
	Q_PROPERTY( int theme	 	READ getTheme	    WRITE setTheme			NOTIFY themeChanged)
	Q_PROPERTY( int autoRefresh	 READ getAutoRefresh WRITE setAutoRefresh   NOTIFY autoRefreshChanged)
	Q_PROPERTY( bool hubIntegration  READ getHubIntegration WRITE setHubIntegration   NOTIFY hubIntegrationChanged)
	Q_PROPERTY( int hubRefreshRate  READ getHubRefreshRate WRITE setHubRefreshRate   NOTIFY hubRefreshRateChanged)

	Q_PROPERTY( bool notifGreen  READ getNotifGreen WRITE setNotifGreen   NOTIFY notifGreenChanged)
	Q_PROPERTY( bool notifBlue  READ getNotifBlue WRITE setNotifBlue   NOTIFY notifBlueChanged)
	Q_PROPERTY( bool notifOrange  READ getNotifOrange WRITE setNotifOrange   NOTIFY notifOrangeChanged)
	Q_PROPERTY( bool notifPink  READ getNotifPink WRITE setNotifPink   NOTIFY notifPinkChanged)
	Q_PROPERTY( bool notifPurple  READ getNotifPurple WRITE setNotifPurple   NOTIFY notifPurpleChanged)

private:


	static int m_FontSize;
	static int m_SmileySize;
	static int m_Theme;
	static int m_AutoRefresh;
	static bool m_HubIntegration;
	static int m_HubRefreshRate;

	static bool m_NotifGreen;
	static bool m_NotifBlue;
	static bool m_NotifOrange;
	static bool m_NotifPink;
	static bool m_NotifPurple;

	static bool m_MPNotificationUp;

	static QMap<QString, int>   m_TopicTags;

	static QSettings  *m_Settings;


public:
	Settings(QObject *parent = 0);
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

	inline void setHubIntegration(bool s)                   { m_HubIntegration = s; }
	inline bool getHubIntegration() const                  { return m_HubIntegration; }

	inline void setHubRefreshRate(int s)                   { m_HubRefreshRate = s; }
	inline int getHubRefreshRate() const                   { return m_HubRefreshRate; }

	inline void setNotifGreen(bool s)                   { m_NotifGreen = s; }
	inline bool getNotifGreen() const                   { return m_NotifGreen; }

	inline void setNotifBlue(bool s)                    { m_NotifBlue = s; }
	inline bool getNotifBlue() const                    { return m_NotifBlue; }

	inline void setNotifOrange(bool s)                  { m_NotifOrange = s; }
	inline bool getNotifOrange() const                  { return m_NotifOrange; }

	inline void setNotifPink(bool s)                    { m_NotifPink = s; }
	inline bool getNotifPink() const                    { return m_NotifPink; }

    inline void setNotifPurple(bool s)                  { m_NotifPurple = s; }
    inline bool getNotifPurple() const                  { return m_NotifPurple; }

	static void	loadSettings();

	static int getTagValue(const QString& topicID);

	static bool getMPNotificationUp()                     { return m_MPNotificationUp; }
	static void setMPNotificationUp(bool value)           { m_MPNotificationUp = value; }

public Q_SLOTS:
	void saveSettings() const;
	void saveColors() const;
	void updateHub() const;

	void setTag(const QString &topicID, int color);
	int  getTag(const QString &topicID) const;


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void fontSizeChanged();
		void smileySizeChanged();
		void themeChanged();
		void autoRefreshChanged();
		void hubIntegrationChanged();
		void hubRefreshRateChanged();
		void notifGreenChanged();
		void notifBlueChanged();
		void notifOrangeChanged();
		void notifPinkChanged();
		void notifPurpleChanged();

};


#endif /* SETTINGS_HPP_ */
