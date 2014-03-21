import bb.cascades 1.2
import Network.ShowThreadController 1.0

Page {
    property string  urlPage
    property string  caption
        
    Container {
        ListView {
            id: threadView
            objectName: "threadView"
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
                        
                    }
                    
                }
            ]
        }
    }
    
    attachedObjects: [
         ShowThreadController {
            id: showThreadController
        }
    ]
    
    
    onUrlPageChanged: {
//        showThreadController.showThread(urlPage)
        showThreadController.showThread("/forum2.php?config=hfr.inc&cat=25&subcat=535&post=1197&page=875&p=1&sondage=0&owntopic=1&trash=0&trash_post=0&print=0&numreponse=0&quote_only=0&new=0&nojs=0")
    }
}
