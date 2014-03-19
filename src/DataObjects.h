/*
 * DataObjects.h
 *
 *  Created on: 16 mars 2014
 *      Author: PierreL
 */

#ifndef DATAOBJECTS_H_
#define DATAOBJECTS_H_

#include <QtCore/QObject>
#include <QString>

class ThreadListItem : public QObject {
	Q_OBJECT

	Q_PROPERTY( QString title READ getTitle      WRITE setTitle      NOTIFY titleChanged)
	Q_PROPERTY( short category  READ getCategory   WRITE setCategory   NOTIFY categoryChanged)


	// ----------------------------------------------------------------------------------------------


private:
	QString m_Title;
	short 	m_Category;




	// ----------------------------------------------------------------------------------------------

public:
	ThreadListItem (const QString &title, int category) : m_Title(title),
														  m_Category(category) {}
	virtual ~ThreadListItem() {}



	inline const QString &getTitle() const				{ return m_Title; }
	inline void			  setTitle(const QString &s)	{ m_Title = s; }

	inline short		  getCategory() const			{ return m_Category; }
	inline void			  setCategory(short c)			{ m_Category = c; }




	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void titleChanged();
		void categoryChanged();

};


#endif /* DATAOBJECTS_H_ */
