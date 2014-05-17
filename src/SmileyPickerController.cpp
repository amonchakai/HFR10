/*
 * SmileyPickerController.cpp
 *
 *  Created on: 5 avr. 2014
 *      Author: pierre
 */
#include "SmileyPickerController.hpp"



#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>

#include  "Globals.h"
#include  "Network/HFRNetworkAccessManager.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>


static const QString defaultSmiley = QString("<table>")
										+ "<tr>"
											+"<td><img src=\"smilies/pfff.gif\" width=\"40%\" height=\"40%\" alt=\":pfff:\" title=\":pfff:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smile.gif\" width=\"40%\" height=\"40%\" alt=\":)\" title=\":)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"mad.gif\" width=\"40%\" height=\"40%\" alt=\":fou:\" title=\":fou:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/love.gif\" width=\"40%\" height=\"40%\" alt=\":love:\" title=\":love:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"ddr555.gif\" width=\"40%\" height=\"40%\" alt=\"[:ddr555]\" title=\"[:ddr555]\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/ange.gif\" width=\"40%\" height=\"40%\" alt=\":ange:\" title=\":ange:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
										+ "<tr>"
											+"<td><img src=\"frown.gif\" width=\"40%\" height=\"40%\" alt=\":(\" title=\":(\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"confused.gif\" width=\"40%\" height=\"40%\" alt=\":??:\" title=\":??:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/jap.gif\" width=\"40%\" height=\"40%\" alt=\":jap:\" title=\":jap:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/heink.gif\" width=\"40%\" height=\"40%\" alt=\":heink:\" title=\":heink:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"biggrin.gif\" width=\"40%\" height=\"40%\" alt=\":D\" title=\":D\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"tongue.gif\" width=\"40%\" height=\"40%\" alt=\":p\" title=\":p\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
										+ "<tr>"
											+"<td><img src=\"smilies/lol.gif\" width=\"40%\" height=\"40%\" alt=\":lol:\" title=\":lol:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/cry.gif\" width=\"40%\" height=\"40%\" alt=\":cry:\" title=\":cry:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/sleep.gif\" width=\"40%\" height=\"40%\" alt=\":sleep:\" title=\":sleep:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/sweat.gif\" width=\"40%\" height=\"40%\" alt=\":sweat:\" title=\":sweat:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"wink.gif\" width=\"40%\" height=\"40%\" alt=\";)\" title=\";)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/na.gif\" width=\"40%\" height=\"40%\" alt=\":na:\" title=\":na:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
										+ "<tr>"
											+"<td><img src=\"smilies/wahoo.gif\" width=\"40%\" height=\"40%\" alt=\":wahoo:\" title=\":wahoo:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/bounce.gif\" width=\"40%\" height=\"40%\" alt=\":bounce:\" title=\":bounce:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/ouch.gif\" width=\"40%\" height=\"40%\" alt=\":ouch:\" title=\":ouch:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/sarcastic.gif\" width=\"40%\" height=\"40%\" alt=\":sarcastic:\" title=\":sarcastic:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/kaola.gif\" width=\"40%\" height=\"40%\" alt=\":kaola:\" title=\":kaola:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/sol.gif\" width=\"40%\" height=\"40%\" alt=\":sol:\" title=\":sol:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
										+ "<tr>"
											+"<td><img src=\"smilies/hello.gif\" width=\"40%\" height=\"40%\" alt=\":hello:\" title=\":hello:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/non.gif\" width=\"40%\" height=\"40%\" alt=\":non:\" title=\":non:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/pouah.gif\" width=\"40%\" height=\"40%\" alt=\":pouah:\" title=\":pouah:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"smilies/whistle.gif\" width=\"40%\" height=\"40%\" alt=\":whistle:\" title=\":whistle:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"miam.gif\" width=\"40%\" height=\"40%\" alt=\":miam:\" title=\":miam:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"hebe.gif\" width=\"40%\" height=\"40%\" alt=\":hebe:\" title=\":hebe:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
								  + "</table>";

void SmileyPickerController::loadDefautSmiley() {
	updateView(defaultSmiley);
}

void SmileyPickerController::getSmiley(const QString &keyword) {
	// list green + yellow flags
	const QUrl url(DefineConsts::FORUM_URL + "/message-smi-mp-aj.php?config=hfr.inc&findsmilies=" + keyword);

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

}



void SmileyPickerController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);
				m_Page = response;
				m_IndexSubpagesInFile.clear();
				m_IndexSubpagesInFile.append(0);
				m_lastId = 0;
				parse(response);
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}



void SmileyPickerController::parse(const QString &page, int startingPosition) {
	m_SmileysPage = "<table><tr>";

	QRegExp smiley("<img.*/>");
	QRegExp notifRegexp("putSmiley.this.alt,this.src.");

	smiley.setCaseSensitivity(Qt::CaseSensitive);
	smiley.setMinimal(true);

	int pos = startingPosition;
	int rowNb = 0;
	int colNb = 0;
	while((pos = smiley.indexIn(page, pos)) != -1 && rowNb < 10 ) {

		if(colNb > 6) {
			m_SmileysPage += "</tr><tr>";
			++rowNb;
			colNb = 0;
		}

		m_SmileysPage += "<td>" + page.mid(pos, smiley.matchedLength()).replace(notifRegexp, "navigator.cascades.postMessage(this.alt)") + "</td>";

		++colNb;
		pos += smiley.matchedLength();
	}

	for( ; colNb < 6 ; ++colNb)
		m_SmileysPage += "<td></td>";

	m_SmileysPage += "</tr></table>";

	updateView(m_SmileysPage);


	// -------------------------------------------------------------
	// build index in file for next/previous nav.

	if(pos != -1) {
		bool indexFound = false;
		for(int i = 0 ; i < m_IndexSubpagesInFile.length() ; ++i)
			if(m_IndexSubpagesInFile[i] == pos) {
				indexFound = true;
				break;
			}

		if(!indexFound) {
			m_IndexSubpagesInFile.push_back(pos);
		}
	}

}


void SmileyPickerController::getNextPage() {
	if(m_IndexSubpagesInFile.length() == 0)
		return;

	m_lastId = std::min(m_lastId+1, m_IndexSubpagesInFile.length()-1);

	parse(m_Page, m_IndexSubpagesInFile[m_lastId]);
}

void SmileyPickerController::getPrevPage() {
	if(m_IndexSubpagesInFile.length() == 0)
		return;

	m_lastId = std::max(m_lastId-1, 0);

	parse(m_Page, m_IndexSubpagesInFile[m_lastId]);
}


void SmileyPickerController::updateView(const QString &webpage) {
	if(m_WebView == NULL) {
		qWarning() << "error webview was not provided or not a webview";
		return;
	}


	QString colorHandling = "} ";
	if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
		colorHandling = "background-color:#000000; color:#FFFFFF; } ";
	}

	m_WebView->setHtml(QString("<!DOCTYPE html><html><head><style type=\"text/css\">")
				+ "table {table-layout:fixed; width: 800px; border-spacing: 30px; } th {text-align:left; text-decoration:underline;} td {width: 30px; overflow: hidden; }"	// render quotation table
	            + "body {font-size:25px; " + colorHandling  // switch webview color based on theme
	            + "p {font-size:25px;} "
			+ "</style>"
			+ "</head><body><center>" + webpage + "</center></body></html>", QUrl("local:///assets/images/smiley/"));
}


