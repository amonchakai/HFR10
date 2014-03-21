/*
 * ShowThreadController.hpp
 *
 *  Created on: 20 mars 2014
 *      Author: PierreL
 */

#ifndef SHOWTHREADCONTROLLER_HPP_
#define SHOWTHREADCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>

class ShowThreadController : public QObject {
	Q_OBJECT;

	private:

		static bb::cascades::AbstractPane   *m_Pane;
		bb::cascades::GroupDataModel		*m_DataModel;


	// ----------------------------------------------------------------------------------------------
	public:
		ShowThreadController(QObject *parent = 0);
		virtual ~ShowThreadController() {};


		static void setAbstractPane(bb::cascades::AbstractPane *root);


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		void showThread(const QString &url);
		void checkReply();



	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete(const QString &info);



	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page);
		void parsePost(const QString &postIdex, const QString &author, const QString &post);

};


#endif /* SHOWTHREADCONTROLLER_HPP_ */
