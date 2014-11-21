import bb.cascades 1.2

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
            
	        ListView {
	            id: homeList
	            
	            dataModel: XmlDataModel {
	                source: "model/listCategories.xml"
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
	                                preferredWidth: 8
	                            }
	                            
	                            Label {
	                                id: itemText
	                                text: ListItemData.title
	                                verticalAlignment: VerticalAlignment.Center
	                                textStyle.base: SystemDefaults.TextStyles.TitleText
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
                    
                    // Set the url of the page to load and thread caption. 
                    tpage.subCatXml = chosenItem.xml
                    tpage.urlPage = chosenItem.url
                    tpage.caption   = chosenItem.title
                    
                    nav.push(tpage);
                }
                
                
                attachedObjects: [
                    ComponentDefinition {
                        id: categoryPage
                        source: "ExploreCategory.qml"
                    }
                ]
	        } // end view
	    } //end container
	} // end page

    onCreationCompleted: {
        navDepth = 0;
    }

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

