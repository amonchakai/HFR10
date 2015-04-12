/*
 * PostMessageController.cpp
 *
 *  Created on: 30 mars 2014
 *      Author: PierreL
 */

#include "PostMessageController.hpp"


#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>
#include <bb/system/SystemToast>
#include <bb/cascades/Option>

#include "Globals.h"
#include "Network/HFRNetworkAccessManager.hpp"
#include "LoginController.hpp"
#include "Easter.hpp"

#include <bb/system/SystemToast>

void PostMessageController::postMessage(const QString &message) {
	postMessage(m_HashCheck,
				m_PostID,
				m_CatID,
				m_Page,
				m_Pseudo,
				message,
				m_ThreadTitle,
				m_AddSignature);
}

void PostMessageController::postMessage(const QString &hashCheck,
										const QString &postID,
										const QString &catID,
										const QString &page,
										const QString &pseudo,
										const QString &message,
										const QString &threadTitle,
										bool signature) {

    EASTER1


	if(m_MessageBeingPosted) {
		return;
	}

	const QUrl url(DefineConsts::FORUM_URL + "/bddpost.php?config=hfr.inc");


	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QUrl params;
	params.addQueryItem("hash_check", hashCheck);
	params.addQueryItem("post", postID);
	params.addQueryItem("cat", catID);
	params.addQueryItem("verifrequet", "1100");
	params.addQueryItem("MsgIcon", "20");
	params.addQueryItem("page", page);
	params.addQueryItem("pseudo", pseudo);
	params.addEncodedQueryItem("content_form", QUrl::toPercentEncoding(message));
	params.addQueryItem("sujet", threadTitle);
	params.addQueryItem("signature", signature ? "1" : "0");

	m_MessageBeingPosted = true;
	QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}

void PostMessageController::postNewPrivateMessage(const QString &hashCheck
												, const QString &pseudo
												, bool			signature
												, const QString &caption
												, const QString &dest
												, const QString &message) {
	const QUrl url(DefineConsts::FORUM_URL + "/bddpost.php?config=hfr.inc");

	EASTER2

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QUrl params;
	params.addQueryItem("hash_check", hashCheck);
	params.addQueryItem("cat", "prive");
	params.addQueryItem("verifrequet", "1100");
	params.addQueryItem("MsgIcon", "20");
	params.addQueryItem("pseudo", pseudo);
	params.addEncodedQueryItem("content_form", QUrl::toPercentEncoding(message));
	params.addQueryItem("dest", dest);
	params.addQueryItem("sujet", caption);
	params.addQueryItem("signature", signature ? "1" : "0");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}

void PostMessageController::postNewTopic(         const QString &caption
                                                , const QString &subcat
                                                , const QString &message) {
    const QUrl url(DefineConsts::FORUM_URL + "/bddpost.php?config=hfr.inc");


    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl params;
    params.addQueryItem("hash_check", m_HashCheck);
    params.addQueryItem("cat", m_CatID);
    params.addQueryItem("subcat", subcat);
    params.addQueryItem("verifrequet", "1100");
    params.addQueryItem("MsgIcon", "20");
    params.addQueryItem("pseudo", m_Pseudo);
    params.addEncodedQueryItem("content_form", QUrl::toPercentEncoding(message));
    params.addQueryItem("sujet", caption);
    params.addQueryItem("signature", "1");


    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}


void PostMessageController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);

				errorMessage(response);

			}
		} else {
			connectionTimedOut();
		}

	    reply->deleteLater();
	}

	if (response.trimmed().isEmpty()) {
		connectionTimedOut();
    }

	m_MessageBeingPosted = false;
	emit complete();
}

void PostMessageController::errorMessage(const QString &page) {

	QRegExp message("Afin de prevenir les tentatives de flood");
	message.setCaseSensitivity(Qt::CaseSensitive);
	message.setMinimal(true);

	QString error;

	if(message.indexIn(page, 0) != -1) {
		error = QString(tr("Too many consecutive replies, please wait 10 minutes"));
	}

	if(error == "")
		return;

	bb::system::SystemToast *toast = new bb::system::SystemToast(this);

	toast->setBody(error);
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();

}

void PostMessageController::getSubCatsInfo(const QString &url_str) {

    const QUrl url(DefineConsts::FORUM_URL + "/hfr/" + url_str.mid(0, url_str.length()-4) + "/nouveau_sujet.htm");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getDropdownData()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

}

