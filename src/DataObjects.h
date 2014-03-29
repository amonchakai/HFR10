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

struct Flag {
	enum FlagType {
		NONE = 0,
		PARTICIPATE = 1,
		READ = 2,
		FAVORITE = 3
	};
};

class ThreadListItem : public QObject {
	Q_OBJECT

	Q_PROPERTY( QString title 		READ getTitle      	WRITE setTitle        NOTIFY titleChanged)
	Q_PROPERTY( QString category  	READ getCategory   	WRITE setCategory     NOTIFY categoryChanged)
	Q_PROPERTY( QString timestamp 	READ getTimestamp 	WRITE setTimestamp    NOTIFY timestampChanged)
	Q_PROPERTY( QString lastAuthor  READ getLastAuthor  WRITE setLastAuthor   NOTIFY lastAuthorChanged)
	Q_PROPERTY( QString pages		READ getPages       WRITE setPages   	  NOTIFY pagesChanged)
	Q_PROPERTY( QString urlFirstPage READ getUrlFirstPage   WRITE setUrlFirstPage   NOTIFY urlFirstPageChanged)
	Q_PROPERTY( QString urlLastPage  READ getUrlLastPage    WRITE setUrlLastPage    NOTIFY urlLastPageChanged)
	Q_PROPERTY( int 	flagType    READ getFlagType    WRITE setFlagType    NOTIFY flagTypeChanged)
	Q_PROPERTY( bool 	read	    READ isRead		    WRITE setRead	     NOTIFY readChanged)

	// ----------------------------------------------------------------------------------------------


private:
	QString m_Title;
	QString m_Category;
	QString m_Timestamp;
	QString m_LastAuthor;
	QString m_Pages;
	QString m_UrlFirstPage;
	QString m_UrlLastPost;
	int 	m_FlagType;
	bool 	m_Read;





	// ----------------------------------------------------------------------------------------------

public:
	ThreadListItem(QObject *parent = 0) : QObject(parent), m_FlagType(0), m_Read(false) {}
	virtual ~ThreadListItem() {}



	inline const QString &getTitle() const					{ return m_Title; }
	inline void			  setTitle(const QString &s)		{ m_Title = s; }

	inline const QString &getCategory() const				{ return m_Category; }
	inline void			  setCategory(const QString &c)		{ m_Category = c; }


	inline const QString &getTimestamp() const				{ return m_Timestamp; }
	inline void			  setTimestamp(const QString &c)	{ m_Timestamp = c; }

	inline const QString &getLastAuthor() const				{ return m_LastAuthor; }
	inline void			  setLastAuthor(const QString &c)	{ m_LastAuthor = c; }

	inline const QString &getPages() const					{ return m_Pages; }
	inline void			  setPages(const QString &c)		{ m_Pages = c; }

	inline const QString &getUrlFirstPage() const			{ return m_UrlFirstPage; }
	inline void			  setUrlFirstPage(const QString &c)	{ m_UrlFirstPage = c; }

	inline const QString &getUrlLastPage() const			{ return m_UrlLastPost; }
	inline void			  setUrlLastPage(const QString &c)	{ m_UrlLastPost = c; }

	inline int 			  getFlagType() const				{ return m_FlagType; }
	inline void			  setFlagType(int f)				{ m_FlagType = f; }

	inline bool 		  isRead() const					{ return m_Read; }
	inline void 		  setRead(bool r)					{ m_Read = r; }

	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void titleChanged();
		void categoryChanged();
		void timestampChanged();
		void lastAuthorChanged();
		void pagesChanged();
		void urlFirstPageChanged();
		void urlLastPageChanged();
		void flagTypeChanged();
		void readChanged();

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
	inline       QString &getPost()						{ return m_Post; }
	inline void			  setPost(const QString &s)		{ m_Post = s; }


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void authorChanged();
		void avatarChanged();
		void timestampChanged();
		void postChanged();

};


#endif /* DATAOBJECTS_H_ */
