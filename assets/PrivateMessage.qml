import bb.cascades 1.2
import Network.PrivateMessageController 1.0
import Lib.QTimer 1.0
import bb.system 1.2

NavigationPane {
    id: nav
    property variant tpage
    
    property int     navDepth
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
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                
                                Container {
                                    preferredWidth: 8
                                }
                                
        	                    Container {
        	                        
        	                        
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
                                                color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#00a8df") : Color.Blue
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
                                            title: qsTr("Last page read")
                                            imageSource: "asset:///images/icon_next.png"
                                            onTriggered: {
                                                titleContainer.ListItem.view.gotoPage(ListItemData.urlLastPage, ListItemData.title)
                                            }
                                        }
                                        
                                        DeleteActionItem {
                                            title: qsTr("Delete PM")
                                            onTriggered: {
                                                titleContainer.ListItem.view.detetePM(ListItemData.urlLastPage, titleContainer.ListItem.indexPath);
                                            }
                                        }
                                    }
                                ]
    	                    }
    	                
    	                }
    	            ]
    	            
                    function gotoPage(urlFirstPage, titleTopic) {
                        if(!tpage)
                        	tpage = threadPage.createObject();
                        
                        // Set the url of the page to load and thread caption. 
                        tpage.urlPage = urlFirstPage
                        tpage.caption   = titleTopic
                        
                        nav.push(tpage);
                    }
                    
                    function detetePM(urlLastPage, indexPath) {                        
                        deleteToast.who = urlLastPage;
                        deleteToast.idxPath = indexPath;
                        deleteToast.dismissed = false;
                        deleteToast.show();
                        timerDelete.start();
                    }
    	            
    	            onTriggered: {
    	                var chosenItem = dataModel.data(indexPath);
    	                
    	                // Create the content page and push it on top to drill down to it.
                        if(!tpage)
    	                   tpage = threadPage.createObject();
    	                
    	                // Set the url of the page to load and thread caption. 
                       tpage.urlPage = chosenItem.urlLastPage
                       tpage.caption   = chosenItem.title
    	                
                       nav.push(tpage);
    	            }
    	            
    	            onRefreshTriggered: {
    	                privateMessageController.getMessages();
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
	    
	    }
	    
	    attachedObjects: [
	        PrivateMessageController {
	            id: privateMessageController
	            
	            onComplete: {
	                activityIndicator.stop();
	                listMP.requestFocus();
	            }
	            
	            onLoading: {
                    activityIndicator.start();
	            }
	        }, 
	        ComponentDefinition {
	            id: threadPage
	            source: "ThreadPage.qml"
	        },
            SystemToast {
                id: deleteToast
                property string who
                property variant idxPath
                property bool dismissed
                body: qsTr("PM will be deleted") + Retranslate.onLanguageChanged
                button.label: qsTr("Undo")
                
                
                
                onFinished: {
                    console.log('undo' + value)
                    if(value == SystemUiResult.ButtonSelection){
                        dismissed = true;
                        // UNDO clicked - revert to normal Icon in this case
                    } else {
                        privateMessageController.deletePrivateMessage(who);
                        listMP.dataModel.removeAt(idxPath);
                    }
                }
            },
            QTimer {
                id: timerDelete
                singleShot: true
                interval: 3000
                
                onTimeout: {
                    if(!deleteToast.dismissed) {
                        deleteToast.cancel();
                        privateMessageController.deletePrivateMessage(deleteToast.who);
                        listMP.dataModel.removeAt(deleteToast.idxPath);
                    }
                }
            }
	    ] 
	    
	    onCreationCompleted: {
	        privateMessageController.setListView(listMP);
	        privateMessageController.load();
	        navDepth = 0;
            focusOnMPTab = listMP;
            pmController = privateMessageController;
	    }
	    
        actions: [
            ActionItem {
                title: qsTr("Refresh")
                imageSource: "asset:///images/icon_refresh.png"
                onTriggered: {
                    privateMessageController.getMessages();
                }
                
                shortcuts: [
                    Shortcut {
                        key: qsTr("r")
                    }
                ]
            }
        ]
	}
	
    onPopTransitionEnded: {
        --navDepth;
        
        
        privateMessageController.checkforUnreadMP();
        
        console.log('POP: ' + navDepth);
        // Trackpad enabled devices: the listview must have the focus to catch scroll events!
        switch(navDepth) {
            case 1:
                listMP.requestFocus();
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
                listMP.requestFocus();
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
