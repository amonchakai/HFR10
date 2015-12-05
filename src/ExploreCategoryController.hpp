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
#include <bb/system/SystemListDialog>
#include <QString>

class ThreadListItem;

class ExploreCategoryController : public QObject {
	Q_OBJECT;

	private:

		bb::cascades::ListView   *m_ListView;
		bb::cascades::ListView   *m_RootForumListView;
		QList<ThreadListItem*>   *m_Datas;

		int                       m_Flag;
		int 					  m_SelectedSubCat;
		QList<QString>			  m_SubCatURL;

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
		inline void setForumRootListView(QObject *listView)     {m_RootForumListView = dynamic_cast<bb::cascades::ListView*>(listView); }
		void loadIndex              ();
		void buildIndex             ();
		void loadSubCats			(const QString &xmlFile);
		void listSubCat				(int subcat);
		void listTopics			   	(const QString &url, int flag = -1, int cat = -1);
		void checkReply				();
		void checkReplyIndex        ();
		void parseIndex             (const QString &page);
		void parseIndexDetails      (const QString& catDetails, const QString catId);
		void filterByFlag			(int flag);
		void refresh				();

		void firstPage();
		void nextPage();
		void prevPage();


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
	    void catLoaded();
		void complete();
		void flagChanged();




	// ----------------------------------------------------------------------------------------------
	private:

		void showTopicList(const QString &url);
		void parse(const QString &page);
		void parseThreadListing(const QString &caption, const QString &urlFirstPage, bool read, const QString &threadListing);
		void updateView();
		void updateIndexView(const QByteArray& buffer);

};




#endif /* EXPLORECATEGORYCONTROLLER_HPP_ */
