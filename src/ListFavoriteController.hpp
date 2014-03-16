/*
 * ListFavoriteController.hpp
 *
 *  Created on: 15 mars 2014
 *      Author: PierreL
 */

#ifndef LISTFAVORITECONTROLLER_HPP_
#define LISTFAVORITECONTROLLER_HPP_


#include <QtCore/QObject>


class ListFavoriteController : public QObject {
	Q_OBJECT

	private:


	// ----------------------------------------------------------------------------------------------
	public:
	    ListFavoriteController(QObject *parent = 0);
		virtual ~ListFavoriteController() {};




	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		void getFavorite();
		void checkReply();



	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete(const QString &info);




	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page);

};


#endif /* LISTFAVORITECONTROLLER_HPP_ */
