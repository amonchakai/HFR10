import bb.cascades 1.2
import Network.SearchController 1.0

Page {
    property string search
    property string author
    property int 	searchType
    property int 	categoryType
    property int 	searchIn
    property int 	numberOfMessages
    property int 	sortBy
    property bool 	refresh
    
    property variant tpage;
        
    
    Container {
        ActivityIndicator {
            id: activityIndicator
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 60
        }
        
        ListView {            
            objectName: "listCats"
            id: listCats
            
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
                            if(read)
                                return FontWeight.Bold;
                            else 
                                return FontWeight.Normal;
                        }
                        
                        layout: StackLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
                        verticalAlignment: VerticalAlignment.Top
                        Label {
                            text: ListItemData.title
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
                            Container {
                                id: pageNumContainter
                                
                                // ---------------------------------------------------------------
                                // layout functions...
                                
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
                                
                                
                                // ---------------------------------------------------------------
                                // list item itself...
                                
                                
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
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                
                // Create the content page and push it on top to drill down to it.
                if(!tpage) {
                    tpage = threadPage.createObject();
                }
                
                // Set the url of the page to load and thread caption. 
                if(chosenItem.urlLastPostRead != "")
                    tpage.urlPage = chosenItem.urlLastPostRead;
                else
                    tpage.urlPage = chosenItem.urlFirstPage;
                tpage.caption   = chosenItem.title
                
                nav.push(tpage);
            }        
        }
    
    }
    
    attachedObjects: [
        SearchController {
            id: searchController
            
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
        searchController.setListView(listCats);
    }
        
    onRefreshChanged: {
        if(refresh) {
	        activityIndicator.start();
	        searchController.search(search, 
	            					author, 
	            					searchType,
	            					categoryType,
	            					searchIn,
	            					numberOfMessages,
	            					sortBy);
	        refresh = false;
        } 
    }
    
}
