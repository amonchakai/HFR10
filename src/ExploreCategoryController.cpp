/*
 * ExploreCategoryController.cpp
 *
 *  Created on: 27 mars 2014
 *      Author: PierreL
 */

#include "ExploreCategoryController.hpp"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>
#include <QDateTime>
#include <QFile>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/platform/Notification>
#include <bb/data/JsonDataAccess>
#include <bb/cascades/GroupDataModel>

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>

#include  "Globals.h"
#include  "Network/HFRNetworkAccessManager.hpp"
#include  "DataObjects.h"
#include  "Settings.hpp"

ExploreCategoryController::ExploreCategoryController(QObject *parent)
	: QObject(parent), m_ListView(NULL), m_RootForumListView(NULL), m_Datas(new QList<ThreadListItem*>()), m_SelectedSubCat(0) {

}


void ExploreCategoryController::loadIndex() {
    if(!m_RootForumListView)
        return;

    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData/Cats/");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/CacheCategories.json");

    if (!file.open(QIODevice::ReadOnly)) {
        buildIndex();
        return;
    }

    updateIndexView(file.readAll());
    file.close();

}

void ExploreCategoryController::updateIndexView(const QByteArray& buffer) {
    using namespace bb::data;
    JsonDataAccess jda;

    QVariant qtData = jda.loadFromBuffer(buffer);

    if(jda.hasError()) {
        qDebug() << jda.error().errorMessage();
    }


    if(m_RootForumListView == NULL) {
        qWarning() << "did not received the list. quit.";
        return;
    }

    using namespace bb::cascades;
    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_RootForumListView->dataModel());
    dataModel->clear();

    QList<QVariant> list = qtData.toList();
    for(int k = 0; k < (list.size()/2); k++) list.swap(k,list.size()-(1+k));
    dataModel->insertList(list);

    emit catLoaded();
}

void ExploreCategoryController::buildIndex() {
    const QUrl url(DefineConsts::FORUM_URL);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyIndex()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void ExploreCategoryController::checkReplyIndex() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                parseIndex(response);
            }
        } else {
            response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
            qDebug() << response;
        }

        reply->deleteLater();
    }
}

