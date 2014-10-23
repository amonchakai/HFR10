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
		QString                           m_HashCheck;


	// ----------------------------------------------------------------------------------------------
	public:
		PrivateMessageController(QObject *parent = 0);
		virtual ~PrivateMessageController() {};


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		inline void setListView	   (QObject *listView) 		{m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
		void getMessages();
		void checkReply();

		void deletePrivateMessage(const QString &url);
		void checkMessageDeleted();
		void load();
		void checkforUnreadMP();


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
	    void loading();
		void complete();




	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page);
		void parseMessageListing(bool read, const QString &threadListing);
		void updateView();

		void checkErrorMessage(const QString &page);
		void connectionTimedOut();
		void save();
};



#endif /* PRIVATEMESSAGECONTROLLER_HPP_ */
