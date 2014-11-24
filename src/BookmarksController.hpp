/*
 * BookmarksController.hpp
 *
 *  Created on: 14 sept. 2014
 *      Author: pierre
 */

#ifndef BOOKMARKSCONTROLLER_HPP_
#define BOOKMARKSCONTROLLER_HPP_


#include <QtCore/QObject>
#include <bb/cascades/ListView>

class ThreadListItem;

class BookmarksController : public QObject {
    Q_OBJECT;

    private:

        bb::cascades::ListView   *m_ListView;
        QList<ThreadListItem*>   *m_Datas;

    // ----------------------------------------------------------------------------------------------
    public:
        BookmarksController(QObject *parent = 0);
        virtual ~BookmarksController() {};


    // ----------------------------------------------------------------------------------------------

    public Q_SLOTS:
        inline void setListView    (QObject *listView)      {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
        void getFavorite();
        void updateView();
        void deleteBookmark(QString urlFirstPage);

    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void loading();
        void complete();




    // ----------------------------------------------------------------------------------------------
    private:


};




#endif /* BOOKMARKSCONTROLLER_HPP_ */