void ExploreCategoryController::parseIndex(const QString &page) {
    QRegExp mainCat("<tr class=\"cat cBackCouleurTab1\" id=\"cat([0-9]+)\".*<a href=\"([^\"]+)\" class=\"cCatTopic\">(.*)</a>");
    mainCat.setMinimal(true);
    mainCat.setCaseSensitivity(Qt::CaseSensitive);


    int pos = 0;
    int lastPos = 0;

    QMap<int, QString> icons;
    icons[1] = "ProcessorFilled-40@2x";
    icons[16] = "KeyboardFilled-40@2x";
    icons[15] = "LaptopFilled-40@2x";
    icons[2] = "SupportFilled-40@2x";
    icons[30] = "ShieldFilled-40@2x";
    icons[23] = "SmartphoneTabletFilled-40@2x";
    icons[25] = "Apple@2x";
    icons[3] = "VideoCallFilled-40@2x";
    icons[14] = "CameraFilled-40@2x";
    icons[5] = "ControllerFilled-40@2x";
    icons[4] = "WindowsClientFilled-40@2x";
    icons[22] = "Wi-FiLogoFilled-40@2x";
    icons[21] = "VOIPGatewayFilled-40@2x";
    icons[11] = "Debian-40@2x";
    icons[10] = "SourceCodeFilled-40@2x";
    icons[12] = "DesignFilled-40@2x";
    icons[6] = "PriceTagUSDFilled-40@2x";
    icons[8] = "GraduationCapFilled-40@2x";
    icons[9] = "BroadcastingFilled-40@2x";
    icons[13] = "ChatFilled-40@2x";

    QString prevCatId = "";
    QByteArray buffer = "[";
    bool inserted = false;
    while((pos = mainCat.indexIn(page, lastPos)) != -1) {
        pos += mainCat.matchedLength();

        QString iconPath = icons[30];
        if(icons.find(mainCat.cap(1).toInt()) != icons.end()) {
            iconPath = icons.find(mainCat.cap(1).toInt()).value();
        }

        if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
            iconPath = "asset:///images/categories/" + iconPath + "_white" + ".png";
        } else {
            iconPath = "asset:///images/categories/" + iconPath + ".png";
        }



        // parse the sub-cats
        parseIndexDetails(page.mid(lastPos, pos-lastPos), prevCatId);

        // generate a json file to list the categories
        if(!mainCat.cap(3).isEmpty()) {
            if(inserted) buffer += ", ";
            buffer += "\n\t{ \"group\": \"HFR\", \"icon\": \"" + iconPath + "\", \"title\": \""
                    + mainCat.cap(3).toUtf8().replace("&amp;", "&") + "\", \"url\": \"" + DefineConsts::FORUM_URL + mainCat.cap(2).replace("hfr/AchatsVentes/Hardware/liste_sujet", "hfr/AchatsVentes/liste_sujet") + "\", \"xml\": \"" + QDir::homePath() + QLatin1String("/HFRBlackData/Cats/") + mainCat.cap(1) + ".xml\", \"catId\": " + mainCat.cap(1) + "} ";
            inserted = true;
        }

        prevCatId = mainCat.cap(1);
        lastPos = pos;
    }
    // parse the sub-cats
    parseIndexDetails(page.mid(lastPos, pos-lastPos), prevCatId);

    // generate a json file to list the categories
    if(!mainCat.cap(3).isEmpty()){
        QString iconPath = icons[30];
        if(icons.find(mainCat.cap(1).toInt()) != icons.end()) {
            iconPath = icons.find(mainCat.cap(1).toInt()).value();
        }
        if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
            iconPath = "asset:///images/categories/" + iconPath + "_white" + ".png";
        } else {
            iconPath = "asset:///images/categories/" + iconPath + ".png";
        }

        if(inserted) buffer += ", ";
        buffer += "\n\t{ \"group\": \"HFR\", \"icon\": \"" + iconPath + "\", \"title\": \""
                 + mainCat.cap(3).toUtf8().replace("&amp;", "&") + "\", \"url\": \"" + DefineConsts::FORUM_URL + mainCat.cap(2).replace("hfr/AchatsVentes/Hardware/liste_sujet", "hfr/AchatsVentes/liste_sujet") + "\", \"xml\": \"" + QDir::homePath() + QLatin1String("/HFRBlackData/Cats/") + mainCat.cap(1) + ".xml\", \"catId\": " + mainCat.cap(1) + "} ";
    }

    buffer += "\n]";



    // update the view
    updateIndexView(buffer);


    // cache the list of cats
    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData/Cats/");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/CacheCategories.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(buffer);
        file.close();
    }

}

void ExploreCategoryController::parseIndexDetails(const QString& catDetails, const QString catId) {
    QRegExp subcat("<a href=\"(.*)\" class=\"Tableau\">(.*)</a>");
    subcat.setMinimal(true);
    subcat.setCaseSensitivity(Qt::CaseSensitive);

    QByteArray buffer = "<root>";
    int pos = 0;
    while((pos = subcat.indexIn(catDetails, pos)) != -1) {
        buffer += "\n\t<item title=\"" + subcat.cap(2).toUtf8() + "\" url=\"" + DefineConsts::FORUM_URL + subcat.cap(1) + "\" />";
        pos += subcat.matchedLength();
    }
    buffer += "\n</root>";


    // cache the list of subcats
    QString directory = QDir::homePath() + QLatin1String("/HFRBlackData/Cats/");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/" + catId + ".xml");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(buffer);
        file.close();
    }

}

