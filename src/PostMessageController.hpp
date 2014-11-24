/*
 * PostMessageController.hpp
 *
 *  Created on: 30 mars 2014
 *      Author: PierreL
 */

#ifndef POSTMESSAGECONTROLLER_HPP_
#define POSTMESSAGECONTROLLER_HPP_

#include <QtCore/QObject>
#include <QString>
#include <QReadWriteLock>

#include <bb/cascades/DropDown>

class PostMessageController : public QObject {
	Q_OBJECT;



private:
	QString m_HashCheck;
	QString m_Parent;
	QString m_PostID;
	QString m_CatID;
	QString m_ResponseID;
	QString m_Page;
	QString m_ThreadTitle;
	QString m_Pseudo;
	bool m_AddSignature;


	QReadWriteLock m_MessageMutex;
	int 	m_NBMessages;
	QString m_Message;

	bool    m_MessageBeingPosted;

	bb::cascades::DropDown *m_DropdownSubCatPicker;

public:

	PostMessageController(QObject *parent = 0) : QObject(parent), m_AddSignature(false), m_NBMessages(0), m_MessageBeingPosted(false), m_DropdownSubCatPicker(NULL) {}
	virtual ~PostMessageController() {}





public Q_SLOTS:


	void checkReply();
	void checkGetMessageReply();
	void checkQuoteMessageReply();

	void postMessage(const QString &hashcheck,
					 const QString &postID,
					 const QString &catID,
					 const QString &page,
					 const QString &pseudo,
					 const QString &message,
					 const QString &threadTitle,
					 bool signature);

	void postMessage(const QString &message);

	void postEdit(const QString &message);

	void postNewPrivateMessage(const QString &hashCheck
							 , const QString &pseudo
							 , bool			signature
							 , const QString &caption
							 , const QString &dest
							 , const QString &message);

	void postNewTopic(         const QString &caption
                             , const QString &subcat
                             , const QString &message);

	void getEditMessage(const QString &url);
	void parseEditMessage(const QString &editpage);

	void getQuotedMessages(const QString &url);
	void parseQuotedMessage(const QString &editpage);


	inline void setDropdown(bb::cascades::DropDown *d)              { m_DropdownSubCatPicker = d; }
	void getSubCatsInfo(const QString &url);
	void getDropdownData();


Q_SIGNALS:
	void complete();
	void messageLoaded(const QString &message_loaded);


private:
	void errorMessage(const QString &page) ;
	void connectionTimedOut();


};


#endif /* POSTMESSAGECONTROLLER_HPP_ */
