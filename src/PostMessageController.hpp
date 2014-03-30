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


public:

	PostMessageController(QObject *parent = 0) : QObject(parent) {}
	virtual ~PostMessageController() {}





public Q_SLOTS:


	void checkReply();
	void postMessage(const QString &hashcheck,
					 const QString &postID,
					 const QString &catID,
					 const QString &page,
					 const QString &pseudo,
					 const QString &message,
					 const QString &threadTitle,
					 bool signature);

Q_SIGNALS:
	void complete();
};


#endif /* POSTMESSAGECONTROLLER_HPP_ */
