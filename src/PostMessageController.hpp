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



public:

	PostMessageController(QObject *parent = 0) : QObject(parent) {}
	virtual ~PostMessageController() {}





public Q_SLOTS:


	void checkReply();
	void checkGetMessageReply();

	void postMessage(const QString &hashcheck,
					 const QString &postID,
					 const QString &catID,
					 const QString &page,
					 const QString &pseudo,
					 const QString &message,
					 const QString &threadTitle,
					 bool signature);

	void postEdit(const QString &message);


	void getEditMessage(const QString &url);
	void parseEditMessage(const QString &editpage);

Q_SIGNALS:
	void complete();
	void messageLoaded(const QString &message_loaded);
};


#endif /* POSTMESSAGECONTROLLER_HPP_ */
