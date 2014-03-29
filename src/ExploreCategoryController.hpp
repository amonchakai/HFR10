/*
 * ExploreCategoryController.hpp
 *
 *  Created on: 27 mars 2014
 *      Author: PierreL
 */

#ifndef EXPLORECATEGORYCONTROLLER_HPP_
#define EXPLORECATEGORYCONTROLLER_HPP_


#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <QString>

class ThreadListItem;

class ExploreCategoryController : public QObject {
	Q_OBJECT;

	private:

		bb::cascades::ListView   *m_ListView;
		QList<ThreadListItem*>   *m_Datas;

		QString					  m_GeneralUrl;
		QString					  m_Url;
		QString					  m_LastLoadedUrl;


	// ----------------------------------------------------------------------------------------------
	public:
		ExploreCategoryController(QObject *parent = 0);
		virtual ~ExploreCategoryController() {};


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		inline void setListView	   	(QObject *listView) 		{m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
		void listTopics			   	(const QString &url);
		void checkReply				();
		void filterByFlag			(int flag);
		void refresh				();

		void firstPage();
		void nextPage();
		void prevPage();


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete();




	// ----------------------------------------------------------------------------------------------
	private:

		void showTopicList(const QString &url);
		void parse(const QString &page);
		void parseThreadListing(const QString &caption, bool read, const QString &threadListing);
		void updateView();

};




#endif /* EXPLORECATEGORYCONTROLLER_HPP_ */
