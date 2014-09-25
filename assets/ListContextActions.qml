import bb.cascades 1.2

Page {
    property string messageID
    
    ListView {
        id: subcatpicker
        accessibility.name: "listContextActions"
        
        dataModel: XmlDataModel {
            source: "model/ContextualActions.xml"
        }
        
        listItemComponents: [
            ListItemComponent {
                type: "listItem"
                Container {
                    Label {
                        text: ListItemData.name
                    }
                }
            }
        ]
        
        onTriggered: {
            forwardAction(indexPath[0]);
            nav.pop();
        }
    }
    
    function forwardAction(index) {
        switch(index) {
            case 0:
                showThreadController.addToFavorite(messageID);
                break;
            
            case 1:
                showThreadController.addBookmark(messageID);
                break;
        }
        
    }
}
