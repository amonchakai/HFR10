/*
 * ShowThreadController.hpp
 *
 *  Created on: 20 mars 2014
 *      Author: PierreL
 */

#ifndef SHOWTHREADCONTROLLER_HPP_
#define SHOWTHREADCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/WebView>
#include <bb/cascades/ListView>
#include <QList>

struct PostDetailItem;

class ShowThreadController : public QObject {
	Q_OBJECT;

	Q_PROPERTY( QString hashCheck 	READ getHashCheck   )
	Q_PROPERTY( QString postID 		READ getPostID      )
	Q_PROPERTY( QString catID  		READ getCatID   	)
	Q_PROPERTY( QString pages 		READ getPage 		)
	Q_PROPERTY( QString pseudo  	READ getPseudo		)
	Q_PROPERTY( QString title  		READ getThreadTitle )
	Q_PROPERTY( bool	sign		READ isAddSignature )

	Q_PROPERTY( QString survey 		READ getSurvey   )
	Q_PROPERTY( bool    emptySurvey READ isEmptySurvey   )
	Q_PROPERTY( bool    actionSurvey READ isActionSurvey   )

	private:

		bb::cascades::WebView 				*m_WebView;
		bb::cascades::ListView              *m_ListView;
		QList<PostDetailItem*>				*m_Datas;
		mutable QReadWriteLock	 			 m_EditData;

		QString								 m_UrlNextPage;
		QString								 m_UrlPrevPage;
		QString								 m_UrlFirstPage;
		QString								 m_UrlLastPage;
		QString								 m_Url;

		QString								 m_PostID;
		QString								 m_CatID;
		QString								 m_Page;
		QString								 m_Pseudo;
		QString								 m_ThreadTitle;
		QString								 m_HashCheck;
		bool								 m_AddSignature;

		bool 								 m_ScrollAtLocation;
		int									 m_NbWebviewLoaded;

		QString								 m_Survey;
		bool                                 m_ActionSurvey;

	// ----------------------------------------------------------------------------------------------
	public:
		ShowThreadController(QObject *parent = 0);
		virtual ~ShowThreadController() {};


		void updateView();
		bool isEmptySurvey() const      { return m_Survey.isEmpty(); }
		bool isActionSurvey() const     { return m_ActionSurvey; }

	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		void showThread(const QString &url);
		inline void setWebView	   (QObject *view) 		{m_WebView = dynamic_cast<bb::cascades::WebView*>(view); }
		void setActionListView     (QObject *list);
		void checkReply();
		void checkSurveyReply();

		void addToFavorite(int responseID);
		void deletePost(int messageID);
		void checkSuccessAddAddFavorite();
		void checkSuccessDeletePost();
		void vote(const QString &choices);
		void addBookmark(int messageID);

		void doAction(int code);
		void nextPage();
		void prevPage();
		void firstPage();
		void lastPage(bool bas = false);
		void scrollToItem();

		QString getEditUrl(int messageID) const;
		QString getMessageAuthor(int messageID) const;


		const QString &getHashCheck() 	const 	{ return m_HashCheck; }
		const QString &getPostID() 		const 	{ return m_PostID; }
		const QString &getCatID() 		const	{ return m_CatID; }
		const QString &getPage() 		const	{ return m_Page; }
		const QString &getPseudo() 		const	{ return m_Pseudo; }
		const QString &getThreadTitle() const	{ return m_ThreadTitle; }
		const QString &getSurvey()		const 	{ return m_Survey; }
		bool 		   isAddSignature()const	{ return m_AddSignature; }

		bool		   isLastPage() const		{ return m_UrlLastPage == ""; }



		void  notifyItemLoaded();

	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete();
		void surveyUpdated();



	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page);
		void parseSurvey(const QString &page);
		void parsePost(const QString &postIdex, const QString &author, const QString &post);
		void parseDataForReply(const QString &page);
		void cleanupPost(QString &post, int messageID);
		void connectionTimedOut();
};


#endif /* SHOWTHREADCONTROLLER_HPP_ */
