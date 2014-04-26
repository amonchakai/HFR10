import bb.cascades 1.2
import Network.ShowThreadController 1.0
import com.netimage 1.0
import bb.system 1.2

Page {
    id: pageThread
    property variant tpage
    property variant recursivePage
    
    property string  urlPage
    property string  caption
    property bool 	 needUpdate
    property string  tentativeNewURL
            
    
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
            
            property bool surveyVisible
            leadingVisualSnapThreshold: 2.0
            leadingVisual: SurveyHeader {
                id: surveyHeader
            }
            
            
            multiSelectHandler {
                actions: [
                    ActionItem {
                        title: "Reply to quotes"
                        imageSource: "asset:///images/icon_quote_bis.png"
                        
                        onTriggered: {
                            var selection = threadView.selectionList()
                            threadView.clearSelection();
                            
                            threadView.quoteSelection(selection);
                        }
                    }
                ]
                
            }
            
            function quoteSelection(selection) {
                var quoteURL = "";
                for(var i = selection.length-1 ; i >= 0  ; --i) {
                    quoteURL = quoteURL + "&cat=" + showThreadController.catID + "&post=" + showThreadController.postID + "&numrep=" + dataModel.data(selection[i]).index.toString();
                }
                
                if(!tpage)
                	tpage = postPage.createObject();                
                tpage.quoteURL = quoteURL;
                nav.push(tpage);
                
            }
            listItemComponents: [
                                
                ListItemComponent {
                    type: "item"
                        
                        Container {
                            id: headerContainer
                            
                            ListItem.onSelectionChanged: {
                                if(ListItem.selected)
                                    lineContainer.background = Color.create("#B00000");
                                else 
                                    lineContainer.background = Color.create("#00A7DE");
                            }
                            
                            function themeStyleToHeaderColor(style){
                                switch (style) {
                                    case VisualStyle.Bright:
                                        return Color.create(0.96,0.96,0.96);
                                    case VisualStyle.Dark: 
                                        return Color.create(0.15,0.15,0.15);
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
                                id: lineContainer
                                background: Color.create("#00A7DE") 
                                minHeight: 4
                                maxHeight: 4
                                horizontalAlignment: HorizontalAlignment.Fill
                            }
                            
                            PostRenderer {
                            }
                            
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Actions")
                                    
                                    ActionItem {
                                        title: qsTr("Edit")
                                        imageSource: "asset:///images/icon_write.png"
                                        onTriggered: {
                                            headerContainer.ListItem.view.gotoEditMessage(ListItemData.editUrl);
                                        }
                                    }
                                    ActionItem {
                                        title: qsTr("Add Favorite")
                                        imageSource: "asset:///images/icon_favorites.png"
                                        onTriggered: {
                                            headerContainer.ListItem.view.addToFavorite(ListItemData.index);
                                        }
                                    }
                                    ActionItem {
                                        title: qsTr("Send PM")
                                        imageSource: "asset:///images/icon_mp.png"
                                        onTriggered: {
                                            headerContainer.ListItem.view.sendPrivateMessage(ListItemData.author);
                                        }
                                    }
                                    ActionItem {
                                        title: qsTr("Quote")
                                        imageSource: "asset:///images/icon_quote_bis.png"
                                        onTriggered: {
                                            headerContainer.ListItem.view.gotoSingleQuoteMessage(ListItemData.index);
                                        }
                                    }
                                    MultiSelectActionItem {
                                        title: qsTr("Quote more")
                                        onTriggered: {
                                            headerContainer.ListItem.view.multiSelectHandler.active = true;
                                        }
                                    }
                                    DeleteActionItem {
                                        title: qsTr("Delete post")
                                        onTriggered: {
                                            headerContainer.ListItem.view.deletePost(ListItemData.index, ListItemData.author);
                                        }
                                    }
                                }
                            ]                       
                                
                    }                
                } 
            ]
            
            function gotoSingleQuoteMessage(messageID) {
                var quoteURL = "&cat=" + showThreadController.catID + "&post=" + showThreadController.postID + "&numrep=" + messageID.toString();
        
                if(!tpage)
                	tpage = postPage.createObject();                
                tpage.quoteURL = quoteURL;
                nav.push(tpage);
            }
            
            function deletePost(messageID, author) {
                if(author == showThreadController.pseudo)
                    showThreadController.deletePost(messageID);
            }
            
            function gotoEditMessage(urlEditPage) {
                if(urlEditPage == "")
                	return;
                	
                if(!tpage)
                	tpage = postPage.createObject();
                
                // Set the url of the page to load and thread caption. 
                tpage.editURL = urlEditPage
                
                nav.push(tpage);
            }
            
            function sendPrivateMessage(dest) {
                if(dest == "")
                    return;
                
                if(!tpage)
                    tpage = postPage.createObject();
                
                // Set the url of the page to load and thread caption. 
                tpage.recipient = dest;
                tpage.pseudo = showThreadController.pseudo;
                tpage.addSignature= showThreadController.sign;
                tpage.hashCheck = showThreadController.hashCheck 
                tpage.mode = 4;
                
                nav.push(tpage);
            }
            
            function pushNewUrl(urlPage) {
                if(!recursivePage)
                    recursivePage = recPageDef.createObject();
                                    
                recursivePage.urlPage = urlPage
                nav.push(recursivePage);
            }
            
            function addToFavorite(responseID) {
                showThreadController.addToFavorite(responseID);
            }
            
            function invokeWebBrowser(urlPage) {
                tentativeNewURL = urlPage;
                leaveAppDialog.show();
            }
            
            function notifyWebViewLoaded() {
                showThreadController.notifyItemLoaded();
            }
            
            function redirectWithinApp(newUrl) {
                urlPage = newUrl;
            }
        }
    }
    
    attachedObjects: [
         ShowThreadController {
            id: showThreadController
            
            onComplete: {
                activityIndicator.stop();
                if(showThreadController.isLastPage()) {
                    nextPageAction.title = qsTr("Refresh");
                    nextPageAction.imageSource = "asset:///images/icon_refresh.png"
                } else {
                    nextPageAction.title = qsTr("Next page");
                    nextPageAction.imageSource = "asset:///images/icon_next.png"
                }
            }
            
            onSurveyUpdated: {
                surveyHeader.setSurvey(showThreadController.survey);
            }
        },
        ComponentDefinition {
            id: postPage
            source: "PostMessage.qml"
            
        },
        ComponentDefinition {
            id: recPageDef
            source: "ThreadPage.qml"
        },
        Invocation {
            id: linkInvocation
            
            query {
                onUriChanged: {
                    linkInvocation.query.updateQuery();
                }
            }
            
            onArmed: {
                trigger("bb.action.OPEN");
            }
        },
        SystemDialog {
            id: leaveAppDialog
            title: qsTr("Friendly warning")
            body: qsTr("You are going to leave the application, do you want to continue?")
            onFinished: {
                if(result == SystemUiResult.ConfirmButtonSelection)
                	linkInvocation.query.uri = tentativeNewURL;
            }
        }
    ]
    
    actions: [
        ActionItem {
            title: qsTr("Reply")
            imageSource: "asset:///images/icon_write.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if(!tpage)
                    tpage = postPage.createObject();
                
                tpage.hashCheck = showThreadController.hashCheck;
                tpage.postID = showThreadController.postID;
                tpage.page	= showThreadController.pages;
                tpage.catID	= showThreadController.catID;
                tpage.pseudo = showThreadController.pseudo;
                tpage.threadTitle = showThreadController.title;
                tpage.addSignature= showThreadController.sign;
                                
                nav.push(tpage);
                
                
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
            id: nextPageAction
        	title: qsTr("Next page")
        	imageSource: "asset:///images/icon_next.png"
            ActionBar.placement: ActionBarPlacement.OnBar
        	onTriggered: {
        		showThreadController.nextPage();
                activityIndicator.start();
            }
        },
        ActionItem {
            title: qsTr("To Top")
            imageSource: "asset:///images/icon_top.png"
            onTriggered: {
                threadView.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.Default);
            }
        },
        ActionItem {
            title: qsTr("To Bottom")
            imageSource: "asset:///images/icon_bottom.png"
            onTriggered: {
                threadView.scrollToPosition(ScrollPosition.End, ScrollAnimation.Default);
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
        	showThreadController.lastPage(true);
        	activityIndicator.start();
            needUpdate = false;
        } 
    } 
}
