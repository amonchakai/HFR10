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
            dataModel: dataFav
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    
                    StandardListItem {
                        title: ListItemData.title
                    }
                }
            ]
         }
        
         attachedObjects: [
             ListFavoriteController {
                 id: listFavoriteController
             },

             GroupDataModel {
                 id: dataFav
                 grouping: ItemGrouping.None
             },
             
             DataSource {
                 id: dataSource
                 
                 source: "model/feeds.xml"
                 query: "/*/td"
                 type: DataSourceType.Xml
                 
                 onDataLoaded: {
                     dataFav.clear();
                     dataFav.insertList(data)
                 }
             }
         ]
         
         onCreationCompleted: {
             dataSource.load();
         }
    }
}
