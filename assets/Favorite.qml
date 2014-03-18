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
                    type: "listItem"
                    
                    StandardListItem {
                        title: ListItemData.caption
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
