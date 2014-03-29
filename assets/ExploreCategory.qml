import bb.cascades 1.2
import Network.ExploreCategoryController 1.0

Page {
    id: pageCat
    property string  urlPage
    property string  caption
    
    
    Container {
        ActivityIndicator {
            id: activityIndicator
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 60
        }
        
        ListView {            
            objectName: "listCats"
            
            // ------------------------------------------------------------------
            // Pull to refresh
            signal refreshTriggered()
            property bool loading: false
            id: listCats
            leadingVisualSnapThreshold: 2.0
            leadingVisual: RefreshHeader {
                id: refreshHandler
                onRefreshTriggered: {
                    listCats.refreshTriggered();
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
                        layout: StackLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
                        verticalAlignment: VerticalAlignment.Top
                        Label {
                            text: ListItemData.title
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
                            Container {
                                id: pageNumContainter
                                function getFlag(flagCode) {
                                    switch(flagCode) {
                                        case 0:
                                            return "";
                                            
                                        case 1:
                                            return "asset:///images/icon_drap_participe.gif"
                                            
                                        case 2:
                                            return "asset:///images/icon_drap_lecture.gif"
                                            
                                        case 3:
                                            return "asset:///images/icon_favori.gif"
                                            
                                    }
                                    return "";
                                }
                                
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                horizontalAlignment: HorizontalAlignment.Left
                                ImageView {
                                    imageSource: pageNumContainter.getFlag(ListItemData.flagType)
                                    horizontalAlignment: HorizontalAlignment.Left
                                }
                                Label {
                                    text: ListItemData.pages
                                    horizontalAlignment: HorizontalAlignment.Right
                                    textStyle {
                                        base: SystemDefaults.TextStyles.SmallText
                                        color: Color.Gray
                                    }
                                }
                            }
                            
                        }
                        Divider {}
                    }
                }
            ]
            
            onRefreshTriggered: {
                activityIndicator.start();
                exploreCategoryController.listTopics(urlPage);
            }
        }
        
    }
    
    attachedObjects: [
        ExploreCategoryController {
            id: exploreCategoryController
            
            onComplete: {
                activityIndicator.stop()
            }
        }
    ] 
    
    
    onUrlPageChanged: {
        exploreCategoryController.setListView(listCats);
        exploreCategoryController.listTopics(urlPage);
        activityIndicator.start();
    }
}
