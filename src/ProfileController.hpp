/*
 * ProfileController.hpp
 *
 *  Created on: 20 sept. 2015
 *      Author: pierre
 */

#ifndef PROFILECONTROLLER_HPP_
#define PROFILECONTROLLER_HPP_


#include <QtCore/QObject>
#include <bb/cascades/ListView>

class ProfileController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( QString online      READ getOnline)
    Q_PROPERTY( QString pseudo      READ getPseudo)
    Q_PROPERTY( QString email       READ getEmail)
    Q_PROPERTY( QString birthday    READ getBirthday)
    Q_PROPERTY( QString site        READ getSite)
    Q_PROPERTY( QString gender      READ getGender)
    Q_PROPERTY( QString city        READ getCity)
    Q_PROPERTY( QString hobby       READ getHobby)
    Q_PROPERTY( QString job         READ getJob)

    Q_PROPERTY( QString status      READ getStatus)
    Q_PROPERTY( QString postCount   READ getPostCount)
    Q_PROPERTY( QString registrationTime   READ getRegistrationTime)
    Q_PROPERTY( QString lastmessage       READ getLastMessage)
    Q_PROPERTY( QString citation          READ getCitation)
    Q_PROPERTY( QString signature       READ getSignature)

    Q_PROPERTY( QString transationVG      READ getTransactionVG)
    Q_PROPERTY( QString transation        READ getTransaction)

    Q_PROPERTY( QString nbSalesVG      READ getNumberOfSalesVG)
    Q_PROPERTY( QString nbSales        READ getNumberOfSales)

private:

    QString m_Online;
    QString m_Pseudo;
    QString m_Email;
    QString m_Birthday;
    QString m_Site;
    QString m_Gender;
    QString m_City;
    QString m_Hobby;
    QString m_Job;
    QString m_Status;
    QString m_PostCount;
    QString m_RegistrationTimestamp;
    QString m_LastMessageTimestamp;
    QString m_Citation;
    QString m_Signature;
    QString m_ConfigurationUrl;
    QString m_TransationsVideoGamesUrl;
    QString m_TransationsUrl;
    QString m_NumberSalesVG;
    QString m_NumberSales;

    bb::cascades::ListView   *m_ListView;


public:

    ProfileController           (QObject *parent = 0) : QObject(parent), m_ListView(NULL){}
    virtual ~ProfileController  () {}


    inline const QString &getOnline() const                    { return m_Online; }
    inline const QString &getPseudo() const                    { return m_Pseudo; }
    inline const QString &getEmail() const                     { return m_Email; }
    inline const QString &getBirthday() const                  { return m_Birthday; }
    inline const QString &getSite() const                      { return m_Site; }
    inline const QString &getGender() const                    { return m_Gender; }
    inline const QString &getCity() const                      { return m_City; }
    inline const QString &getHobby() const                     { return m_Hobby; }
    inline const QString &getJob() const                       { return m_Job; }
    inline const QString &getPostCount() const                 { return m_PostCount; }
    inline const QString &getStatus() const                    { return m_Status; }
    inline const QString &getRegistrationTime() const          { return m_RegistrationTimestamp; }
    inline const QString &getLastMessage() const               { return m_LastMessageTimestamp; }
    inline const QString &getCitation() const                  { return m_Citation; }
    inline const QString &getSignature() const                 { return m_Signature; }
    inline const QString &getTransactionVG() const             { return m_TransationsVideoGamesUrl; }
    inline const QString &getTransaction() const               { return m_TransationsUrl; }
    inline const QString &getNumberOfSalesVG() const           { return m_NumberSalesVG; }
    inline const QString &getNumberOfSales() const             { return m_NumberSales; }

private:
    void cleanup               (QString &str);

public Q_SLOTS:
    void loadProfile            (const QString& url);
    void checkReply             ();

    void connectionTimedOut     ();
    void parse                  (const QString& page);

    inline void setListView    (QObject *listView)      {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
    void loadFeedback          (const QString& url);
    void checkReplyFeedback    ();
    void parseFeedback         (const QString& page);

    void blacklistUser         ();


Q_SIGNALS:

    void complete();

};



#endif /* PROFILECONTROLLER_HPP_ */
