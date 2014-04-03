import bb.cascades 1.2
import Network.ShowThreadController 1.0
import com.netimage 1.0

Page {
    id: pageThread
    property string  urlPage
    property string  caption
    property bool 	 needUpdate
        
    Container {
        id: pageContainer

        ActivityIndicator {
            id: activityIndicator
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 60
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
                            id: headerContainer
                            
                            function themeStyleToHeaderColor(style){
                                switch (style) {
                                    case VisualStyle.Bright:
                                        return Color.create(0.96,0.96,0.96);
                                    case VisualStyle.Dark: 
                                        return Color.create(0.01,0.01,0.01);
                                    default :
                                        return Color.create(0.96,0.96,0.96);    
                                }
                                return Color.create(0.96,0.96,0.96); 
                            }
                            
                            layout: StackLayout {
                                orientation: LayoutOrientation.TopToBottom
                            }
                            
                            horizontalAlignment: HorizontalAlignment.Fill
                            Container {
                                background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
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
                            
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Navigation")
                                    
                                    ActionItem {
                                        title: qsTr("Edit")
                                        imageSource: "asset:///images/icon_write.png"
                                        onTriggered: {
                                            headerContainer.ListItem.view.gotoEditMessage(ListItemData.editUrl);
                                        }
                                    }
                                    ActionItem {
                                        title: qsTr("Quote")
                                        imageSource: "asset:///images/icon_check.png"
                                        onTriggered: {

                                        }
                                    }
                                    ActionItem {
                                        title: qsTr("Add Favorite")
                                        imageSource: "asset:///images/icon_favorites.png"
                                        onTriggered: {
                                            headerContainer.ListItem.view.addToFavorite(ListItemData.index);
                                        }
                                    }
                                    
                                    DeleteActionItem {
                                        title: qsTr("Delete post")
                                    }
                                }
                            ]
                                                    
                        }                        
                } 
            ]
            
            function gotoEditMessage(urlEditPage) {
                if(urlEditPage == "")
                	return;
                	
                var page = postPage.createObject();
                
                // Set the url of the page to load and thread caption. 
                page.editURL = urlEditPage
                
                nav.push(page);
            }
            
            function addToFavorite(responseID) {
                showThreadController.addToFavorite(responseID);
            }
        }
    }
    
    attachedObjects: [
         ShowThreadController {
            id: showThreadController
            
            onComplete: {
                activityIndicator.stop();
            }
        },
        ComponentDefinition {
            id: postPage
            source: "PostMessage.qml"
            
        }
    ]
    
    actions: [
        ActionItem {
            title: qsTr("Reply")
            imageSource: "asset:///images/icon_write.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                var page = postPage.createObject();
                
                page.hashCheck = showThreadController.hashCheck;
                page.postID = showThreadController.postID;
                page.page	= showThreadController.pages;
                page.catID	= showThreadController.catID;
                page.pseudo = showThreadController.pseudo;
                page.threadTitle = showThreadController.title;
                page.addSignature= showThreadController.sign;
                                
                nav.push(page);
                
                
            }
        },
        ActionItem {
            title: qsTr("First page")
            imageSource: "asset:///images/icon_prev_all.png"
            onTriggered: {
                showThreadController.firstPage();
                activityIndicator.start();
            }
        },
        ActionItem {
             title: qsTr("Last page")
             imageSource: "asset:///images/icon_next_all.png"
             onTriggered: {
                 showThreadController.lastPage();
                 activityIndicator.start();
             }
        },
        ActionItem {
            title: qsTr("Prev page")
            imageSource: "asset:///images/icon_prev.png"
            onTriggered: {
                showThreadController.prevPage();
                activityIndicator.start();
            }
        },
        ActionItem {
        	title: qsTr("Next page")
        	imageSource: "asset:///images/icon_next.png"
        	onTriggered: {
        		showThreadController.nextPage();
                activityIndicator.start();
            }
        }
        
    ]    
    
    onUrlPageChanged: {
        showThreadController.setListView(threadView);
        showThreadController.showThread(urlPage);

        activityIndicator.start();
    }
    
    onNeedUpdateChanged: {
        if(needUpdate) {
        	showThreadController.showThread(urlPage);
        	activityIndicator.start();
            needUpdate = false;
        } 
    } 
}
