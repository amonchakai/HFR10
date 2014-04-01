import bb.cascades 1.2
import Network.PrivateMessageController 1.0

NavigationPane {
    id: nav
    
	Page {
	    Container {
	        ActivityIndicator {
	            id: activityIndicator
	            horizontalAlignment: HorizontalAlignment.Center
	            verticalAlignment: VerticalAlignment.Center
	            preferredHeight: 60
	        }
	        
	        ListView {            
	            objectName: "listMP"
	            
	            // ------------------------------------------------------------------
	            // Pull to refresh
	            signal refreshTriggered()
	            property bool loading: false
	            id: listMP
	            leadingVisualSnapThreshold: 2.0
	            leadingVisual: RefreshHeader {
	                id: refreshHandler
	                onRefreshTriggered: {
	                    listMP.refreshTriggered();
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
	            
	            // ------------------------------------------------------------------
	            // view
	            
	            dataModel: GroupDataModel {
	                id: theModel
	                sortingKeys: ["category"]
	            }
	            
	            listItemComponents: [
	                ListItemComponent {
	                    type: "header"
	                    Header {
	                        title: ListItemData
	                    }
	                },
	                ListItemComponent {
	                    type: "item"
	                    
	                    Container {
	                        id: titleContainer
	                        function typeFromReadStatus(read) {
	                            if(read) {
	                                return FontWeight.Bold;}
	                            else {
	                                return FontWeight.Normal;}
	                        }
	                        function hasRead(read) {
	                            if(read)
	                                return "";
	                            else
	                                return qsTr("[Not Read] "); 
	                        }
	                        
	                        layout: StackLayout {
	                            orientation: LayoutOrientation.TopToBottom
	                        }
	                        verticalAlignment: VerticalAlignment.Top
	                        Label {
	                            text: titleContainer.hasRead(ListItemData.addresseeRead) + ListItemData.title
	                            textStyle.fontWeight: titleContainer.typeFromReadStatus(ListItemData.read)
	                        }
	                        
	                        Container {
	                            layout: DockLayout {
	                            }
	                            horizontalAlignment: HorizontalAlignment.Fill
	                            
	                            Label {
	                                text: ListItemData.lastAuthor + " - " + ListItemData.timestamp
	                                horizontalAlignment: HorizontalAlignment.Right
	                                textStyle {
	                                    base: SystemDefaults.TextStyles.SmallText
	                                    color: Color.Blue
	                                }
	                            }
	                            Label {
	                                text: "@" + ListItemData.addressee
	                                horizontalAlignment: HorizontalAlignment.Left
	                                textStyle {
	                                    base: SystemDefaults.TextStyles.SmallText
	                                    color: Color.Gray
	                                }
	                            }
	                        
	                        }
	                        Divider {}
	                        
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Navigation")
                                    
                                    ActionItem {
                                        title: qsTr("First page")
                                        imageSource: "asset:///images/icon_prev_all.png"
                                        onTriggered: {
                                            titleContainer.ListItem.view.gotoPage(ListItemData.urlFirstPage, ListItemData.title)
                                        }
                                    }
                                    ActionItem {
                                        title: qsTr("Last page read")
                                        imageSource: "asset:///images/icon_next.png"
                                        onTriggered: {
                                            titleContainer.ListItem.view.gotoPage(ListItemData.urlLastPage, ListItemData.title)
                                        }
                                    }
                                }
                            ]
	                    }
	                
	                }
	            ]
	            
                function gotoPage(urlFirstPage, titleTopic) {
                    console.log(urlFirstPage)
                    var page = threadPage.createObject();
                    
                    // Set the url of the page to load and thread caption. 
                    page.urlPage = urlFirstPage
                    page.caption   = titleTopic
                    
                    nav.push(page);
                }
	            
	            onTriggered: {
	                var chosenItem = dataModel.data(indexPath);
	                
	                // Create the content page and push it on top to drill down to it.
	                var page = threadPage.createObject();
	                
	                // Set the url of the page to load and thread caption. 
	                page.urlPage = chosenItem.urlLastPage
	                page.caption   = chosenItem.title
	                
	                nav.push(page);
	            }
	            
	            onRefreshTriggered: {
	                activityIndicator.start();
	                privateMessageController.getMessages();
	            }
	        }
	    
	    }
	    
	    attachedObjects: [
	        PrivateMessageController {
	            id: privateMessageController
	            
	            onComplete: {
	                activityIndicator.stop()
	            }
	        }, 
	        ComponentDefinition {
	            id: threadPage
	            source: "ThreadPage.qml"
	        }
	    ] 
	    
	    onCreationCompleted: {
	        privateMessageController.setListView(listMP);
	        privateMessageController.getMessages();
	        activityIndicator.start();
	    }
	}
}