void PostMessageController::getDropdownData() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QRegExp andAmp("&amp;");
    QRegExp quote("&#034;");
    QRegExp euro("&euro;");
    QRegExp inf("&lt;");
    QRegExp sup("&gt;");

    m_DropdownSubCatPicker->removeAll();

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QRegExp dropDown("<option value=\"([0-9]+)\" >([^<]+)</option>");
                int pos = 0;
                while((pos = dropDown.indexIn(response, pos)) != -1) {
                    QString title = dropDown.cap(2);
                    title.replace(andAmp, "&");
                    title.replace(quote, "\"");
                    title.replace(euro, "e");
                    title.replace(inf, "<");
                    title.replace(sup, ">");

                    bb::cascades::Option *option = bb::cascades::Option::create().text(title);
                    option->setValue(dropDown.cap(1));

                    m_DropdownSubCatPicker->add(option);

                    pos += dropDown.matchedLength();
                }

                QRegExp pseudo("name=\"pseudo\" value=\"([^\"]+)\"");
                if(pseudo.indexIn(response) != -1)
                    m_Pseudo = pseudo.cap(1);


                QRegExp hash("name=\"hash_check\" value=\"([^\"]+)\"");
                if(hash.indexIn(response) != -1)
                    m_HashCheck = hash.cap(1);

                QRegExp cat("name=\"cat\" id=\"catid\" value=\"([^\"]+)\"");
                if(cat.indexIn(response) != -1)
                    m_CatID = cat.cap(1);

                emit messageLoaded("");


            }
        } else {
            connectionTimedOut();
        }

        reply->deleteLater();
    }
}


void PostMessageController::getEditMessage(const QString &messageUrl) {
	// list green + yellow flags
	const QUrl url(DefineConsts::FORUM_URL + messageUrl);

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkGetMessageReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}



void PostMessageController::getQuotedMessages(const QString &url_str) {
	m_Message = "";
	m_NBMessages = 0;

	QRegExp args("&cat=[prive0-9]+&post=[0-9]+&numrep=[0-9]+&");
	args.setCaseSensitivity(Qt::CaseSensitive);
	args.setMinimal(true);

	int pos = 0;
	while((pos = args.indexIn(url_str, pos)) != -1) {
		pos += args.matchedLength();
		++m_NBMessages;
	}

	// ---------------------------------------------------------
	// get the messages

	int lastPos = 0;
	pos = 0;
	while((pos = args.indexIn(url_str, lastPos)) != -1) {


		const QUrl url(DefineConsts::FORUM_URL + "/message.php?config=hfr.inc" + url_str.mid(lastPos, args.matchedLength()-1));

		QNetworkRequest request(url);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


		QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
		bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkQuoteMessageReply()));
		Q_ASSERT(ok);
		Q_UNUSED(ok);

		lastPos = pos+args.matchedLength();
	}

}

void PostMessageController::checkQuoteMessageReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				parseQuotedMessage(response);
			}
		} else {
			connectionTimedOut();
		}

		reply->deleteLater();
	}
}

void PostMessageController::checkGetMessageReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);

				parseEditMessage(response);

			}
		} else {
			connectionTimedOut();
		}

		reply->deleteLater();
	}

}


void PostMessageController::parseEditMessage(const QString &editpage) {
	QRegExp getMessageRegexp("<textarea cols=\"[0-9]+\" rows=\"[0-9]+\" class=\"contenu\" name=\"content_form\" id=\"content_form\" accesskey=\"c\">(.+)</textarea><input type=\"hidden\" name=\"wysiwyg\" value=\"0\" />");

	QRegExp andAmp("&amp;");
	QRegExp quote("&#034;");
	QRegExp euro("&euro;");
	QRegExp inf("&lt;");
	QRegExp sup("&gt;");

	QString message;
	if(getMessageRegexp.indexIn(editpage, 0) != -1) {
		message = getMessageRegexp.cap(1);

		message.replace(andAmp, "&");
		message.replace(quote, "\"");
		message.replace(euro, "e");
		message.replace(inf, "<");
		message.replace(sup, ">");
	}

	QRegExp postData(QString("<input type=\"hidden\" name=\"hash_check\" value=\"([0-9a-z]+)\" />")
							+ 	".*<input type=\"hidden\" name=\"parents\" value=\"([0-9]*)\" />"
							+ 	".*<input type=\"hidden\" name=\"post\" value=\"([0-9]+)\" />"
							+ 	".*<input type=\"hidden\" name=\"cat\" id=\"catid\" value=\"([0-9a-z]+)\" />"
							+ 	".*<input type=\"hidden\" name=\"numreponse\" value=\"([0-9a-z]+)\" />"
							+	".*<input type=\"hidden\" name=\"page\" value=\"([0-9]+)\" />"
							+	".*<input type=\"hidden\" name=\"sujet\" value=\"(.+)\" />"
							+	".*<input maxlength=\"[0-9]+\" accesskey=\"p\" size=\"[0-9]+\" name=\"pseudo\" value=\"(.+)\""
	);


	postData.setCaseSensitivity(Qt::CaseSensitive);
	postData.setMinimal(true);

	if(postData.indexIn(editpage, 0) != -1) {
		m_HashCheck = postData.cap(1);
		m_Parent = postData.cap(2);
		m_PostID = postData.cap(3);
		m_CatID = postData.cap(4);
		m_ResponseID = postData.cap(5);
		m_Page = postData.cap(6);
		m_ThreadTitle = postData.cap(7);
		m_AddSignature = false;
		m_Pseudo = postData.cap(8);

	}

	emit messageLoaded(message);

}

