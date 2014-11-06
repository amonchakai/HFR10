/*
 * SmileyPickerController.hpp
 *
 *  Created on: 5 avr. 2014
 *      Author: pierre
 */

#ifndef SMILEYPICKERCONTROLLER_HPP_
#define SMILEYPICKERCONTROLLER_HPP_

#include <QtCore/QObject>
#include <QString>
#include <QList>
#include <QReadWriteLock>

#include <bb/cascades/ListView>

class Emoticon;

class SmileyPickerController : public QObject {
	Q_OBJECT;

	private:

	bb::cascades::ListView 				*m_ListView;
	QList<Emoticon*>					 m_Emoticons;
	QString                              m_Page;

	int									 m_lastId;
	QList<int>							 m_IndexSubpagesInFile;
	QReadWriteLock                       m_Mutex;
	bool                                 m_Pushing;

	// ----------------------------------------------------------------------------------------------
	public:
		SmileyPickerController(QObject *parent = 0);
		virtual ~SmileyPickerController() {};


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		void loadDefautSmiley();
		void getSmiley(const QString &keyword);
		void getNextPage();
		void getPrevPage();

		void checkReply();
		void pushToView(Emoticon *e);


		inline void setListView(QObject *listview) 				{m_ListView = dynamic_cast<bb::cascades::ListView*>(listview); }
		void onImageReady      (const QString &url, const QString &diskPath);



	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete();




	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page, int startingPosition = 0);
		void updateView();

};




#endif /* SMILEYPICKERCONTROLLER_HPP_ */