void ExploreCategoryController::listSubCat(int subcat) {
    if(subcat < 0) return;

    qDebug()  << "ExploreCategoryController::listSubCat(" << subcat << ");";
	if(subcat < m_SubCatURL.length()) {
		m_SelectedSubCat = subcat;
		qDebug() << m_SubCatURL[subcat];
		listTopics(m_SubCatURL[subcat]);
	}
}

void ExploreCategoryController::loadSubCats(const QString &xmlFile) {
	QFile file(xmlFile);

	if (file.open(QIODevice::ReadOnly)) {
		QString content = QString::fromUtf8(file.readAll());

		QRegExp catRegExp("<item title=\"(.+)\" url=\"(.+)\"");
		catRegExp.setCaseSensitivity(Qt::CaseSensitive);
		catRegExp.setMinimal(true);

		QRegExp andAmp("&amp;");
		int pos = 0;
		m_SubCatURL.clear();

		while((pos = catRegExp.indexIn(content, pos)) != -1) {
			QString s = catRegExp.cap(2); s.replace(andAmp, "&");
			m_SubCatURL.append(s);

			pos += catRegExp.matchedLength();
		}
	}

}

void ExploreCategoryController::listTopics(const QString &url_string, int flag, int cat) {
	m_Url = url_string;

	if(flag > 0) showTopicList(DefineConsts::FORUM_URL + "/forum1.php?config=hfr.inc&cat=" + QString::number(cat) + "&page=1&subcat=0&sondage=0&owntopic=" + QString::number(flag) + "&trash=0&trash_post=0&moderation=0&new=0&nojs=0&subcatgroup=0");
    else         showTopicList(url_string);

}

void ExploreCategoryController::showTopicList(const QString &url_string) {
	// list green + yellow flags
	const QUrl url(url_string);
	m_LastLoadedUrl = url_string;

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

}

void ExploreCategoryController::refresh() {
	showTopicList(m_LastLoadedUrl);
}


void ExploreCategoryController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);

				parse(response);
/*
				QFile htmlEndTemplateFile(QDir::currentPath() + "/app/native/assets/topics.html");
                if (htmlEndTemplateFile.open(QIODevice::ReadOnly)) {
                    //parse(response);
                    parse(htmlEndTemplateFile.readAll());
                }
*/
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}



