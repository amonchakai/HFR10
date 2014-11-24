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
#include <QMap>

class ThreadListItem;

class ListFavoriteController : public QObject {
	Q_OBJECT;

	private:

		bb::cascades::ListView   *m_ListView;
		QList<ThreadListItem*>   *m_Datas;
		QString                   m_HashCheck;
		QList<QString>            m_CategoriesLabels;

	// ----------------------------------------------------------------------------------------------
	public:
	    ListFavoriteController(QObject *parent = 0);
		virtual ~ListFavoriteController() {};


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		inline void setListView	   (QObject *listView) 		{m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
		void getFavorite();
		void checkReply();
		void checkReplyDeleteFlag();
		void updateView();
		void deleteFlag(const QString &url);
		void load();

		inline const QString getCatCaption(int index)      {if(index < m_CategoriesLabels.length()) return m_CategoriesLabels.at(index); else return "";}

	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
	    void loading();
		void complete();




	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page);
		void parseThreadListing(const QString &category, const QString &caption, const QString &urlFirstPage, const QString &threadListing, const QString &today, int groupKey);


		void checkErrorMessage(const QString &page);
		void connectionTimedOut();

		void save();
};


#endif /* LISTFAVORITECONTROLLER_HPP_ */
