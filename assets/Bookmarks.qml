import bb.cascades 1.2
import bb.data 1.0
import Network.BookmarksController 1.0
import conf.settings 1.0


NavigationPane {
    id: nav
    property int     navDepth
    property variant tpage
    
    
    Page {
        
        Container {
            layout: DockLayout {
            
            }
            Container {
                layout: StackLayout {
                
                }
                
                ActivityIndicator {
                    id: activityIndicator
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Top
                    preferredHeight: 60
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
                    focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                    
                    dataModel: GroupDataModel {
                        id: theModel
                        sortingKeys: ["category"]
                        
                        property bool empty: true
                        
                        
                        onItemAdded: empty = isEmpty()  
                        onItemRemoved: empty = isEmpty()  
                        onItemUpdated: empty = isEmpty()  
                        
                        // You might see an 'unknown signal' error  
                        // in the QML-editor, guess it's a SDK bug.  
                        onItemsChanged: empty = isEmpty()
                    
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
                                function colorIndexToHex(index) {
                                    switch (index) {
                                        case 0:
                                            return Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "#000000" : "#ffffff" ;
                                        
                                        case 1:
                                            return "#a8cc0a";
                                        
                                        case 2:
                                            return "#27bbc6";
                                        
                                        case 3:
                                            return "#fe5e05";
                                        
                                        case 4:
                                            return "#ef0061";
                                        
                                        case 5:
                                            return "#795ae0";
                                        
                                        default:
                                            return "#ffffff";
                                    }
                                }
                                
                                id: titleContainer
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Fill
                                Container {
                                    id: tagContainer
                                    minWidth: 8
                                    maxWidth: 8
                                    verticalAlignment: VerticalAlignment.Fill
                                    background: Color.create(colorIndexToHex(ListItemData.color))
                                }
                                Container {
                                    minWidth: 2
                                    maxWidth: 2
                                    verticalAlignment: VerticalAlignment.Fill
                                }
                                
                                Container {
                                    id: topicContainer
                                    
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
                                                color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#00a8df") : Color.Blue
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
                                            title: qsTr("Last page")
                                            imageSource: "asset:///images/icon_next_all.png"
                                            onTriggered: {
                                                titleContainer.ListItem.view.gotoPage(ListItemData.urlLastPage, ListItemData.title)
                                            }
                                        }
                                        ActionItem {
                                            title: qsTr("Last page read")
                                            imageSource: "asset:///images/icon_next.png"
                                            onTriggered: {
                                                titleContainer.ListItem.view.gotoPage(ListItemData.urlLastPostRead, ListItemData.title)
                                            }
                                        }
                                                                                
                                        DeleteActionItem {
                                            title: qsTr("Remove flag")
                                            onTriggered: {
                                                titleContainer.ListItem.view.deleteFlag(ListItemData.urlFirstPage, titleContainer.ListItem.indexPath)
                                            }
                                        }
                                    }
                                ]
                            }
                        }
                    ]
                    
                    function gotoPage(urlFirstPage, titleTopic) {
                        if(!tpage) {
                            if(appSettings.threadInterface == 1)
                                tpage = threadPage.createObject();
                            else
                                tpage = threadPageNative.createObject();
                        }
                        
                        // Set the url of the page to load and thread caption. 
                        tpage.urlPage = urlFirstPage
                        tpage.caption   = titleTopic
                        
                        nav.push(tpage);
                    }
                    
                    function deleteFlag(urlFirstPage, indexPath) {
                        //listFavoriteController.deleteFlag(urlFirstPage);
                        listFav.dataModel.removeAt(indexPath);
                        bookmarksController.deleteBookmark(urlFirstPage);
                    }
                    
                    function openColorPicker(urlFirstPage, indexPath) {
                        if(!tColPage) {
                            tColPage = colorPickerPage.createObject();
                        }
                        
                        // Set the url of the page to load and thread caption. 
                        itemToTag = indexPath;
                        tColPage.urlPage = urlFirstPage
                        nav.push(tColPage);
                    
                    }
                    
                    onTriggered: {
                        var chosenItem = dataModel.data(indexPath);
                        
                        // Create the content page and push it on top to drill down to it.
                        if(!tpage) {
                            if(appSettings.threadInterface == 1)
                                tpage = threadPage.createObject();
                            else
                                tpage = threadPageNative.createObject();
                        }
                        
                        // Set the url of the page to load and thread caption. 
                        tpage.urlPage = chosenItem.urlLastPostRead
                        tpage.caption   = chosenItem.title
                        
                        nav.push(tpage);
                    }
                    
                    onRefreshTriggered: {
                        bookmarksController.getFavorite();
                    }
                
                }
            }
            Container {  
                id: dataEmptyLabel
                visible: theModel.empty && !activityIndicator.running //model.isEmpty() will not work  
                horizontalAlignment: HorizontalAlignment.Center  
                verticalAlignment: VerticalAlignment.Center  
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                ImageView {
                    imageSource: "asset:///images/icon_favorites.png"
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Center
                }
                
                Label {
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                    text: qsTr("Use the star to mark new content")  
                    textStyle.textAlign: TextAlign.Center  
                }
            
            }
            
            
            attachedObjects: [
                BookmarksController {
                    id: bookmarksController
                    
                    onComplete: {
                        activityIndicator.stop();
                    }
                    
                    onLoading: {
                        activityIndicator.start();
                    }
                }, 
                ComponentDefinition {
                    id: threadPage
                    source: "ThreadPage.qml"
                },
                ComponentDefinition {
                    id: threadPageNative
                    source: "ThreadPageNative.qml"
                },
                AppSettings {
                    id: appSettings
                }
            ]
        
        }
        
        onCreationCompleted: {
            
            bookmarksController.setListView(listFav);
            bookmarksController.getFavorite();
            navDepth = 0;

        }
    }
    
    onPopTransitionEnded: {
        --navDepth;
        
        if(navDepth == 1) {
            if(appSettings.autoRefresh) {
                bookmarksController.getFavorite();
            }
            
        }
    }
    
    onPushTransitionEnded: {
        ++navDepth;
    }
}
