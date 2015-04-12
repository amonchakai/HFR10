/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>
#include <bb/device/DisplayInfo>

#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/LocaleHandler>

#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ListView>
#include <bb/cascades/WebView>
#include <bb/cascades/WebStorage>
#include <bb/cascades/controls/standardlistitem.h>

#include <QTimer>

#include "LoginController.hpp"
#include "ListFavoriteController.hpp"
#include "ExploreCategoryController.hpp"
#include "ShowThreadController.hpp"
#include "PrivateMessageController.hpp"
#include "PostMessageController.hpp"
#include "SmileyPickerController.hpp"
#include "SearchController.hpp"
#include "BookmarksController.hpp"
#include "SearchKeyRetriever.hpp"
#include "ImageUploaderController.hpp"
#include "Network/NetImageTracker.h"
#include "Settings.hpp"
#include "Network/WebResourceManager.h"
#include "Network/CookieJar.hpp"
#include "DataObjects.h"
#include "BugReportController.hpp"
#include "ApplicationLogController.hpp"

#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>
#include <bb/system/CardDoneMessage>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app),
        m_pLocaleHandler(NULL),
        m_app(app),
        m_root(NULL),
        m_qml(NULL),
        m_Settings("Amonchakai", "HFR10Service"),
        m_InvokeManager(new bb::system::InvokeManager(this)),
        m_HeadlessStart(false),
        m_isCard(false)

