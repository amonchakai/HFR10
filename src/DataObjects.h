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

class ReviewListItem : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString review       READ getReview       WRITE setReview        NOTIFY reviewChanged)
    Q_PROPERTY( QString from         READ getFrom         WRITE setFrom          NOTIFY fromChanged)
    Q_PROPERTY( QString role         READ getRole         WRITE setRole          NOTIFY roleChanged)
    Q_PROPERTY( QString advice       READ getAdvice       WRITE setAdvice        NOTIFY adviceChanged)
    Q_PROPERTY( QString date         READ getDate         WRITE setDate          NOTIFY dateChanged)
    Q_PROPERTY( qint64 timestamp     READ getTimestamp    WRITE setTimestamp     NOTIFY timestampChanged)

private:
    QString m_Review;
    QString m_From;
    QString m_Role;
    QString m_Advice;
    QString m_Date;
    qint64  m_Timestamp;

public:
    inline const QString &getReview() const                { return m_Review; }
    inline void           setReview(const QString &s)      { m_Review = s; emit reviewChanged(); }


    inline const QString &getFrom() const                  { return m_From; }
    inline void           setFrom(const QString &s)        { m_From = s; emit fromChanged(); }

    inline const QString &getRole() const                  { return m_Role; }
    inline void           setRole(const QString &s)        { m_Role = s; emit roleChanged(); }

    inline const QString &getAdvice() const                { return m_Advice; }
    inline void           setAdvice(const QString &s)      { m_Advice = s; emit adviceChanged(); }

    inline const QString &getDate() const                  { return m_Date; }
    inline void           setDate(const QString &s)        { m_Date = s; emit dateChanged(); }

    inline const qint64   getTimestamp() const             { return m_Timestamp; }
    inline void           setTimestamp(qint64 s)           { m_Timestamp = s; emit timestampChanged(); }

// ----------------------------------------------------------------------------------------------
Q_SIGNALS:

    void reviewChanged();
    void fromChanged();
    void roleChanged();
    void adviceChanged();
    void dateChanged();
    void timestampChanged();


};

class ThreadListItem : public QObject {
	Q_OBJECT

	Q_PROPERTY( QString title 		READ getTitle      	WRITE setTitle        NOTIFY titleChanged)
	Q_PROPERTY( QString category  	READ getCategory   	WRITE setCategory     NOTIFY categoryChanged)
	Q_PROPERTY( int     groupKey    READ getGroupKey    WRITE setGroupKey     NOTIFY groupKeyChanged)
	Q_PROPERTY( QString timestamp 	READ getTimestamp 	WRITE setTimestamp    NOTIFY timestampChanged)
	Q_PROPERTY( QString lastAuthor  READ getLastAuthor  WRITE setLastAuthor   NOTIFY lastAuthorChanged)
	Q_PROPERTY( QString pages		READ getPages       WRITE setPages   	  NOTIFY pagesChanged)
	Q_PROPERTY( QString urlFirstPage READ getUrlFirstPage   WRITE setUrlFirstPage   NOTIFY urlFirstPageChanged)
	Q_PROPERTY( QString urlLastPage  READ getUrlLastPage    WRITE setUrlLastPage    NOTIFY urlLastPageChanged)
	Q_PROPERTY( QString urlLastPostRead  READ getUrlLastPostRead    WRITE setUrlLastPostRead    NOTIFY urlLastPostReadChanged)
	Q_PROPERTY( int 	flagType    READ getFlagType    WRITE setFlagType    NOTIFY flagTypeChanged)
	Q_PROPERTY( bool 	read	    READ isRead		    WRITE setRead	     NOTIFY readChanged)
	Q_PROPERTY( bool    sticky      READ isSticky       WRITE setSticky      NOTIFY stickyChanged)
	Q_PROPERTY( int     color       READ getColor       WRITE setColor       NOTIFY colorChanged)

	// ----------------------------------------------------------------------------------------------


private:
	QString m_Title;
	QString m_Category;
	int     m_GroupKey;
	QString m_Timestamp;
	QString m_DetailedTimestamp;
	QString m_LastAuthor;
	QString m_Pages;
	QString m_UrlFirstPage;
	QString m_UrlLastPage;
	QString m_UrlLastPostRead;
	int 	m_FlagType;
	bool 	m_Read;
	bool    m_Sticky;
	int     m_Color;




