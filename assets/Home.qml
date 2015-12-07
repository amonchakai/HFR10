import bb.cascades 1.3
import Network.ExploreCategoryController 1.0

NavigationPane {
    id: nav
    property variant tpage
    
    property int     navDepth
    property variant focusedItem
    property variant listTopic
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
	        layout: DockLayout { }
	        
            ActivityIndicator {
                id: activityIndicator
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top
                preferredHeight: 60
            }
            
	        ListView {
	            id: homeList
                focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                signal refreshTriggered()
                property bool loading: false
                leadingVisualSnapThreshold: 2.0
                leadingVisual: RefreshHeader {
                    id: refreshHandler
                    onRefreshTriggered: {
                        homeList.refreshTriggered();
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
                
                onRefreshTriggered: {
                    activityIndicator.start();
                    exploreCategoryController.buildIndex();
                }
	            
	            
	            dataModel: GroupDataModel {	                
                    property bool empty: true
                    id: theModel
                    sortingKeys: ["group"]
                    grouping: ItemGrouping.ByFullValue
                    
                    onItemAdded: empty = isEmpty()  
                    onItemRemoved: empty = isEmpty()  
                    onItemUpdated: empty = isEmpty()  
                    
                    // You might see an 'unknown signal' error  
                    // in the QML-editor, guess it's a SDK bug.  
                    onItemsChanged: empty = isEmpty()
	            }
	            
	            listItemComponents: [
	                ListItemComponent {
	                    type: "item"
	                    
	                    CustomListItem {
	                        dividerVisible: true
	                        highlightAppearance: HighlightAppearance.Default
	                        
	                        Container {
	                            id: contentContainer
	                            layout: StackLayout {
	                                orientation: LayoutOrientation.LeftToRight
	                            }
	                            verticalAlignment: VerticalAlignment.Fill
	                            horizontalAlignment: HorizontalAlignment.Fill
	                            
	                            Container {
	                                preferredWidth: ui.du(2)
	                            }
	                            
	                            ImageView {
	                                verticalAlignment: VerticalAlignment.Center
	                                preferredHeight: ui.du(5)
	                                scalingMethod: ScalingMethod.AspectFit
	                                imageSource: ListItemData.icon
	                                
	                            }
	                            
	                            Label {
	                                id: itemText
	                                text: ListItemData.title
	                                verticalAlignment: VerticalAlignment.Center
	                                //textStyle.base: SystemDefaults.TextStyles.TitleText
	                                accessibilityMode: A11yMode.Collapsed 
	                            }    
	                            
	                        } // container in one ITEM
	                    } // custom item
	                } //end list ITEM
	            ] //end list
	            
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                                        
                    // Create the content page and push it on top to drill down to it.
                    if(!tpage)
                        tpage = categoryPage.createObject();
                    
                    console.log('chosen url: ' + chosenItem.url)
                    // Set the url of the page to load and thread caption. 
                    tpage.subCatXml = chosenItem.xml
                    tpage.cat     = chosenItem.catId
                    tpage.urlPage = chosenItem.url
                    tpage.caption   = chosenItem.title
                    
                    nav.push(tpage);
                }
	        } // end view
	        
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
	    } //end container
	    
        actions: [
            ActionItem {
                title: qsTr("Refresh")
                imageSource: "asset:///images/icon_refresh.png"
                onTriggered: {
                    activityIndicator.start();
                    exploreCategoryController.buildIndex();
                }
                
                shortcuts: [
                    Shortcut {
                        key: qsTr("r")
                    }
                ]
            }
        ]
        
	    
        onCreationCompleted: {
            navDepth = 0;
            
            exploreCategoryController.setForumRootListView(homeList);
            exploreCategoryController.loadIndex();
        }
        
        attachedObjects: [
            ExploreCategoryController {
                id: exploreCategoryController
                
                onCatLoaded: {
                    activityIndicator.stop();
                    homeList.requestFocus();
                }
            
            }, 
            ComponentDefinition {
                id: categoryPage
                source: "ExploreCategory.qml"
            }
        ]
        
	} // end page

    

    onPopTransitionEnded: {
        --navDepth;
        
        // Trackpad enabled devices: the listview must have the focus to catch scroll events!
        switch(navDepth) {
            case 1:
                homeList.requestFocus();
                break;                
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
        
        // should have been an array... :/
        switch(navDepth) {
            case 1:
                homeList.requestFocus();
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