{





    bool connectResult;

    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning.
    Q_UNUSED(connectResult);
    connectResult = connect(m_InvokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(onInvoked(const bb::system::InvokeRequest&)));
    qDebug() << "HubIntegration: started and connected to invokeManager";


    // This is only available in Debug builds.
    Q_ASSERT(connectResult);

    switch(m_InvokeManager->startupMode()) {
        case bb::system::ApplicationStartupMode::LaunchApplication:
            qDebug() << "HeadlessHubIntegration: Regular application launch";
            break;
        case bb::system::ApplicationStartupMode::InvokeApplication:
            m_HeadlessStart = true;
            qDebug() << "--------------------- HeadlessHubIntegration: Launching app from invoke";
            break;
        case bb::system::ApplicationStartupMode::InvokeCard:
            m_HeadlessStart = true;
            m_isCard = true;
            qDebug() << "--------------------- HeadlessHubIntegration: Launching card from invoke";
            break;
        // enable when 10.3 beta is released
        //case ApplicationStartupMode::InvokeHeadless:
        case 4:
            qDebug() << "--------------------- HeadlessHubIntegration: Launching headless from invoke";
            m_HeadlessStart = true;
            break;
        default:
            qDebug() << "--------------------- HeadlessHubIntegration: other launch: " << m_InvokeManager->startupMode();
            break;
       }

    // ---------------------------------------------------------------------
    // Regular integration

    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    CookieJar *cookies = CookieJar::get();
    cookies->loadFromDisk();

    Settings::themeValue() = app->themeSupport()->theme()->colorTheme()->style() == VisualStyle::Bright ? 1 : 2;

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();


    WebResourceManager::get()->cleanup();


    // -------------------------------------------------------------------------------------------------------
    // Register controllers to QML
    qmlRegisterType<LoginController>("Network.LoginController", 1, 0, "LoginController");
    qmlRegisterType<ListFavoriteController>("Network.ListFavoriteController", 1, 0, "ListFavoriteController");
    qmlRegisterType<ExploreCategoryController>("Network.ExploreCategoryController", 1, 0, "ExploreCategoryController");
    qmlRegisterType<ShowThreadController>("Network.ShowThreadController", 1, 0, "ShowThreadController");
    qmlRegisterType<PrivateMessageController>("Network.PrivateMessageController", 1, 0, "PrivateMessageController");
    qmlRegisterType<PostMessageController>("Network.PostMessageController", 1, 0, "PostMessageController");
    qmlRegisterType<SmileyPickerController>("Network.SmileyPickerController", 1, 0, "SmileyPickerController");
    qmlRegisterType<SearchController>("Network.SearchController", 1, 0, "SearchController");
    qmlRegisterType<SearchKeyRetriever>("Network.SearchKeyRetriever", 1, 0, "SearchKeyRetriever");
    qmlRegisterType<BookmarksController>("Network.BookmarksController", 1, 0, "BookmarksController");
    qmlRegisterType<ImageUploaderController>("Network.ImageUploaderController", 1, 0, "ImageUploaderController");
    qmlRegisterType<NetImageTracker>("com.netimage", 1, 0, "NetImageTracker");
    qmlRegisterType<Settings>("conf.settings", 1, 0, "AppSettings");
    qmlRegisterType<QTimer>("Lib.QTimer", 1, 0, "QTimer");
    qmlRegisterType<BugReportController>("Lib.BugReport", 1, 0, "BugReport");
    qmlRegisterType<ApplicationLogController>("Lib.ApplicationLogController", 1, 0, "ApplicationLogController");

    // -------------------------------------------------------------------------------------------------------
    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.

    if(!m_HeadlessStart) {
        QmlDocument *qml = QmlDocument::create("asset:///main.qml")
                                .parent(this);


        // Create root object for the UI
        AbstractPane *root = qml->createRootObject<AbstractPane>();


        // Set created root object as the application scene
        m_app->setScene(root);


        bb::device::DisplayInfo display;
        QDeclarativePropertyMap* displayDimensions = new QDeclarativePropertyMap;
        displayDimensions->insert( "width", QVariant( display.pixelSize().width() ) );
        displayDimensions->insert( "height", QVariant( display.pixelSize().height() ) );
        qml->setContextProperty( "DisplayInfo", displayDimensions );

    }
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("HFR10_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}


void ApplicationUI::onInvoked(const bb::system::InvokeRequest& request) {
    qDebug() << "invoke!" << request.action();

    if(request.action().compare("bb.action.VIEW") == 0 || request.action().compare("bb.action.OPEN") == 0) {
//         qDebug() << "HubIntegration: onInvoked: view item: " << request.data();

         JsonDataAccess jda;

         QVariantMap objectMap = (jda.loadFromBuffer(request.data())).toMap();
         QVariantMap itemMap = objectMap["attributes"].toMap();


         QVariantList items = m_Settings.value("hub/items").toList();

         QString urlToOpen;
         for(int index = 0; index < items.size(); index++) {
             QVariantMap item = items.at(index).toMap();
             QString sourceId = item["messageid"].toString();

              if (item["sourceId"].toString() == itemMap["messageid"].toString() ||
                  item["sourceId"].toString() == itemMap["sourceId"].toString()) {

                  qDebug() << "FOUD!";
                  urlToOpen = item["url"].toString();

                  break;
              }
         }

         qDebug() << "URL TO OPEN: " << urlToOpen;

         QmlDocument *qml = QmlDocument::create("asset:///StartupCardThread.qml")
                                                          .parent(this);

         m_root = qml->createRootObject<NavigationPane>();
         qml->setContextProperty("_app", this);
         m_app->setScene(m_root);

         QObject *thread = m_root->findChild<QObject*>("pageThread");
         if(thread != NULL) {
             thread->setProperty("urlPage", urlToOpen);

             bb::device::DisplayInfo display;
             QDeclarativePropertyMap* displayDimensions = new QDeclarativePropertyMap;
             displayDimensions->insert( "width", QVariant( display.pixelSize().width() ) );
             displayDimensions->insert( "height", QVariant( display.pixelSize().height() ) );
             qml->setContextProperty( "DisplayInfo", displayDimensions );
         } else
             qDebug() << "pageThread variable is not found in the qml document :(";


         InvokeRequest request;
         request.setTarget("com.amonchakai.HFR10Service");
         request.setAction("bb.action.MARKREAD");
         request.setMimeType("hub/item");
         request.setUri(QUrl("pim:"));

         QByteArray bytes;
         jda.saveToBuffer(objectMap, &bytes);
         request.setData(bytes);

         m_InvokeManager->invoke(request);

    }

    if(request.action().compare("bb.action.COMPOSE") == 0) {
        QmlDocument *qml = QmlDocument::create("asset:///StartupCardCompose.qml")
                                                                  .parent(this);

        m_root = qml->createRootObject<NavigationPane>();
        qml->setContextProperty("_app", this);

        m_app->setScene(m_root);

        QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
        if (!QFile::exists(directory)) {
            return;
        }

        QFile file(directory + "/UserID.txt");

        QString userName;
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream stream(&file);
            stream >> userName;

            file.close();
        }

        QObject *thread = m_root->findChild<QObject*>("postMesssage");
        if(thread != NULL) {
            thread->setProperty("pseudo", userName);

            bb::device::DisplayInfo display;
            QDeclarativePropertyMap* displayDimensions = new QDeclarativePropertyMap;
            displayDimensions->insert( "width", QVariant( display.pixelSize().width() ) );
            displayDimensions->insert( "height", QVariant( display.pixelSize().height() ) );
            qml->setContextProperty( "DisplayInfo", displayDimensions );
        }
    }


}


void ApplicationUI::closeCard() {
    m_app->requestExit();

    if (m_isCard) {
        // Assemble response message
        CardDoneMessage message;
        message.setData(tr(""));
        message.setDataType("text/plain");
        message.setReason(tr("Success!"));

        // Send message
        m_InvokeManager->sendCardDone(message);
    }
}

