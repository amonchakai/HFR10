import bb.cascades 1.2
import bb.data 1.0
import Network.ListFavoriteController 1.0


NavigationPane {
    id: nav
    Page {
	    Container {
	        	        
            ActivityIndicator {
                id: activityIndicator
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                preferredHeight: 200
            }
	        
	        ListView {
                // ------------------------------------------------------------------
                // Pull to refresh
                signal refreshTriggered()
                property bool loading: false
                id: listFav
                leadingVisualSnapThreshold: 2.0
                leadingVisual: RefreshHeader {
                    id: refreshHandler
                    onRefreshTriggered: {
                        listFav.refreshTriggered();
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
                
                // ---------------------------------------------------------------------
                // view

	            objectName: "listFav"
	            
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
	                        layout: StackLayout {
	                            orientation: LayoutOrientation.TopToBottom
	                        }
	                        verticalAlignment: VerticalAlignment.Top
	                        Label {
	                            text: ListItemData.caption
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
	                                text: ListItemData.pages
	                                horizontalAlignment: HorizontalAlignment.Left
	                                textStyle {
	                                    base: SystemDefaults.TextStyles.SmallText
	                                    color: Color.Gray
	                                }
	                            }
	                        }
	                        Divider {}
	                    }
	                }
	            ]
	            
	            onTriggered: {
	                var chosenItem = dataModel.data(indexPath);
	                
	                // Create the content page and push it on top to drill down to it.
	                var page = threadPage.createObject();
	                
	                // Set the url of the page to load and thread caption. 
                    page.urlPage = chosenItem.indexLastPost
	                page.caption   = chosenItem.caption
	                	                
	                nav.push(page);
	            }
	            
	            onRefreshTriggered: {
                    activityIndicator.start();
                    listFavoriteController.getFavorite();
                }
	            
	         }
	        
	         attachedObjects: [
	             ListFavoriteController {
	                 id: listFavoriteController
	                 
	                 onComplete: {
	                        activityIndicator.stop();
	                 }
	             }, 
	             ComponentDefinition {
	                 id: threadPage
	                 source: "ThreadPage.qml"
	             }
	         ]
	         
	    }
	    
	    onCreationCompleted: {
	        
            listFavoriteController.setListView(listFav);
            listFavoriteController.getFavorite();
            activityIndicator.start();
        }
	}
}
