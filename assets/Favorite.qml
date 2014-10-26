import bb.cascades 1.2
import bb.data 1.0
import Network.ListFavoriteController 1.0
import conf.settings 1.0


NavigationPane {
    id: nav
    property int     navDepth
    property variant tpage
    property variant tColPage
    
    property variant itemToTag
    property int     chosenTag
    
    property variant focusedItem
    property variant focusedItemDepth
    property variant focusStage1
    property variant focusStage2
    property variant focusStage3
    property variant focusStage4
    property variant focusStage5
    property variant focusStage6
    property variant focusStage7
    
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
                                        
                                        ActionItem {
                                            title: qsTr("Color Tag")
                                            imageSource: "asset:///images/icon_feed.png"
                                            onTriggered: {
                                                titleContainer.ListItem.view.openColorPicker(ListItemData.urlFirstPage, titleContainer.ListItem.indexPath)
                                               
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
                            tpage = threadPage.createObject();
                        }
                        
                        // Set the url of the page to load and thread caption. 
                        tpage.urlPage = urlFirstPage
                        tpage.caption   = titleTopic
                        
                        nav.push(tpage);
                    }
                    
                    function deleteFlag(urlFirstPage, indexPath) {
                        listFavoriteController.deleteFlag(urlFirstPage);
                        listFav.dataModel.removeAt(indexPath);
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
                            tpage = threadPage.createObject();
                        }
    	                
    	                // Set the url of the page to load and thread caption. 
                        tpage.urlPage = chosenItem.urlLastPostRead
                        tpage.caption   = chosenItem.title

                        nav.push(tpage);
    	            }
    	            
    	            onRefreshTriggered: {
                        listFavoriteController.getFavorite();
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
                     imageSource: "asset:///images/pull-to-refresh.png"
                     horizontalAlignment: HorizontalAlignment.Left
                     verticalAlignment: VerticalAlignment.Center
                 }
                 
                 Label {
                     horizontalAlignment: HorizontalAlignment.Right
                     verticalAlignment: VerticalAlignment.Center
                     text: qsTr("Pull down to update")  
                     textStyle.textAlign: TextAlign.Center  
                 }
                 
             }
	         
	        
	         attachedObjects: [
	             ListFavoriteController {
	                 id: listFavoriteController
	                 
	                 onComplete: {
	                        activityIndicator.stop();
                            focusedItem = listFav;
	                        listFav.requestFocus();
	                 }
	                 
	                 onLoading: {
	                        activityIndicator.start();
	                 }
	             }, 
	             ComponentDefinition {
	                 id: threadPage
	                 source: "ThreadPage.qml"
	             },
                 AppSettings {
                     id: appSettings
                 },
                 ComponentDefinition {
                     id: colorPickerPage
                     source: "ColorPicker.qml"
                 }
	         ]
	         
	    }
	    
	    onCreationCompleted: {	        
            listFavoriteController.setListView(listFav);
            if(loginController.isLogged()) {
                listFavoriteController.load();
            }
            navDepth = 0;
            chosenTag = -1;
            
            listFav.requestFocus();
            
            focusOnFavTab = listFav;
        }
	}
    
    onPopTransitionEnded: {
        --navDepth;
        
        if(navDepth == 1) {
            if(appSettings.autoRefresh) {
            	listFavoriteController.getFavorite();
            }
            
            if(chosenTag != -1) {
                var selectedItem = listFav.dataModel.data(itemToTag);
                selectedItem.color = chosenTag;
            }
            chosenTag = -1;
            
             
        } 
        
        console.log('POP: ' + navDepth);
        
        // Trackpad enabled devices: the listview must have the focus to catch scroll events!
        switch(navDepth) {
            case 1:
                listFav.requestFocus();
            case 2:
                focusStage1.requestFocus();
                break;
            case 3:
                focusStage2.requestFocus();
                break;
            case 4:
                focusStage3.requestFocus();
                break;
            case 5:
                focusStage4.requestFocus();
                break;
            case 6:
                focusStage5.requestFocus();
                break;
            case 7:
                focusStage6.requestFocus();
                break;
            case 8:
                focusStage7.requestFocus();
                break;
        }
        
    }
    
    onPushTransitionEnded: {
        ++navDepth;
        console.log('PUSH: ' + navDepth + ' ITEM: ' + focusedItemDepth);
        
        // should have been an array... :/
        switch(navDepth) {
            case 1:
                listFav.requestFocus();
                break;
            case 2:
                if(focusedItemDepth == 1) {
                    focusStage1 = focusedItem;
                    focusedItemDepth = 0;
                    console.log('PUSH 2')
                }
                focusStage1.requestFocus();
                break;
            case 3:
                if(focusedItemDepth == 1) {
                    focusStage2 = focusedItem;
                    focusedItemDepth = 0;
                }
                focusStage2.requestFocus();
                break;
            case 4:
                if(focusedItemDepth == 1) {
                    focusStage3 = focusedItem;
                    focusedItemDepth = 0;
                }
                focusStage3.requestFocus();
                break;
            case 5:
                if(focusedItemDepth == 1) {
                    focusStage4 = focusedItem;
                    focusedItemDepth = 0;
                }
                focusStage4.requestFocus();
                break;
            case 6:
                if(focusedItemDepth == 1) {
                    focusStage5 = focusedItem;
                    focusedItemDepth = 0;
                }
                focusStage5.requestFocus();
                break;
            case 7:
                if(focusedItemDepth == 1) {
                    focusStage6 = focusedItem;
                    focusedItemDepth = 0;
                }
                focusStage6.requestFocus();
                break;
            case 8:
                if(focusedItemDepth == 8) {
                    focusStage7 = focusedItem;
                    focusedItemDepth = 0;
                }
                focusStage7.requestFocus();
                break;
        }
        
        
        
    }
    
}
