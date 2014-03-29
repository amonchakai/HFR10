/*
 * ListFavoriteController.hpp
 *
 *  Created on: 15 mars 2014
 *      Author: PierreL
 */

#ifndef LISTFAVORITECONTROLLER_HPP_
#define LISTFAVORITECONTROLLER_HPP_


#include <QtCore/QObject>
#include <bb/cascades/ListView>

class ThreadListItem;

class ListFavoriteController : public QObject {
	Q_OBJECT;

	private:

		bb::cascades::ListView   *m_ListView;
		QList<ThreadListItem*>   *m_Datas;


	// ----------------------------------------------------------------------------------------------
	public:
	    ListFavoriteController(QObject *parent = 0);
		virtual ~ListFavoriteController() {};


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		inline void setListView	   (QObject *listView) 		{m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
		void getFavorite();
		void checkReply();



	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete();




	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page);
		void parseThreadListing(const QString &category, const QString &caption, const QString &threadListing, const QString &today);
		void updateView();

};


#endif /* LISTFAVORITECONTROLLER_HPP_ */
