/*
 * ListFavoriteController.hpp
 *
 *  Created on: 15 mars 2014
 *      Author: PierreL
 */

#ifndef LISTFAVORITECONTROLLER_HPP_
#define LISTFAVORITECONTROLLER_HPP_


#include <QtCore/QObject>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>


class ListFavoriteController : public QObject {
	Q_OBJECT;

	private:

		static bb::cascades::AbstractPane   *m_Pane;
		bb::cascades::GroupDataModel		*m_DataModel;


	// ----------------------------------------------------------------------------------------------
	public:
	    ListFavoriteController(QObject *parent = 0);
		virtual ~ListFavoriteController() {};


		static void setAbstractPane(bb::cascades::AbstractPane *root);


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		void getFavorite();
		void checkReply();



	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete();




	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page);

};


#endif /* LISTFAVORITECONTROLLER_HPP_ */
