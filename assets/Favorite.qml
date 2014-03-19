import bb.cascades 1.2
import bb.data 1.0
import Network.ListFavoriteController 1.0


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
/*                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
                        verticalAlignment: VerticalAlignment.Fill
                        Label {
                            text: ListItemData.caption
                        }
                        
                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            horizontalAlignment: HorizontalAlignment.Fill
                            Label {
                                text: ListItemData.lastAuthor
                                textStyle {
                                    base: SystemDefaults.TextStyles.SmallText
                                    color: Color.Gray
                                }
                            }
                            Label {
                                text: ListItemData.timestamp
                                textStyle {
                                    base: SystemDefaults.TextStyles.SmallText
                                    color: Color.Gray
                                }
                            }
                        }
                        
                        
                    }*/

                    StandardListItem {
                        title: ListItemData.caption
                        //status: ListItemData.lastAuthor
                        description: ListItemData.timestamp + " - " +ListItemData.lastAuthor
                    }


                }
            ]
         }
        
         attachedObjects: [
             ListFavoriteController {
                 id: listFavoriteController
             }
         ]
         
    }
}
