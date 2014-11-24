/*
 * ImageUploaderController.cpp
 *
 *  Created on: 1 mai 2014
 *      Author: pierre
 */

#ifndef IMAGEUPLOADERCONTROLLER_HPP_
#define IMAGEUPLOADERCONTROLLER_HPP_

#include <QtCore/QObject>
#include <QString>
#include <bb/cascades/ProgressIndicator>

class ImageUploaderController : public QObject {
	Q_OBJECT

	Q_PROPERTY( QString mini 			READ getMini      	         NOTIFY miniChanged)
	Q_PROPERTY( QString preview 		READ getPreview              NOTIFY previewChanged)
	Q_PROPERTY( QString original 		READ getOriginal             NOTIFY originalChanged)

private:

	QString 			m_Mini;
	QString 			m_Preview;
	QString 			m_Original;

	bool				m_uploading;

	bb::cascades::ProgressIndicator			*m_ProgressIndicator;

public:
	// ----------------------------------------------------------------------------------------------
	// member functions

	ImageUploaderController(QObject *parent = 0);
	virtual ~ImageUploaderController() {};



	// ----------------------------------------------------------------------------------------------
public Q_SLOTS:
	void upload(const QString &path);
	void checkReply();
	void uploading(qint64, qint64);


	inline const QString &getMini() 	 const 				{ return m_Mini;}
	inline const QString &getPreview()  const 				{ return m_Preview;}
	inline const QString &getOriginal() const 				{ return m_Original;}


	inline void setProgressIndicator(bb::cascades::ProgressIndicator *indic)					{ m_ProgressIndicator = indic; }
	// ----------------------------------------------------------------------------------------------

Q_SIGNALS:
	void complete();

	void miniChanged();
	void previewChanged();
	void originalChanged();


private:

	void connectionTimedOut();
	void parse(const QString &page);
};



#endif /* IMAGEUPLOADERCONTROLLER_CPP_ */
