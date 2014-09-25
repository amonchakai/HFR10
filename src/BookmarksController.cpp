/*
 * BookmarksController.cpp
 *
 *  Created on: 14 sept. 2014
 *      Author: pierre
 */

#include "BookmarksController.hpp"
#include  "DataObjects.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>


BookmarksController::BookmarksController(QObject *parent) : QObject(parent), m_ListView(NULL), m_Datas(new QList<ThreadListItem*>()) {

}

void BookmarksController::getFavorite() {

    m_Datas->clear();

    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
    if (!QFile::exists(directory)) {
        return;
    }

    QFile file(directory + "/Bookmarks.txt");

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);

        ThreadListItem *item = new ThreadListItem();
        stream >> (*item);

        while(item->getTitle() != "") {
            m_Datas->push_back(item);
            item = new ThreadListItem();

            stream >> (*item);
        }
        item->deleteLater();

        file.close();

        updateView();
    }


}

void BookmarksController::deleteBookmark(QString urlFirstPage) {
    for(int i = 0 ; i < m_Datas->length() ; ++i) {
        if(m_Datas->at(i)->getUrlFirstPage() == urlFirstPage) {
            m_Datas->removeAt(i);
            break;
        }
    }

    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
    if (!QFile::exists(directory)) {
        return;
    }

    QFile file(directory + "/Bookmarks.txt");

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);

        for(int i = 0 ; i < m_Datas->length() ; ++i) {
            stream << (*(m_Datas->at(i)));
        }

        file.close();
    }
}


void BookmarksController::updateView() {
    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if (dataModel) {
        dataModel->clear();
    } else {
        qDebug() << "create new model";
        dataModel = new GroupDataModel(
                QStringList() << "title"
                              << "timestamp"
                              << "lastAuthor"
                              << "urlFirstPage"
                              << "urlLastPage"
                              << "urlLastPostRead"
                              << "pages"
                              << "flagType"
                              << "read"
                              << "color"
                );
        m_ListView->setDataModel(dataModel);
    }
    dataModel->setGrouping(ItemGrouping::ByFullValue);

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    QList<QObject*> datas;
    for(int i = m_Datas->length()-1 ; i >= 0 ; --i) {
        datas.push_back(m_Datas->at(i));
    }

    dataModel->clear();
    dataModel->insertList(datas);
}
