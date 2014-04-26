/*
 * PrivateMessageController.hpp
 *
 *  Created on: 29 mars 2014
 *      Author: PierreL
 */

#ifndef PRIVATEMESSAGECONTROLLER_HPP_
#define PRIVATEMESSAGECONTROLLER_HPP_


#include <QtCore/QObject>
#include <bb/cascades/ListView>

class PrivateMessageListItem;

class PrivateMessageController : public QObject {
	Q_OBJECT;

	private:

		bb::cascades::ListView  		 *m_ListView;
		QList<PrivateMessageListItem*>   *m_Datas;


	// ----------------------------------------------------------------------------------------------
	public:
		PrivateMessageController(QObject *parent = 0);
		virtual ~PrivateMessageController() {};


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		inline void setListView	   (QObject *listView) 		{m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
		void getMessages();
		void checkReply();



	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete();




	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page);
		void parseMessageListing(bool read, const QString &threadListing);
		void updateView();

		void checkErrorMessage(const QString &page);
		void connectionTimedOut();
};



#endif /* PRIVATEMESSAGECONTROLLER_HPP_ */
