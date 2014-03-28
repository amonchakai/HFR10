import bb.cascades 1.2

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
	            
	            dataModel: XmlDataModel{
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
                    var page = categoryPage.createObject();
                    
                    // Set the url of the page to load and thread caption. 
                    page.urlPage = chosenItem.url
                    page.caption   = chosenItem.title
                    
                    nav.push(page);
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
}