void PostMessageController::parseQuotedMessage(const QString &editpage) {
	QRegExp getMessageRegexp("<textarea cols=\"[0-9]+\" rows=\"[0-9]+\" class=\"contenu\" name=\"content_form\" id=\"content_form\" accesskey=\"c\">(.+)</textarea><input type=\"hidden\" name=\"wysiwyg\" value=\"0\" />");

	QRegExp andAmp("&amp;");
	QRegExp quote("&#034;");
	QRegExp euro("&euro;");
	QRegExp inf("&lt;");
	QRegExp sup("&gt;");

	QString message;
	if(getMessageRegexp.indexIn(editpage, 0) != -1) {
		message = getMessageRegexp.cap(1);

		message.replace(andAmp, "&");
		message.replace(quote, "\"");
		message.replace(euro, "e");
		message.replace(inf, "<");
		message.replace(sup, ">");
	}

	QRegExp postData(QString("<input type=\"hidden\" name=\"hash_check\" value=\"([0-9a-z]+)\" />")
							+ 	".*<input type=\"hidden\" name=\"parents\" value=\"([0-9]*)\" />"
							+ 	".*<input type=\"hidden\" name=\"post\" value=\"([0-9]+)\" />"
							+ 	".*<input type=\"hidden\" name=\"cat\" id=\"catid\" value=\"([0-9a-z]+)\" />"
							+ 	".*<input type=\"hidden\" name=\"numrep\" value=\"([0-9a-z]+)\" />"
							+	".*<input type=\"hidden\" name=\"page\" value=\"([0-9]+)\" />"
							+	".*<input type=\"hidden\" name=\"sujet\" value=\"(.+)\" />"
							+	".*<input maxlength=\"[0-9]+\" accesskey=\"p\" size=\"[0-9]+\" name=\"pseudo\" value=\"(.+)\""
	);


	postData.setCaseSensitivity(Qt::CaseSensitive);
	postData.setMinimal(true);

	m_MessageMutex.lockForWrite();

	if(postData.indexIn(editpage, 0) != -1) {
		m_HashCheck = postData.cap(1);
		m_Parent = postData.cap(2);
		m_PostID = postData.cap(3);
		m_CatID = postData.cap(4);
		m_ResponseID = postData.cap(5);
		m_Page = postData.cap(6);
		m_ThreadTitle = postData.cap(7);
		m_AddSignature = false;
		m_Pseudo = postData.cap(8);

	}

	m_Message += message +  "\n\n" ;
	--m_NBMessages;

	if(m_NBMessages == 0) {
		emit messageLoaded(m_Message);
	}

	m_MessageMutex.unlock();

}


void PostMessageController::postEdit(const QString &message) {
	const QUrl url(DefineConsts::FORUM_URL + "/bdd.php?config=hfr.inc");


	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString copyMessage = message;
    copyMessage.replace("+", "%2");

	QUrl params;
	params.addQueryItem("hash_check", m_HashCheck);
	params.addQueryItem("numreponse", m_ResponseID);
	params.addQueryItem("post", m_PostID);
	params.addQueryItem("cat", m_CatID);
	params.addQueryItem("verifrequet", "1100");
	params.addQueryItem("parents", m_Parent);
	params.addQueryItem("pseudo", m_Pseudo);
	params.addEncodedQueryItem("content_form", QUrl::toPercentEncoding(message));
	params.addQueryItem("sujet", m_ThreadTitle);
	params.addQueryItem("signature", "1");

	QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}


void PostMessageController::connectionTimedOut() {
	bb::system::SystemToast *toast = new bb::system::SystemToast(this);

	toast->setBody(tr("Connection timed out"));
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();
}
