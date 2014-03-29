/*
 * ShowThreadController.hpp
 *
 *  Created on: 20 mars 2014
 *      Author: PierreL
 */

#ifndef SHOWTHREADCONTROLLER_HPP_
#define SHOWTHREADCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <QList>

struct PostDetailItem;

class ShowThreadController : public QObject {
	Q_OBJECT;

	private:

		bb::cascades::ListView 				*m_ListView;
		QList<PostDetailItem*>				*m_Datas;
		mutable QReadWriteLock	 			 m_EditData;

		QString								 m_UrlNextPage;
		QString								 m_UrlPrevPage;
		QString								 m_UrlFirstPage;
		QString								 m_UrlLastPage;
		QString								 m_Url;

	// ----------------------------------------------------------------------------------------------
	public:
		ShowThreadController(QObject *parent = 0);
		virtual ~ShowThreadController() {};


		void updateView();

	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		void showThread(const QString &url);
		inline void setListView	   (QObject *listView) 		{m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
		void checkReply();

		void nextPage();
		void prevPage();
		void firstPage();
		void lastPage();
		void scrollToItem();



	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete();



	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page);
		void parsePost(const QString &postIdex, const QString &author, const QString &post);
		void cleanupPost(QString &post);
};


#endif /* SHOWTHREADCONTROLLER_HPP_ */
