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

import bb.cascades 1.2

TabbedPane {
    id: mainTab
    showTabsOnActionBar: true
    activeTab: tabHome
    
    
    attachedObjects: [
        Sheet {
            id: settingsPage
            Settings {
                onDone : {
                    settingsPage.close();
                }
            }
        },
        Sheet {
            id: infoPage
            Help {
                onDone: {
                    infoPage.close();
                }
            }
        }
    ]
    
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: qsTr("About")
                imageSource: "asset:///images/icon_about.png"
                onTriggered: {
                    infoPage.open();
                }
            },
            ActionItem {
                title: qsTr("Settings")
                imageSource: "asset:///images/icon_settings.png"
                onTriggered: {
                    settingsPage.open();
                }
            }
        ]
    }
    

    Tab { //First tab
        // Localized text with the dynamic translation and locale updates support
        id: tabHome
        title: qsTr("Home") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_home.png"

        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateImmediately

        delegate: Delegate {
            source: "Home.qml"
        }

    } //End of home tab
    
    
    
    
    Tab { //Favorite tab
        title: qsTr("Favorite") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_favorites.png"

        delegateActivationPolicy: TabDelegateActivationPolicy.Default

        delegate: Delegate {
            source: "Favorite.qml"
        }

    } //End of favorite tab
    
    
    Tab { //Setting tab
        title: qsTr("Message") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_mp.png"
        delegateActivationPolicy: TabDelegateActivationPolicy.Default
        
        delegate: Delegate {
            source: "PrivateMessage.qml"
        }
    
    } //End of Setting tab
    
    
    Tab { //Browse tab
        title: qsTr("Search") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_browse.png"
        
        delegateActivationPolicy: TabDelegateActivationPolicy.Default
        
        delegate: Delegate {
            source: "Search.qml"
        }
    } //End of browse tab

}
