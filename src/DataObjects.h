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
	Q_PROPERTY( QString urlLastPostRead  READ getUrlLastPostRead    WRITE setUrlLastPostRead    NOTIFY urlLastPostReadChanged)
	Q_PROPERTY( int 	flagType    READ getFlagType    WRITE setFlagType    NOTIFY flagTypeChanged)
	Q_PROPERTY( bool 	read	    READ isRead		    WRITE setRead	     NOTIFY readChanged)
	Q_PROPERTY( int     color       READ getColor       WRITE setColor       NOTIFY colorChanged)

	// ----------------------------------------------------------------------------------------------


private:
	QString m_Title;
	QString m_Category;
	QString m_Timestamp;
	QString m_DetailedTimestamp;
	QString m_LastAuthor;
	QString m_Pages;
	QString m_UrlFirstPage;
	QString m_UrlLastPage;
	QString m_UrlLastPostRead;
	int 	m_FlagType;
	bool 	m_Read;
	int     m_Color;




	// ----------------------------------------------------------------------------------------------

public:
	ThreadListItem(QObject *parent = 0) : QObject(parent), m_FlagType(0), m_Read(false) {}
	virtual ~ThreadListItem() {}



	inline const QString &getTitle() const					{ return m_Title; }
	inline void			  setTitle(const QString &s)		{ m_Title = s; emit titleChanged(); }

	inline const QString &getCategory() const				{ return m_Category; }
	inline void			  setCategory(const QString &c)		{ m_Category = c; emit categoryChanged();}


	inline const QString &getTimestamp() const				{ return m_Timestamp; }
	inline void			  setTimestamp(const QString &c)	{ m_Timestamp = c; emit timestampChanged(); }

	inline const QString &getDetailedTimestamp() const              { return m_DetailedTimestamp; }
	inline void           setDetailedTimestamp(const QString &c)    { m_DetailedTimestamp = c; }

	inline const QString &getLastAuthor() const				{ return m_LastAuthor; }
	inline void			  setLastAuthor(const QString &c)	{ m_LastAuthor = c; emit lastAuthorChanged(); }

	inline const QString &getPages() const					{ return m_Pages; }
	inline void			  setPages(const QString &c)		{ m_Pages = c; emit pagesChanged(); }

	inline const QString &getUrlFirstPage() const			{ return m_UrlFirstPage; }
	inline void			  setUrlFirstPage(const QString &c)	{ m_UrlFirstPage = c; emit urlFirstPageChanged(); }

	inline const QString &getUrlLastPage() const			{ return m_UrlLastPage; }
	inline void			  setUrlLastPage(const QString &c)	{ m_UrlLastPage = c; emit urlLastPageChanged(); }

	inline const QString &getUrlLastPostRead() const			{ return m_UrlLastPostRead; }
	inline void			  setUrlLastPostRead(const QString &c)	{ m_UrlLastPostRead = c; emit urlLastPostReadChanged(); }

	inline int 			  getFlagType() const				{ return m_FlagType; }
	inline void			  setFlagType(int f)				{ m_FlagType = f; emit flagTypeChanged(); }

	inline bool 		  isRead() const					{ return m_Read; }
	inline void 		  setRead(bool r)					{ m_Read = r; emit readChanged(); }

	inline int            getColor() const                  { return m_Color; }
    inline void           setColor(int r)                   { m_Color = r; emit colorChanged(); }

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
		void urlLastPostReadChanged();
		void colorChanged();

};



class PrivateMessageListItem : public QObject {
	Q_OBJECT

	Q_PROPERTY( QString title 		READ getTitle      	WRITE setTitle        NOTIFY titleChanged)
	Q_PROPERTY( QString addressee 	READ getAddressee   WRITE setAddressee    NOTIFY addresseeChanged)
	Q_PROPERTY( QString timestamp 	READ getTimestamp 	WRITE setTimestamp    NOTIFY timestampChanged)
	Q_PROPERTY( QString lastAuthor  READ getLastAuthor  WRITE setLastAuthor   NOTIFY lastAuthorChanged)
	Q_PROPERTY( QString urlFirstPage READ getUrlFirstPage   WRITE setUrlFirstPage   NOTIFY urlFirstPageChanged)
	Q_PROPERTY( QString urlLastPage  READ getUrlLastPage    WRITE setUrlLastPage    NOTIFY urlLastPageChanged)
	Q_PROPERTY( bool 	read	    READ isRead		    WRITE setRead	     NOTIFY readChanged)
	Q_PROPERTY( bool 	addresseeRead	    READ isAddresseeRead		    WRITE setAddresseeRead	     NOTIFY addresseeReadChanged)