void ExploreCategoryController::parse(const QString &page) {

	for(int i = 0 ; i < m_Datas->length() ; ++i)
		(*m_Datas)[i]->deleteLater();
	m_Datas->clear();

	QRegExp andAmp("&amp;");
	QRegExp quote("&#034;");
	QRegExp euro("&euro;");
	QRegExp inf("&lt;");
	QRegExp sup("&gt;");


    // ----------------------------------------------------------------------------------------------
    // Parse new MP, and notify if needed!
    QRegExp newMP("Vous avez ([0-9]+) nouveau[x]* message[s]*");
    newMP.setMinimal(true);
    if(newMP.indexIn(page) != -1) {
        if(Settings::getMPNotificationUp) {
            Settings::setMPNotificationUp(true);

            bb::platform::Notification *notif = new bb::platform::Notification();
            notif->notify();

            Settings s;
            s.saveSettings();
        }
    }

	// ----------------------------------------------------------------------------------------------
	// drap filter

	// The URL for filtering topic by flags is highly similar:
	// "/forum1.php?config=hfr.inc&cat=25&page=1&subcat=0&sondage=0&owntopic=1&trash=0&trash_post=0&moderation=0&new=0&nojs=0&subcatgroup=0" Blue
	// "/forum1.php?config=hfr.inc&cat=25&page=1&subcat=0&sondage=0&owntopic=2&trash=0&trash_post=0&moderation=0&new=0&nojs=0&subcatgroup=0" Red
	// "/forum1.php?config=hfr.inc&cat=25&page=1&subcat=0&sondage=0&owntopic=3&trash=0&trash_post=0&moderation=0&new=0&nojs=0&subcatgroup=0" Star

	// Just "owntopic" changes...
	// => retrieve one URL (using either blue or red flag), then we will be able to tune the URL depending on our needs.

	QRegExp filterDrapRegexp("<a accesskey=\"b\" href=\"(.+)\" class=\"onglet\" id=\"onglet3\"");
	filterDrapRegexp.setCaseSensitivity(Qt::CaseSensitive);
	filterDrapRegexp.setMinimal(true);

	if(filterDrapRegexp.indexIn(page, 0) != -1) {		// can we catch the URL from blue flag
		m_GeneralUrl = filterDrapRegexp.cap(1);
		m_GeneralUrl.replace(andAmp, "&");
	} else {											// if not, it means we were on the blue flag, then red one must be available.

		filterDrapRegexp = QRegExp("<a accesskey=\"r\" href=\"(.+)\" class=\"onglet\" id=\"onglet4\"");
		filterDrapRegexp.setCaseSensitivity(Qt::CaseSensitive);
		filterDrapRegexp.setMinimal(true);

		if(filterDrapRegexp.indexIn(page, 0) != -1) {
			m_GeneralUrl = filterDrapRegexp.cap(1);
			m_GeneralUrl.replace(andAmp, "&");
		}
	}


	// ----------------------------------------------------------------------------------------------
	// Parse categories using regexp

	// Get favorite topics
	//QRegExp regexp(QString("sujet ligne_booleen cBackCouleurTab[0-9] ( ligne_sticky\"|\") >.*<td class=\"sujetCase1 cBackCouleurTab[0-9] \"><img src=\".*\" title=\".*\" alt=\"(Off|On)\" /></td>.*<a href=\"(.+)\" class=\"cCatTopic\" title=\"[^\"]+\">(.+)</a></td>"));  	// topics' name
	QRegExp regexp(QString("<td class=\"sujetCase1 cBackCouleurTab[0-9] \"><img src=\".*\" title=\".*\" alt=\"(Off|On)\" /></td>.*<a href=\"(.+)\" class=\"cCatTopic\" title=\"[^\"]+\">(.+)</a></td>"));
	QRegExp stickyRegExp(QString("ligne_sticky"));
	regexp.setCaseSensitivity(Qt::CaseSensitive);
	regexp.setMinimal(true);


	QString today = QDateTime::currentDateTime().toString("dd-MM-yyyy");

	int pos = 0;
	int lastPos = regexp.indexIn(page, pos);
	QString caption;
	QString urlFirstPage;
	bool read = regexp.cap(1).compare("On") == 0;
	bool sticky = stickyRegExp.indexIn(page.mid(pos, lastPos)) != -1;

	if(lastPos != -1) {
		caption = regexp.cap(3);
		caption.replace(andAmp,"&");
		caption.replace(quote,"\"");
		caption.replace(euro, "e");
		caption.replace(inf, "<");
		caption.replace(sup, ">");

		urlFirstPage = regexp.cap(2);
		urlFirstPage.replace(andAmp,"&");

	}

	while((pos = regexp.indexIn(page, lastPos)) != -1) {
		pos += regexp.matchedLength();

		// parse each post individually
		parseThreadListing(caption, urlFirstPage, read, sticky, page.mid(lastPos, pos-lastPos));


		caption = regexp.cap(3);
		caption.replace(andAmp,"&");
		caption.replace(quote,"\"");
		caption.replace(euro, "e");
		caption.replace(inf, "<");
		caption.replace(sup, ">");

		urlFirstPage = regexp.cap(2);
		urlFirstPage.replace(andAmp,"&");

		read = regexp.cap(1).compare("On") == 0;
		sticky = stickyRegExp.indexIn(page.mid(pos, lastPos)) != -1;

		lastPos = pos;
	}
	parseThreadListing(caption, urlFirstPage, read, sticky, page.mid(lastPos, pos-lastPos));


	updateView();
	emit complete();

}


