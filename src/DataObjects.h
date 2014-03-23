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



class PostDetailItem : public QObject {
	Q_OBJECT;

	Q_PROPERTY( QString author		READ getAuthor      WRITE setAuthor		NOTIFY authorChanged)
	Q_PROPERTY( QString avatar		READ getAvatar      WRITE setAvatar		NOTIFY avatarChanged)
	Q_PROPERTY( QString timestamp	READ getTimestamp	WRITE setTimestamp	NOTIFY timestampChanged)
	Q_PROPERTY( QString post		READ getPost		WRITE setPost		NOTIFY postChanged)

	// ----------------------------------------------------------------------------------------------

private:
	QString m_Author;
	QString m_Avatar;
	QString m_Timestamp;
	QString m_Post;


public:

	PostDetailItem(QObject *parent = 0) : QObject(parent) {};
	virtual ~PostDetailItem() {}

	inline const QString &getAuthor() const				{ return m_Author; }
	inline void			  setAuthor(const QString &s)	{ m_Author = s; }

	inline const QString &getAvatar() const				{ return m_Avatar; }
	inline void			  setAvatar(const QString &s)	{ m_Avatar = s; }

	inline const QString &getTimestamp() const			{ return m_Timestamp; }
	inline void			  setTimestamp(const QString &s){ m_Timestamp = s; }

	inline const QString &getPost() const				{ return m_Post; }
	inline void			  setPost(const QString &s)		{ m_Post = s; }


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void authorChanged();
		void avatarChanged();
		void timestampChanged();
		void postChanged();

};


#endif /* DATAOBJECTS_H_ */
