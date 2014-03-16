import bb.cascades 1.2

TabbedPane {
    id: mainWindowsPane
    showTabsOnActionBar: false
    activeTab: tabForum
    
    //Forum part (category, topic)
    Tab {
        id: tabForum
        title: qsTr("Forum") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "images/icon_forum.png"
        delegate: Delegate {
            id: tabForumDelegate
            source: "asset:///DisplayCategoryListPage.qml"
        }
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateImmediately
    }
    
    //MP part
    Tab {
        id: tabMp
        title: qsTr("MP")  + Retranslate.onLocaleOrLanguageChanged
        imageSource: "images/icon_mp.png"
        content: Page {
            content: Label {
                text: "Pour les MPs (à venir)"
            }
        }
    }
    
    //Setting
    Tab {
        id: tabSetting
        title: qsTr("Options") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "images/icon_settings.png"
        content: Page {
            content: Label {
                text: "Pour les options"
            }
        }
    }
    
    //App infos
    Tab {
        id: tabInfos
        title: ("A propos") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "images/icon_about.png"
        content: Page {
            content: Label {
                //TODO Create a page to display app information
                text: "HFR 4 BlackBerry"
            }
        }
    }
}
