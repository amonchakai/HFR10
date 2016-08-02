import bb.cascades 1.4
import bb.system 1.2

Page {
    titleBar: TitleBar {
        title: qsTr("Blacklist")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                nav.pop();
            }
        }
    }
    
    Container {
        layout: DockLayout {
            
        }
        
        Label {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            
            text: qsTr("No blacklisted users.")
            visible: theModel.empty
        }
        
        ListView {
            id: blacklistView
            
            focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
            
            // ------------------------------------------------------------------
            // Pull to refresh
            signal refreshTriggered()
            property bool loading: false
            leadingVisualSnapThreshold: 2.0
            leadingVisual: RefreshHeader {
                id: refreshHandler
                onRefreshTriggered: {
                    appSettings.loadBlackList();
                }
            }
            onTouch: {
                refreshHandler.onListViewTouch(event);
            }
            onLoadingChanged: {
                refreshHandler.refreshing = refreshableList.loading;
                
                if(!refreshHandler.refreshing) {
                    // If the refresh is done 
                    // Force scroll to top to ensure that all items are visible
                    scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
                }
            }
            
            dataModel: ArrayDataModel {
                id: theModel
                
                property bool empty: true
                
                
                onItemAdded: {
                    empty = isEmpty();
                }
                onItemRemoved: {
                    empty = isEmpty();
                }  
                onItemUpdated: empty = isEmpty()  
                
                // You might see an 'unknown signal' error  
                // in the QML-editor, guess it's a SDK bug.  
                onItemsChanged: empty = isEmpty()
            
            }
            
            layout: StackListLayout {
                id: theLayout
            }
            
            multiSelectAction: MultiSelectActionItem {}
            
            multiSelectHandler {
                // These actions will be shown during multiple selection, while this 
                // multiSelectHandler is active
                actions: [
                    DeleteActionItem {
                        property variant selectionList
                        property variant selectedItem
                        id: deleteActionItem
                        onTriggered: {
                            deleteActionItem.selectionList = blacklistView.selectionList()
                            deleteActionItem.selectedItem = theModel.data(selectionList);
                            multiSelectDeleteDialog.show()
                        }
                        attachedObjects: [
                            SystemDialog {
                                id: multiSelectDeleteDialog
                                title: qsTr("Remove from Blacklist") + Retranslate.onLocaleOrLanguageChanged
                                body: qsTr("Are you sure you want to show again these users?") + Retranslate.onLocaleOrLanguageChanged
                                onFinished: {
                                    if (result == 3) {
                                    } else {
                                        
                                        for (var i = 0; i < deleteActionItem.selectionList.length; ++ i) {
                                            var encoded_text = theModel.data(deleteActionItem.selectionList[i]);
                                            appSettings.removeFromBlacklistNoAsk(encoded_text);
                                        
                                        }
                                        appSettings.loadBlackList();
                                    }
                                }
                            }
                        ]
                    }
                ]
                
                status: qsTr("Delete")
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: ""
                    
                    CustomListItem {
                        id: customListItem
                        
                        Container {
                            preferredHeight: ui.du(12)
                            id: listItemContainer
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Center
                            layout: DockLayout {
                            }
                            
                            Container {
                                verticalAlignment: VerticalAlignment.Center
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                Container {
                                    preferredWidth: ui.du(0.1)
                                }
                                Label {
                                    text: ListItemData
                                    verticalAlignment: VerticalAlignment.Center
                                }
                            }
                        
                        }
                        
                        contextActions: [
                            ActionSet {
                                title: qsTr("BlackList")
                                
                                DeleteActionItem {
                                    title: qsTr("Remove from blacklist")
                                    onTriggered: {
                                        customListItem.ListItem.view.deleteEntry(ListItemData);
                                    }
                                }
                            }
                        ]
                    }
                
                }
            
            ]
            
            function deleteEntry(encoded_text) {
                appSettings.removeFromBlacklist(encoded_text);
            }
        }
        
    }
    
    onCreationCompleted: {
        appSettings.setBlackListView(blacklistView);
        appSettings.loadBlackList();
    }
}
