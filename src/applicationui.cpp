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
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ListView>
#include <bb/cascades/controls/standardlistitem.h>


#include "LoginController.hpp"
#include "ListFavoriteController.hpp"
#include "ExploreCategoryController.hpp"
#include "ShowThreadController.hpp"
#include "PrivateMessageController.hpp"
#include "PostMessageController.hpp"
#include "SmileyPickerController.hpp"
#include "Network/NetImageTracker.h"
#include "Settings.hpp"


#include "CookieJar.hpp"
#include "DataObjects.h"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    CookieJar *cookies = CookieJar::get();
    cookies->loadFromDisk();

    Settings::loadSettings();

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();


    // -------------------------------------------------------------------------------------------------------
    // Register controllers to QML
    qmlRegisterType<LoginController>("Network.LoginController", 1, 0, "LoginController");
    qmlRegisterType<ListFavoriteController>("Network.ListFavoriteController", 1, 0, "ListFavoriteController");
    qmlRegisterType<ExploreCategoryController>("Network.ExploreCategoryController", 1, 0, "ExploreCategoryController");
    qmlRegisterType<ShowThreadController>("Network.ShowThreadController", 1, 0, "ShowThreadController");
    qmlRegisterType<PrivateMessageController>("Network.PrivateMessageController", 1, 0, "PrivateMessageController");
    qmlRegisterType<PostMessageController>("Network.PostMessageController", 1, 0, "PostMessageController");
    qmlRegisterType<SmileyPickerController>("Network.SmileyPickerController", 1, 0, "SmileyPickerController");
    qmlRegisterType<NetImageTracker>("com.netimage", 1, 0, "NetImageTracker");
    qmlRegisterType<Settings>("conf.settings", 1, 0, "Settings");


    // -------------------------------------------------------------------------------------------------------
    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml")
    						.parent(this);


    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();


    // Set created root object as the application scene
    app->setScene(root);
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("HFRBlack_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
