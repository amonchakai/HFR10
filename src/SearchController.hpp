/*
 * SearchController.hpp
 *
 *  Created on: 15 avr. 2014
 *      Author: pierre
 */

#ifndef SEARCHCONTROLLER_HPP_
#define SEARCHCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>

class ThreadListItem;

class SearchController : public QObject {
	Q_OBJECT;

	private:

		bb::cascades::ListView  		 *m_ListView;
		QList<ThreadListItem*>   		 *m_Datas;


	// ----------------------------------------------------------------------------------------------
	public:
		SearchController(QObject *parent = 0);
		virtual ~SearchController() {};


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		inline void setListView	   (QObject *listView) 		{m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
		void checkReply();
		void search(const QString &search,
					const QString &author,
					int searchType,
					int categoryType,
					int searchIn,
					int numberOfMessages,
					int sortBy);


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete();




	// ----------------------------------------------------------------------------------------------
	private:

		void waitMore(const QString &page);
		void parse(const QString &page);
		void parseThreadListing(const QString &caption, const QString &urlFirstPage, bool read, const QString &threadListing);
		void updateView();
};


#endif /* SEARCHCONTROLLER_HPP_ */