	// ----------------------------------------------------------------------------------------------


private:
	QString m_Title;
	QString m_Addressee;
	QString m_Timestamp;
	QString m_LastAuthor;
	QString m_UrlFirstPage;
	QString m_UrlLastPost;
	bool 	m_Read;
	bool	m_AddresseeRead;





	// ----------------------------------------------------------------------------------------------

public:
	PrivateMessageListItem(QObject *parent = 0) : QObject(parent), m_Read(false), m_AddresseeRead(false) {}
	virtual ~PrivateMessageListItem() {}



	inline const QString &getTitle() const					{ return m_Title; }
	inline void			  setTitle(const QString &s)		{ m_Title = s; }

	inline const QString &getAddressee() const				{ return m_Addressee; }
	inline void			  setAddressee(const QString &c)	{ m_Addressee = c; }


	inline const QString &getTimestamp() const				{ return m_Timestamp; }
	inline void			  setTimestamp(const QString &c)	{ m_Timestamp = c; }

	inline const QString &getLastAuthor() const				{ return m_LastAuthor; }
	inline void			  setLastAuthor(const QString &c)	{ m_LastAuthor = c; }

	inline const QString &getUrlFirstPage() const			{ return m_UrlFirstPage; }
	inline void			  setUrlFirstPage(const QString &c)	{ m_UrlFirstPage = c; }

	inline const QString &getUrlLastPage() const			{ return m_UrlLastPost; }
	inline void			  setUrlLastPage(const QString &c)	{ m_UrlLastPost = c; }

	inline bool 		  isRead() const					{ return m_Read; }
	inline void 		  setRead(bool r)					{ m_Read = r; }

	inline bool 		  isAddresseeRead() const			{ return m_AddresseeRead; }
	inline void 		  setAddresseeRead(bool r)			{ m_AddresseeRead = r; }

	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void titleChanged();
		void addresseeChanged();
		void timestampChanged();
		void lastAuthorChanged();
		void urlFirstPageChanged();
		void urlLastPageChanged();
		void readChanged();
		void addresseeReadChanged();

};


class PostDetailItem : public QObject {
	Q_OBJECT;

	Q_PROPERTY( QString author		READ getAuthor      WRITE setAuthor		NOTIFY authorChanged)
	Q_PROPERTY( QString avatar		READ getAvatar      WRITE setAvatar		NOTIFY avatarChanged)
	Q_PROPERTY( QString timestamp	READ getTimestamp	WRITE setTimestamp	NOTIFY timestampChanged)
	Q_PROPERTY( QString post		READ getPost		WRITE setPost		NOTIFY postChanged)
	Q_PROPERTY( QString editUrl		READ getEditUrl		WRITE setEditUrl	NOTIFY editUrlChanged)
	Q_PROPERTY( int		index		READ getIndex		WRITE setIndex		NOTIFY indexChanged)

	// ----------------------------------------------------------------------------------------------

private:
	QString m_Author;
	QString m_Avatar;
	QString m_Timestamp;
	QString m_Post;
	int		m_Index;
	QString m_EditURL;


public:

	PostDetailItem(QObject *parent = 0) : QObject(parent), m_Index(0) {};
	virtual ~PostDetailItem() {}

	inline const QString &getAuthor() const				{ return m_Author; }
	inline void			  setAuthor(const QString &s)	{ m_Author = s; }

	inline const QString &getAvatar() const				{ return m_Avatar; }
	inline void			  setAvatar(const QString &s)	{ m_Avatar = s; }

	inline const QString &getTimestamp() const			{ return m_Timestamp; }
	inline void			  setTimestamp(const QString &s){ m_Timestamp = s; }

	inline const QString &getEditUrl() const			{ return m_EditURL; }
	inline void			  setEditUrl(const QString &s)	{ m_EditURL = s; }

	inline const QString &getPost() const				{ return m_Post; }
	inline       QString &getPost()						{ return m_Post; }
	inline void			  setPost(const QString &s)		{ m_Post = s; }

	inline int			  getIndex() const				{ return m_Index; }
	inline void			  setIndex(int i)				{ m_Index = i; }


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void authorChanged();
		void avatarChanged();
		void timestampChanged();
		void postChanged();
		void indexChanged();
		void editUrlChanged();

};


#endif /* DATAOBJECTS_H_ */
