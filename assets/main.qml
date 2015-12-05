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
import Network.LoginController 1.0


TabbedPane {
    id: mainTab
    showTabsOnActionBar: true
    activeTab: tabFav
    property variant focusOnFavTab
    property variant focusOnMPTab
        
    attachedObjects: [
        Delegate {
            id: paymentDelegate
            source: "Payment.qml"
        
        },
        Sheet {
            id: payment
            content: paymentDelegate.object
            onOpenedChanged: {
                if (opened)
                    paymentDelegate.active = true;
            }
            onClosed: {
                paymentDelegate.active = false;
            }
        },
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
        },
        Sheet {
            id: welcome
            Welcome {
                onDone: {
                    welcome.close();
                }
            }
        },
        Sheet {
            id: bugReport
            BugReport {
                onDone : {
                    bugReport.close();
                }
            }
        },
        
        LoginController {
            id: loginController
            
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
            },
            ActionItem {
                title: qsTr("Report")
                imageSource: "asset:///images/BugReport_white.png"
                onTriggered: {
                    bugReport.open();
                }
            },
            ActionItem {
                title: qsTr("Donate")
                imageSource: "asset:///images/icon_credit.png"
                onTriggered: {
                    payment.open();
                }
            }
            
            
        ]
    }
    
    
    Tab { //Favorite tab
        id: tabFav
        title: qsTr("Favorite") + Retranslate.onLocaleOrLanguageChanged
        
        imageSource: "asset:///images/icon_favorites.png"
        
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateImmediately
        
        delegate: Delegate {
            id: favorite
            source: "Favorite.qml"
        }    
    } //End of favorite tab
    

    Tab { //First tab
        // Localized text with the dynamic translation and locale updates support
        id: tabHome
        title: qsTr("Home") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_home.png"

        delegateActivationPolicy: TabDelegateActivationPolicy.Default

        delegate: Delegate {
            source: "Home.qml"
        }

    } //End of home tab
    
    
    Tab { //Setting tab
        id: tabMP
        title: qsTr("Message") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_mp.png"
        delegateActivationPolicy: TabDelegateActivationPolicy.Default
        
        delegate: Delegate {
            source: "PrivateMessage.qml"
        }
    
    } //End of Setting tab
    
    Tab {
        title: qsTr("Bookmarks") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.InOverflow
        imageSource: "asset:///images/Bookmarks.png"
        
        
        delegateActivationPolicy: TabDelegateActivationPolicy.Default
        
        delegate: Delegate {
            source: "Bookmarks.qml"
        }
    }
    
    Tab { //Browse tab
        title: qsTr("Search") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.InOverflow
        imageSource: "asset:///images/icon_browse.png"
        
        delegateActivationPolicy: TabDelegateActivationPolicy.Default
        
        delegate: Delegate {
            source: "Search.qml"
        }
    } //End of browse tab
    
    
    onCreationCompleted: {
        if(!loginController.isLogged()) {
            welcome.open();
        }
    }
    
    onActiveTabChanged: {
        if(activeTab == tabFav) {
            if(focusOnFavTab) {
                focusOnFavTab.requestFocus();
            }
        }
        
        if(activeTab == tabMP) {
            if(focusOnMPTab) {
                focusOnMPTab.requestFocus();
            }
        }
    }
    
   
}
