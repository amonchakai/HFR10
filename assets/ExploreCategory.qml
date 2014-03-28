import bb.cascades 1.2
import Network.ExploreCategoryController 1.0

Page {
    id: pageCat
    property string  urlPage
    property string  caption
    
    
    Container {
        ListView {            
            id: listCats
            objectName: "listCats"
            
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
        }
        
    }
    
    attachedObjects: [
        ExploreCategoryController {
            id: exploreCategoryController
        }
    ] 
    
    
    onUrlPageChanged: {
        exploreCategoryController.setListView(listCats);
        exploreCategoryController.listTopics(urlPage);
    }
}
