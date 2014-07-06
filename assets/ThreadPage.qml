import bb.cascades 1.2
import Network.ShowThreadController 1.0
import com.netimage 1.0
import bb.system 1.2

Page {
    id: pageThread
    property variant tpage
    property variant recursivePage
    property variant previewPage
    
    property string  urlPage
    property string  caption
    property bool 	 needUpdate
    property string  tentativeNewURL
    
    property int    scrollRequested
    
    Container {
        id: pageContainer
        background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "#000000" : "#ffffff" ;
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        horizontalAlignment: HorizontalAlignment.Fill

        ActivityIndicator {
            id: activityIndicator
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 60
        }

        Container {
            layout: DockLayout {
            }
            ScrollView {    
                rightMargin: 50
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                
                WebView {
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    html: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "<!DOCTYPE html><html><head><style>body { background-color: #000000; } </style></head><body></body></html>" : "" ;
                                        
                    id: threadWebView
                    settings.background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "#2E2E2E" : "#ffffff" ;
                    
                    
                    onNavigationRequested: {
                        if(request.navigationType != WebNavigationType.Other) {
                            request.action = WebNavigationRequestAction.Ignore;
                            
                            if(request.url.toString().substr(0,31) == "http://forum.hardware.fr/forum2") {
                                pageContainer.redirectWithinApp(request.url.toString().substring(24));
                            } else {
                                var urlTopic = RegExp("sujet_[0-9]+_[0-9]+.htm")
                                if(urlTopic.test(request.url.toString()))
                                    pageContainer.redirectWithinApp(request.url.toString().substring(24));
                                else {
                                    var urlImg = RegExp(".jpg");
                                    var urlImgPng = RegExp(".png");
                                    if(urlImg.test(request.url.toString()) || urlImgPng.test(request.url.toString()))
                                        pageContainer.showPictureViewer(request.url);         
                                    else
                                        pageContainer.invokeWebBrowser(request.url);
                                }
                            }
                        
                        } else { 
                            request.action = WebNavigationRequestAction.Accept;
                        }
                    }
                    
                    onMessageReceived: {
                            
                            
                        var isQuote = RegExp("QUOTE_MULTIPLES:")
                        var match = message.data.match(isQuote);
                        if(match) {
                            var listQuote = RegExp("([0-9]+,)")
                            var listValue = new Array();
                            
                            var str = message.data.substring(16);
                            var ret = str.match(listQuote);
                            while(ret) {
                                listValue.push(parseInt(ret[1].substring(0, ret[1].length-1)));
                                str = str.substring(ret[1].length+1);
                                ret = str.match(listQuote);
                            }
                            pageContainer.quoteSelection(listValue);
                        }                            
                        
                        var isOpenImg = RegExp("OPEN_IMAGE:([^\"]+)")
                        match = message.data.match(isOpenImg);
                        if(match)
                            pageContainer.showPictureViewer(match[1]);
                        
                        var isRedirect = RegExp("REDIRECT:([^\"]+)")
                        match = message.data.match(isRedirect);
                        if(match)
                            pageContainer.pushNewUrl(match[1]);
                            
                        var isSurvey = RegExp("SURVEY:([0-9]+)")
                        match = message.data.match(isSurvey);
                        if(match)
                            showThreadController.vote(match[1]);
                            
                        var isContext = RegExp("SHOW_CONTEXT:([0-9]+)");
                        match = message.data.match(isContext);
                        if(match)
                            pageContainer.showContextMenu(match[1]);
                            
                        isContext = RegExp("RANDOM_TAP");
                        match = message.data.match(isContext);
                        if(match)
                            pageContainer.closeContextMenu();
                        
                        console.log(message.data);
                    }
                    
                    settings.textAutosizingEnabled: false 
                    settings.zoomToFitEnabled: false  
    
                    
                    onLoadProgressChanged: {
                        if(loadProgress < 10)
                            scrollRequested = 0;
                   
                        if(loadProgress > 30) {
                            if(scrollRequested == 0) {
                                console.log("scroll 1")
                                pageContainer.notifyWebViewLoaded();
                                scrollRequested = 1;
                            }
                        }
                        if(loadProgress > 70) {
                            if(scrollRequested != 2) {
                                console.log("scroll 2")
                                pageContainer.notifyWebViewLoaded();
                                scrollRequested = 2;
                            }
                        }
                    }
                    
                    attachedObjects: [
                        OrientationHandler {
                            id: orientationHandler
                            
                            onOrientationChanged: {
                                threadWebView.evaluateJavaScript("orientationChanged();")
                            }
                            
                        }
                    ]         
                }  
            }
            
            Container {
                id: contextMenu
                layout: DockLayout {
                }
                background: Color.Black
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Right
                minWidth: 100
                maxWidth: 100
                
                property bool isVisible;
                property int  itemSelected;
                property bool multiSelect;
                
                
                Container {
                    layout: StackLayout {
                    }
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    
                    Container {
                        id: line1
                        minHeight: 2
                        maxHeight: 2
                        horizontalAlignment: HorizontalAlignment.Fill
                        background: Color.create("#333333")
                    }
                    
                    ImageButton {
                        id: editButton
                        defaultImageSource: "asset:///images/icon_write_context.png"
                        layoutProperties: StackLayoutProperties {
                        }
                        horizontalAlignment: HorizontalAlignment.Center
                        
                        onClicked: {
                            pageContainer.gotoEditMessage(showThreadController.getEditUrl(parseInt(contextMenu.itemSelected)));
                            contextMenu.close();
                        }
                        
                    }
                    
                    Container {
                        id: line2
                        minHeight: 2
                        maxHeight: 2
                        horizontalAlignment: HorizontalAlignment.Fill
                        background: Color.create("#333333")
                    }
                    
                    ImageButton {
                        id: addFavorite
                        defaultImageSource: "asset:///images/icon_favorites.png"
                        layoutProperties: StackLayoutProperties {
                        }
                        horizontalAlignment: HorizontalAlignment.Center
                        onClicked: {
                            pageContainer.addToFavorite(contextMenu.itemSelected);
                            contextMenu.close();
                        }
                    }
                    
                    Container {
                        id: line3
                        minHeight: 2
                        maxHeight: 2
                        horizontalAlignment: HorizontalAlignment.Fill
                        background: Color.create("#333333")
                    }
                    
                    ImageButton {
                        id: sendMP
                        defaultImageSource: "asset:///images/icon_mp.png"
                        layoutProperties: StackLayoutProperties {
                        }
                        horizontalAlignment: HorizontalAlignment.Center
                        onClicked: {
                            pageContainer.sendPrivateMessage(showThreadController.getMessageAuthor(parseInt(contextMenu.itemSelected)));
                            contextMenu.close();
                        }
                    }
                    
                    Container {
                        minHeight: 2
                        maxHeight: 2
                        horizontalAlignment: HorizontalAlignment.Fill
                        background: Color.create("#333333")
                    }
                    
                    ImageButton {
                        id: quote
                        defaultImageSource: "asset:///images/icon_quote_bis.png"
                        layoutProperties: StackLayoutProperties {
                        }
                        horizontalAlignment: HorizontalAlignment.Center
                        onClicked: {
                            if(!contextMenu.multiSelect) {
                                pageContainer.gotoSingleQuoteMessage(contextMenu.itemSelected);
                                contextMenu.close();
                            } else {
                                threadWebView.evaluateJavaScript("retrieveMultiselect();");
                                contextMenu.multiSelect = false;
                                contextMenu.close();
                            }
                        }
                    
                    }
                    
                    Container {
                        minHeight: 2
                        maxHeight: 2
                        horizontalAlignment: HorizontalAlignment.Fill
                        background: Color.create("#333333")
                    }
                    
                    ImageButton {
                        id: quoteMore
                        defaultImageSource: "asset:///images/icon_more.png"
                        layoutProperties: StackLayoutProperties {
                        }
                        horizontalAlignment: HorizontalAlignment.Center
                        
                        onClicked: {
                            threadWebView.evaluateJavaScript("startMultiselect();");
                            contextMenu.multiSelect = true;
                            visible = false;
                            editButton.visible = false;
                            sendMP.visible = false;
                            addFavorite.visible = false;
                            deleteButton.visible = false;
                            cancelButton.visible = true;
                            
                            line1.visible = false;
                            line2.visible = false;
                            line3.visible = false;
                            line4.visible = false;
                        }
                    
                    }
                    
                    Container {
                        id: line4
                        minHeight: 2
                        maxHeight: 2
                        horizontalAlignment: HorizontalAlignment.Fill
                        background: Color.create("#333333")
                    }
                }
                
                
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Bottom
                    preferredHeight: 100
                    
                    layout: DockLayout {
                    }
                    
                    Container {
                        minHeight: 2
                        maxHeight: 2
                        horizontalAlignment: HorizontalAlignment.Fill
                        background: Color.create("#333333")
                    }
                    
                    ImageButton {
                        id: deleteButton
                        defaultImageSource: "asset:///images/can.png"
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        onClicked: {
                            pageContainer.deletePost(contextMenu.itemSelected, showThreadController.getMessageAuthor(parseInt(contextMenu.itemSelected)));
                            contextMenu.close();
                        }
                        
                    }
                    
                    ImageButton {
                        id: cancelButton
                        defaultImageSource: "asset:///images/icon_dismiss.png"
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        visible: false
                        onClicked: {
                            contextMenu.multiSelect = false;
                            contextMenu.close();
                        }
                    }
                    
                    
                    Container {
                        minHeight: 2
                        maxHeight: 2
                        horizontalAlignment: HorizontalAlignment.Fill
                        background: Color.create("#333333")
                    }
                }
                
                
                function show() {
                    if(!isVisible) {
                        contextMenu.translationX = 0;
                        isVisible = true;
                        editButton.visible = true;
                        sendMP.visible = true;
                        addFavorite.visible = true;
                        quoteMore.visible = true;
                        deleteButton.visible = true;
                        cancelButton.visible = false;
                        line1.visible = true;
                        line2.visible = true;
                        line3.visible = true;
                        line4.visible = true;
                    }
                }
                
                function close() {
                    if(multiSelect) return;
                    
                    if(isVisible) {
                        contextMenu.translationX = contextMenu.minWidth;
                        isVisible = false;
                        threadWebView.evaluateJavaScript("unselectAll();")
                    }
                }
                
            }
                      
        }
        
        function showContextMenu(messageID) {
            console.log("show");
            contextMenu.itemSelected = messageID;
            contextMenu.show();
        }
        
        function closeContextMenu() {
            contextMenu.close();
        }
        
        function quoteSelection(selection) {
            var quoteURL = "";
            for(var i = selection.length-1 ; i >= 0  ; --i) {
                quoteURL = quoteURL + "&cat=" + showThreadController.catID + "&post=" + showThreadController.postID + "&numrep=" + selection[i].toString() + "&";
            }
            
            
            if(!tpage)
                tpage = postPage.createObject();                
            tpage.quoteURL = quoteURL;
            nav.push(tpage); 
        
        }
        
        function gotoSingleQuoteMessage(messageID) {
            var quoteURL = "&cat=" + showThreadController.catID + "&post=" + showThreadController.postID + "&numrep=" + messageID.toString() + "&";
            
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
        
        function showPictureViewer(imageUrl) {
            if(!previewPage)
                previewPage = imagePreview.createObject();
            previewPage.imageList = imageUrl;
            nav.push(previewPage);
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
        ComponentDefinition {
            id: imagePreview
            source: "ImagePreview.qml"
        },
        
        SystemDialog {
            id: leaveAppDialog
            title: qsTr("Friendly warning")
            body: qsTr("You are going to leave the application, do you want to continue?")
            onFinished: {
                if(result == SystemUiResult.ConfirmButtonSelection) {
                	linkInvocation.query.uri = tentativeNewURL;
                }
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
                scrollRequested = 0;
            }
        },
        ActionItem {
             title: qsTr("Last page")
             imageSource: "asset:///images/icon_next_all.png"
             onTriggered: {
                 showThreadController.lastPage();
                 activityIndicator.start();
                 scrollRequested = 0;
             }
        },
        ActionItem {
            title: qsTr("Prev page")
            imageSource: "asset:///images/icon_prev.png"
            onTriggered: {
                showThreadController.prevPage();
                activityIndicator.start();
                scrollRequested = 0;
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
                scrollRequested = 0;
            }
        },
        ActionItem {
            title: qsTr("To Top")
            imageSource: "asset:///images/icon_top.png"
            onTriggered: {
                threadWebView.evaluateJavaScript("scrollToTop();");
            }
        },
        ActionItem {
            title: qsTr("To Bottom")
            imageSource: "asset:///images/icon_bottom.png"
            onTriggered: {
                threadWebView.evaluateJavaScript("scrollToEndPage();");
            }
        }
    ]    
    
    onCreationCompleted: {
        contextMenu.translationX = contextMenu.minWidth;
        contextMenu.isVisible = false;
        showThreadController.setWebView(threadWebView);
    }
    
    onUrlPageChanged: {
        showThreadController.showThread(urlPage);
        scrollRequested = 0;

        activityIndicator.start();
    }
    
    onNeedUpdateChanged: {
        if(needUpdate) {
        	showThreadController.lastPage(true);
        	activityIndicator.start();
            scrollRequested = 0;
            needUpdate = false;
        } 
    } 
}