	// ----------------------------------------------------------------------------------------------

public:
	ThreadListItem(QObject *parent = 0) : QObject(parent), m_FlagType(0), m_Read(false) {}
	virtual ~ThreadListItem() {}


	inline const QString &getTitle() const					{ return m_Title; }
	inline void			  setTitle(const QString &s)		{ m_Title = s; emit titleChanged(); }

	inline const QString &getCategory() const				{ return m_Category; }
	inline void			  setCategory(const QString &c)		{ m_Category = c; emit categoryChanged();}

	inline int            getGroupKey() const               { return m_GroupKey; }
    inline void           setGroupKey(int c)                { m_GroupKey = c; emit groupKeyChanged();}

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

    inline bool           isSticky() const                    { return m_Sticky; }
    inline void           setSticky(bool r)                   { m_Sticky = r; emit stickyChanged(); }

	inline int            getColor() const                  { return m_Color; }
    inline void           setColor(int r)                   { m_Color = r; emit colorChanged(); }

	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void titleChanged();
		void groupKeyChanged();
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
		void stickyChanged();

};

inline QDataStream &operator<<(QDataStream& stream, const ThreadListItem& threadListItem) {
    stream << threadListItem.getTitle();
    stream << threadListItem.getCategory();
    stream << threadListItem.getTimestamp();
    stream << threadListItem.getDetailedTimestamp();
    stream << threadListItem.getLastAuthor();
    stream << threadListItem.getPages();
    stream << threadListItem.getUrlFirstPage();
    stream << threadListItem.getUrlLastPage();
    stream << threadListItem.getUrlLastPostRead();
    stream << threadListItem.getFlagType();
    stream << (int) threadListItem.isRead();
    stream << threadListItem.getColor();
    stream << threadListItem.getGroupKey();

    return stream;
}


inline QDataStream &operator>>(QDataStream& stream, ThreadListItem& threadListItem) {
    QString tempStr;
    int tempInt;

    stream >> tempStr; threadListItem.setTitle(tempStr);
    stream >> tempStr; threadListItem.setCategory(tempStr);
    stream >> tempStr; threadListItem.setTimestamp(tempStr);
    stream >> tempStr; threadListItem.setDetailedTimestamp(tempStr);
    stream >> tempStr; threadListItem.setLastAuthor(tempStr);
    stream >> tempStr; threadListItem.setPages(tempStr);
    stream >> tempStr; threadListItem.setUrlFirstPage(tempStr);
    stream >> tempStr; threadListItem.setUrlLastPage(tempStr);
    stream >> tempStr; threadListItem.setUrlLastPostRead(tempStr);
    stream >> tempInt; threadListItem.setFlagType(tempInt);
    stream >> tempInt; threadListItem.setRead(tempInt);
    stream >> tempInt; threadListItem.setColor(tempInt);
    stream >> tempInt; threadListItem.setGroupKey(tempInt);

    return stream;
}


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

inline QDataStream &operator<<(QDataStream& stream, const PrivateMessageListItem& threadListItem) {
    stream << threadListItem.getTitle();
    stream << threadListItem.getAddressee();
    stream << threadListItem.getTimestamp();
    stream << threadListItem.getLastAuthor();
    stream << threadListItem.getUrlFirstPage();
    stream << threadListItem.getUrlLastPage();
    stream << (int) threadListItem.isRead();
    stream << (int) threadListItem.isAddresseeRead();

    return stream;
}


inline QDataStream &operator>>(QDataStream& stream, PrivateMessageListItem& threadListItem) {
    QString tempStr;
    int tempInt;

    stream >> tempStr; threadListItem.setTitle(tempStr);
    stream >> tempStr; threadListItem.setAddressee(tempStr);
    stream >> tempStr; threadListItem.setTimestamp(tempStr);
    stream >> tempStr; threadListItem.setLastAuthor(tempStr);
    stream >> tempStr; threadListItem.setUrlFirstPage(tempStr);
    stream >> tempStr; threadListItem.setUrlLastPage(tempStr);
    stream >> tempInt; threadListItem.setRead(tempInt);
    stream >> tempInt; threadListItem.setAddresseeRead(tempInt);

    return stream;
}



class PostDetailItem : public QObject {
	Q_OBJECT;

