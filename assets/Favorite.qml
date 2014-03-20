import bb.cascades 1.2
import bb.data 1.0
import Network.ListFavoriteController 1.0


NavigationPane {
    id: nav
    Page {
	    Container {
	        
	        
	        
	        Button {
	            text: "load"
	            horizontalAlignment: HorizontalAlignment.Fill
	            onClicked: {
	                listFavoriteController.getFavorite()
	            }
	        }
	        
	        ListView {
	            id: listFav
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
	                        verticalAlignment: VerticalAlignment.Fill
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
	                
	                // Set the title and source of the feed that the user selected. 
	                page.urlPage = chosenItem.urlFirstPost
	                page.title   = chosenItem.caption
	                
	                nav.push(page);
	            }
	            
	         }
	        
	         attachedObjects: [
	             ListFavoriteController {
	                 id: listFavoriteController
	             }, 
	             ComponentDefinition {
	                 id: threadPage
	                 source: "ThreadPage.qml"
	             }
	         ]
	         
	    }
	}
}