void ExploreCategoryController::parseThreadListing(const QString &caption, const QString &urlFirstPage, bool read, bool sticky, const QString &threadListing) {
	ThreadListItem *item = new ThreadListItem();
	QRegExp andAmp("&amp;");
	QRegExp nbsp("&nbsp;");

	item->setTitle(caption);
	item->setRead(read);
	item->setUrlFirstPage(urlFirstPage);
	item->setSticky(sticky);

	QRegExp lastPostReadRegexp("</td><td class=\"sujetCase5\"><a href=\"(.*#t[0-9bas]+)\"><img src=");
	lastPostReadRegexp.setCaseSensitivity(Qt::CaseSensitive);
	lastPostReadRegexp.setMinimal(true);

	if(lastPostReadRegexp.indexIn(threadListing, 0) != -1) {
		QString s = lastPostReadRegexp.cap(1);
		s.replace(andAmp, "&");
		item->setUrlLastPostRead(s);
	}

	QRegExp pageNumberRegExp("<td class=\"sujetCase4\"><a href=\"(.*)\" class=\"cCatTopic\">([0-9]+)</a></td><td class=\"sujetCase5\">");
	pageNumberRegExp.setCaseSensitivity(Qt::CaseSensitive);
	pageNumberRegExp.setMinimal(true);

	if(pageNumberRegExp.indexIn(threadListing, 0) != -1) {
		item->setPages(pageNumberRegExp.cap(2));

		if(item->getUrlLastPostRead() == "") {
			QString s = pageNumberRegExp.cap(1);
			s.replace(andAmp, "&");
			item->setUrlLastPostRead(s+"#bas");
		}
	} else {
		item->setPages("1");
	}

	QRegExp lastPageRegExp("<td class=\"sujetCase9 cBackCouleurTab[0-9] \"><a href=\"(.*)\" class=\"Tableau\">(.*)<br /><b>(.*)</b>");
	lastPageRegExp.setCaseSensitivity(Qt::CaseSensitive);
	lastPageRegExp.setMinimal(true);

	if(lastPageRegExp.indexIn(threadListing, 0) != -1) {
		QString s = lastPageRegExp.cap(2);
		s.replace(nbsp, " ");
		item->setTimestamp(s);

		s = lastPageRegExp.cap(1);
		s.replace(andAmp, "&");
		item->setUrlLastPage(s);

		item->setLastAuthor(lastPageRegExp.cap(3));
	}

	QRegExp flagTypeRegexp("<img src=\"http://forum-images.hardware.fr/themes_static/images_forum/1/favoris.gif\"");
	if(flagTypeRegexp.indexIn(threadListing, 0) != -1)
		item->setFlagType(Flag::FAVORITE);

	flagTypeRegexp = QRegExp("<img src=\"http://forum-images.hardware.fr/themes_static/images_forum/1/flag([0-1]).gif\"");
	if(flagTypeRegexp.indexIn(threadListing, 0) != -1) {
		switch(flagTypeRegexp.cap(1).toInt()) {
			case 0:
				item->setFlagType(Flag::READ);
				break;

			case 1:
				item->setFlagType(Flag::PARTICIPATE);
				break;
		}
	}


	if(!item->getLastAuthor().isEmpty())
		m_Datas->append(item);
	else
		item->deleteLater();
}


void ExploreCategoryController::updateView() {

	// ----------------------------------------------------------------------------------------------
	// get the dataModel of the listview if not already available

	if(m_ListView == NULL) {
		qWarning() << "the list view is either not provided or not a listview...";
		return;
	}

	using namespace bb::cascades;

	GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
	if (dataModel) {
		dataModel->clear();
	} else {
		dataModel = new GroupDataModel(
						QStringList() << "title"
									  << "timestamp"
									  << "lastAuthor"
									  << "urlFirstPage"
									  << "urlLastPage"
									  << "urlLastPostRead"
									  << "pages"
									  << "flagType"
									  << "read"
					);
		m_ListView->setDataModel(dataModel);
	}
	dataModel->setGrouping(ItemGrouping::ByFullValue);

	// ----------------------------------------------------------------------------------------------
	// push data to the view

	QList<QObject*> datas;
	for(int i = m_Datas->length()-1 ; i >= 0 ; --i) {
		datas.push_back(m_Datas->at(i));
	}

	dataModel->clear();
	dataModel->insertList(datas);

}