	Q_PROPERTY( QString author		READ getAuthor      WRITE setAuthor		NOTIFY authorChanged)
	Q_PROPERTY( QString avatar		READ getAvatar      WRITE setAvatar		NOTIFY avatarChanged)
	Q_PROPERTY( QString timestamp	READ getTimestamp	WRITE setTimestamp	NOTIFY timestampChanged)
	Q_PROPERTY( QString post		READ getPost		WRITE setPost		NOTIFY postChanged)
	Q_PROPERTY( QString editUrl		READ getEditUrl		WRITE setEditUrl	NOTIFY editUrlChanged)
	Q_PROPERTY( QString profileUrl  READ getProfileUrl  WRITE setProfileUrl NOTIFY profileUrlChanged)
	Q_PROPERTY( int		index		READ getIndex		WRITE setIndex		NOTIFY indexChanged)

	// ----------------------------------------------------------------------------------------------

private:
	QString m_Author;
	QString m_Avatar;
	QString m_Timestamp;
	QString m_Post;
	int		m_Index;
	QString m_EditURL;
	QString m_ProfileUrl;
	QString m_NotFilteredThreadUrl;


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

	inline const QString &getProfileUrl() const            { return m_ProfileUrl; }
    inline void           setProfileUrl(const QString &s)  { m_ProfileUrl = s; }

    inline const QString &getNotFilteredUrl() const            { return m_NotFilteredThreadUrl; }
    inline void           setNotFilteredUrl(const QString &s)  { m_NotFilteredThreadUrl = s; }

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
		void profileUrlChanged();

};


// ---------------------------------------------------------------------------------------------
// Stickers

class Emoticon : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString localUrl        READ getLocalUrl      WRITE setLocalUrl        NOTIFY localUrlChanged)
    Q_PROPERTY( QString distlUrl        READ getDistUrl       WRITE setDistUrl         NOTIFY distUrlChanged)
    Q_PROPERTY( QString tag             READ getTag           WRITE setTag             NOTIFY tagChanged)

private:
    QString m_LocalUrl;
    QString m_DistUrl;
    QString m_Tag;


public:
    Emoticon(QObject *parent = 0) : QObject(parent) {}
    virtual ~Emoticon() {}

    inline const QString &getLocalUrl() const                    { return m_LocalUrl; }
    inline void           setLocalUrl(const QString &s)          { m_LocalUrl = s; emit localUrlChanged(); }

    inline const QString &getDistUrl() const                     { return m_DistUrl; }
    inline void           setDistUrl(const QString &s)           { m_DistUrl = s; emit localUrlChanged(); }

    inline const QString &getTag() const                         { return m_Tag; }
    inline void           setTag(const QString &c)               { m_Tag = c; emit tagChanged();}

    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void localUrlChanged();
        void distUrlChanged();
        void tagChanged();

};


// ---------------------------------------------------------------------------------------------
// ActionComposer

class ActionComposerItem : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString image           READ getImage      WRITE setImage        NOTIFY imageChanged)
    Q_PROPERTY( QString category        READ getCategory   WRITE setCategory     NOTIFY categoryChanged)
    Q_PROPERTY( QString caption         READ getCaption    WRITE setCaption      NOTIFY captionChanged)
    Q_PROPERTY( int action              READ getAction     WRITE setAction       NOTIFY actionChanged)

private:
    QString m_Image;
    QString m_Category;
    QString m_Caption;
    int m_Action;


public:
    ActionComposerItem(QObject *parent = 0) : QObject(parent), m_Action(0) {}
    virtual ~ActionComposerItem() {}

    inline const QString &getImage() const                      { return m_Image; }
    inline void           setImage(const QString &s)            { m_Image = s; emit imageChanged(); }

    inline const QString &getCategory() const                   { return m_Category; }
    inline void           setCategory(const QString &s)         { m_Category = s; emit categoryChanged(); }

    inline const QString &getCaption() const                    { return m_Caption; }
    inline void           setCaption(const QString &c)          { m_Caption = c; emit captionChanged();}

    inline int            getAction() const                     { return m_Action; }
    inline void           setAction(int c)                      { m_Action = c; emit actionChanged();}

    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void imageChanged();
        void categoryChanged();
        void captionChanged();
        void actionChanged();

};



#endif /* DATAOBJECTS_H_ */

