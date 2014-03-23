import bb.cascades 1.2
import Network.ShowThreadController 1.0
import com.netimage 1.0

Page {
    id: pageThread
    property string  urlPage
    property string  caption
        
    Container {
        
        ActivityIndicator {
            id: activityIndicator
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 200
        }
        
        ListView {
            id: threadView
            objectName: "threadView"
            
            dataModel: GroupDataModel {
                id: modelPost
            }
            
            listItemComponents: [
                                
                ListItemComponent {
                    type: "item"
                    
                        Container {
                            
                            layout: StackLayout {
                                orientation: LayoutOrientation.TopToBottom
                            }
                            
                            horizontalAlignment: HorizontalAlignment.Fill
                            Container {
                                background: Color.create(0.96,0.96,0.96);
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Fill
                                preferredWidth: 400
                                
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                ImageView {
                                    id: imgAvatar
                                    minHeight: 65
                                    maxHeight: 65
                                    minWidth: 120
                                    maxWidth: 120
                                    scalingMethod: ScalingMethod.AspectFit
                                    
                                    verticalAlignment: VerticalAlignment.Center
                                    image: tracker.image
                                    
                                    attachedObjects: [
                                        NetImageTracker {
                                            id: tracker
                                            source: ListItemData.avatar
                                       } 
                                    ]
                                    
                                }
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.TopToBottom
                                    }
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Fill
                                    
	                                Label {
	                                    text: ListItemData.author
	                                    textStyle.fontSize: FontSize.XSmall
	                                }
	                                Label {
	                                    text: ListItemData.timestamp
	                                    textStyle.fontSize: FontSize.XXSmall
	                                }
                                }
                                
                            }
                            Container {
                                background: Color.create("#00A7DE")
                                minHeight: 4
                                maxHeight: 4
                                minWidth: 800
                                maxWidth: 800
                            }
                            
                            PostRenderer {
                            }
                                                    
                        }                        
                } 
            ]
        }
    }
    
    attachedObjects: [
         ShowThreadController {
            id: showThreadController
            
            onComplete: {
                activityIndicator.stop();
            }
        }
    ]
    
    
    onUrlPageChanged: {
//        showThreadController.showThread(urlPage)
        showThreadController.setListView(threadView);
        showThreadController.showThread("/forum2.php?config=hfr.inc&cat=25&subcat=535&post=1197&page=875&p=1&sondage=0&owntopic=1&trash=0&trash_post=0&print=0&numreponse=0&quote_only=0&new=0&nojs=0")
        activityIndicator.start();
    }
}
