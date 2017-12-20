/*
 * ProfileController.cpp
 *
 *  Created on: 20 sept. 2015
 *      Author: pierre
 */

#include "ProfileController.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>

#include <bb/system/SystemToast>

#include  "DataObjects.h"
#include  "Globals.h"
#include  "Network/HFRNetworkAccessManager.hpp"

void ProfileController::loadProfile(const QString& url) {
    // -----------------------------------------------------------------------------------------
    // request page

    QNetworkRequest request(DefineConsts::FORUM_URL+url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfig.setPeerVerifyDepth(1);
    sslConfig.setProtocol(QSsl::TlsV1);
    sslConfig.setSslOption(QSsl::SslOptionDisableSessionTickets, true);

    request.setSslConfiguration(sslConfig);

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}


void ProfileController::checkReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                parse(response);
            }
        } else {
            connectionTimedOut();
        }

        reply->deleteLater();
    }
}

void ProfileController::connectionTimedOut() {

    bb::system::SystemToast *toast = new bb::system::SystemToast(this);

    toast->setBody(tr("Connection timed out"));
    toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
    toast->show();

    emit complete();
}


void ProfileController::parse(const QString& page) {

    QRegExp profilCase3("<td class=\"profilCase3\">(.*)</td>");
    profilCase3.setMinimal(true);
    int pos = profilCase3.indexIn(page);
    if(pos != -1) {
        m_Pseudo = profilCase3.cap(1);
        cleanup(m_Pseudo);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_Email = profilCase3.cap(1);
        cleanup(m_Email);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_Birthday = profilCase3.cap(1);
        cleanup(m_Birthday);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_Site = profilCase3.cap(1);
        cleanup(m_Site);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_Gender = profilCase3.cap(1);
        cleanup(m_Gender);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_City = profilCase3.cap(1);
        cleanup(m_City);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_Job = profilCase3.cap(1);
        cleanup(m_Job);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_Hobby = profilCase3.cap(1);
        cleanup(m_Hobby);
        pos += profilCase3.matchedLength();
    }


    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_Status = profilCase3.cap(1);
        cleanup(m_Status);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_PostCount = profilCase3.cap(1);
        cleanup(m_PostCount);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_RegistrationTimestamp = profilCase3.cap(1);
        cleanup(m_RegistrationTimestamp);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_LastMessageTimestamp = profilCase3.cap(1);
        cleanup(m_LastMessageTimestamp);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_Citation = profilCase3.cap(1);
        cleanup(m_Citation);
        pos += profilCase3.matchedLength();
    }

    pos = profilCase3.indexIn(page, pos);
    if(pos != -1) {
        m_Signature = profilCase3.cap(1);
        cleanup(m_Signature);
        pos += profilCase3.matchedLength();
    }

    QRegExp transaction("(/transactions_reports.php[^\"]+)\"");
    if(transaction.indexIn(page, pos) != -1) {
        m_TransationsVideoGamesUrl = transaction.cap(1);
        pos += transaction.matchedLength();
    }

    m_NumberSalesVG = "(0)";
    QRegExp nbSales("Transactions .([0-9]+).");
    if((pos = nbSales.indexIn(page, pos)) != -1) {
        m_NumberSalesVG = "(" + nbSales.cap(1) + ")";
        pos += nbSales.matchedLength();
    }

    m_NumberSales = "(0)";
    if((pos = nbSales.indexIn(page, pos)) != -1) {
        m_NumberSales = "(" + nbSales.cap(1) + ")";
        pos += nbSales.matchedLength();
    }

    m_TransationsVideoGamesUrl.replace("&amp;", "&");

    m_TransationsUrl = m_TransationsVideoGamesUrl;
    m_TransationsUrl.replace("cat=5", "cat=6");

    emit complete();

}


void ProfileController::cleanup(QString &str) {
    str.replace("\n", "");
    str.replace("\r", "");
    str.replace("<i>", "");
    str.replace("</i>", "");
    str.replace("&nbsp;", " ");

    QRegExp multipleSp("  ");
    while(multipleSp.indexIn(str) != -1) {
        str.replace("  ", " ");
    }

}


void ProfileController::loadFeedback(const QString& url) {

    // -----------------------------------------------------------------------------------------
    // request page

    QNetworkRequest request(DefineConsts::FORUM_URL+url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfig.setPeerVerifyDepth(1);
    sslConfig.setProtocol(QSsl::TlsV1);
    sslConfig.setSslOption(QSsl::SslOptionDisableSessionTickets, true);

    request.setSslConfiguration(sslConfig);

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyFeedback()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void ProfileController::checkReplyFeedback() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
                QFile file(directory + "/cache.txt");
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(buffer);
                    file.close();
                }

                parseFeedback(response);
            }
        } else {
            connectionTimedOut();
        }

        reply->deleteLater();
    }
}

void ProfileController::parseFeedback(const QString& page) {
    QRegExp feedback("<a href=\"[^\"]+\" target=\"_blank\">(.*)</a></td><td class=\"col2\">(.*)</td><td class=\"col3 .*\">(.*)</td><td>([0-9][0-9]/[0-9][0-9]/[0-9][0-9][0-9][0-9])</td><td class=\"col4\">(.*)</a></td>");
    feedback.setMinimal(true);


    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if (dataModel) {
        dataModel->clear();
    }

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    int pos = 0;
    while((pos = feedback.indexIn(page, pos)) != -1) {

        ReviewListItem *item = new ReviewListItem;
        item->setFrom(feedback.cap(1));
        item->setRole(feedback.cap(2));
        item->setDate(feedback.cap(4));
        item->setAdvice(feedback.cap(3));
        item->setReview(feedback.cap(5));
        item->setTimestamp(QDateTime::fromString(feedback.cap(3), "dd/MM/yyyy").toMSecsSinceEpoch());

        QRegExp cleanup("<a href=\"[^\"]+\" target=\"_blank\">(.*)");
        if(cleanup.indexIn(feedback.cap(5)) != -1) {
            item->setReview(cleanup.cap(1));
        }

        dataModel->insert(item);

        pos += feedback.matchedLength();
    }
}

void ProfileController::blacklistUser() {
    QString pseudo;
    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
    QFile file(directory + "/blacklist.txt");
    if (file.open(QIODevice::Append)) {
        QTextStream stream(&file);

        QString pseudo = m_Pseudo;
        pseudo.replace("\t", "");
        pseudo = pseudo.mid(1);

        stream << pseudo << "\n";

        file.close();
    }


    bb::system::SystemToast *toast = new bb::system::SystemToast(this);

    toast->setBody(tr("User was added to the blacklist"));
    toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
    toast->show();
}