void ExploreCategoryController::filterByFlag(int flag) {
	QRegExp locateFlagInURL("owntopic=([0-3])");

	int pos = locateFlagInURL.indexIn(m_GeneralUrl, 0);
	if(pos != -1) {
		switch(flag) {
			case Flag::NONE:
				showTopicList(m_SubCatURL[m_SelectedSubCat]);
				return;

			case Flag::PARTICIPATE:
				m_GeneralUrl[pos+9] = '1';
				break;

			case Flag::READ:
				m_GeneralUrl[pos+9] = '2';
				break;

			case Flag::FAVORITE:
				m_GeneralUrl[pos+9] = '3';
				break;
		}

		showTopicList(DefineConsts::FORUM_URL + m_GeneralUrl);


	}
}


void ExploreCategoryController::firstPage() {
	QRegExp isFormListSujet("liste_sujet-([0-9]+).htm");
	int pos = isFormListSujet.indexIn(m_Url, 0);
	if(pos != -1) {
		m_Url = m_SubCatURL[m_SelectedSubCat];

	} else {
		QRegExp locatePage("page=([0-9]+)");
		int pos = locatePage.indexIn(m_GeneralUrl, 0);
		if(pos != -1) {
			int length = pos+locatePage.matchedLength();

			m_GeneralUrl = m_GeneralUrl.mid(0,pos) + "page=1" + m_GeneralUrl.mid(length,m_GeneralUrl.length()-length);

			showTopicList(DefineConsts::FORUM_URL + m_GeneralUrl);

		}
	}
}


void ExploreCategoryController::nextPage() {
	QRegExp isFormListSujet("liste_sujet-([0-9]+).htm");
	int pos = isFormListSujet.indexIn(m_Url, 0);
	if(pos != -1) {
		listTopics(m_Url.mid(0,pos) + "liste_sujet-" + QString::number(isFormListSujet.cap(1).toInt()+1) + ".htm");

	} else {

		QRegExp locatePage("page=([0-9]+)");
		pos = locatePage.indexIn(m_GeneralUrl, 0);
		if(pos != -1) {
			int length = pos+locatePage.matchedLength();

			m_GeneralUrl = m_GeneralUrl.mid(0,pos) + "page=" + QString::number(locatePage.cap(1).toInt()+1) + m_GeneralUrl.mid(length,m_GeneralUrl.length()-length);

			showTopicList(DefineConsts::FORUM_URL + m_GeneralUrl);
		}
	}
}

void ExploreCategoryController::prevPage() {
	QRegExp isFormListSujet("liste_sujet-([0-9]+).htm");
	int pos = isFormListSujet.indexIn(m_Url, 0);
	if(pos != -1) {
		listTopics(m_Url.mid(0,pos) + "liste_sujet-" + QString::number(std::max(isFormListSujet.cap(1).toInt()-1,1)) + ".htm");

	} else {
		QRegExp locatePage("page=([0-9]+)");
		int pos = locatePage.indexIn(m_GeneralUrl, 0);
		if(pos != -1) {
			int length = pos+locatePage.matchedLength();

			m_GeneralUrl = m_GeneralUrl.mid(0,pos) + "page=" + QString::number(std::max(locatePage.cap(1).toInt()-1,1)) + m_GeneralUrl.mid(length,m_GeneralUrl.length()-length);

			showTopicList(DefineConsts::FORUM_URL + m_GeneralUrl);
		}
	}
}


