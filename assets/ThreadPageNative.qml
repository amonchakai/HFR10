import bb.cascades 1.3
import Network.ShowThreadController 1.0
import com.netimage 1.0
import bb.system 1.2

Page {
    id: pageThread
    property variant tpage
    property variant recursivePage
    property variant previewPage
    property variant statPage
    property variant contextActionPage
    property variant profilePage
    
    property string  urlPage
    property string  caption
    property bool 	 needUpdate
    
    property int    scrollRequested
    property string listItemSelected;
    
    
    function isBBPassport() {
        return DisplayInfo.width == 1440 && DisplayInfo.height == 1440;
    }
    
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
            id: webviewContainer
            layout: DockLayout {
            }
            ScrollView {    
                rightMargin: 50
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                id: scrollView
                focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                
                
                WebView {
                    //visible: false
                    horizontalAlignment: HorizontalAlignment.Fill
                    focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                    
                    html: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "<!DOCTYPE html><html><head><style>body { background-color: #000000; } </style></head><body></body></html>" : "" ;
                    
                    id: threadWebView
                    settings.background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "#2E2E2E" : "#ffffff" ;
                    
                    gestureHandlers: [
                        DoubleTapHandler {
                            onDoubleTapped: {
                                showThreadController.nextPage();
                            }
                        }
                    ]
                    
                    onNavigationRequested: {
                        if(request.navigationType != WebNavigationType.Other) {
                            request.action = WebNavigationRequestAction.Ignore;
                            
                            if(request.url.toString().substr(0,31) == "http://forum.hardware.fr/forum2") {
                                pageContainer.pushNewUrl(request.url.toString().substring(24));
                            } else {
                                
                                var urlTopic = RegExp("sujet_[0-9]+_[0-9]+.htm")
                                if(urlTopic.test(request.url.toString()))
                                    pageContainer.pushNewUrl(request.url.toString().substring(24));
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
                            listValue.reverse();
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
                        
                        var isScroll = RegExp("SCROLLTO:([0-9]+)")
                        match = message.data.match(isScroll);
                        if(match)
                            scroll(match[1]);
                        
                        console.log(message.data);    
                        var isContext = RegExp("SHOW_CONTEXT:([0-9]+)");
                        match = message.data.match(isContext); 
                        if(match) {
                            var own = message.data.substr(14+match[1].length)
                            var isOwn = showThreadController.getEditUrl(parseInt(match[1])) != "";
                            pageContainer.showContextMenu(match[1], isOwn);
                        }
                        isContext = RegExp("RANDOM_TAP");
                        match = message.data.match(isContext);
                        if(match) {
                            if(nextPageAction.title != qsTr("Cancel")) { // if the multi-select is on, do not unselect all after a tap
                                pageContainer.closeContextMenu();
                            }
                        }
                        
                        console.log(message.data);
                    }
                    
                    function scroll(position) {
                        scrollView.scrollToPoint(0, position, ScrollAnimation.None);
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
                        
                        //if(loadProgress > 40) {
                        //visible = true;
                        //     activityIndicator.stop();
                        //}
                    }
                    
                    onLoadingChanged: {
                        if (loadRequest.status == WebLoadStatus.Succeeded) {
                            pageContainer.notifyWebViewLoaded();
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
            
            // container to handle context menu
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
            console.log("messageID: " + messageID)
            var quoteURL = "&cat=" + showThreadController.catID + "&post=" + showThreadController.postID + "&numrep=" + messageID.toString() + "&";
            
            if(!tpage)
                tpage = postPage.createObject();                
            tpage.quoteURL = quoteURL;
            nav.push(tpage);
        }
        
        function deletePost(messageID, author) {
            //if(author == showThreadController.pseudo) {
            showThreadController.deletePost(messageID);
            //}
        }
        
        function gotoEditMessage(urlEditPage) {
            console.log("urlEditPage: " + urlEditPage)
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
                
        function openContextActionPage(messageID) {
            console.log('More context actions!')
            if(!contextActionPage)
                contextActionPage = contextPage.createObject();
            contextActionPage.messageID = messageID;
            nav.push(contextActionPage);
            console.log('Page pushed!')
        }
        
        function invokeWebBrowser(urlPage) {
            showThreadController.invokeBrowser(urlPage);
        }
        
        function notifyWebViewLoaded() {
            showThreadController.notifyItemLoaded();
        }
        
        function showPictureViewer(imageUrl) {
            if(!previewPage)
                previewPage = imagePreview.createObject();
            previewPage.imageList = imageUrl;
            nav.push(previewPage);
        }   
        
        function showContextMenu(messageID, own) {
            listItemSelected = (messageID);
            console.log(own)
            if(own == 1) {
                replyAction.title = qsTr("Edit");
                replyAction.imageSource = "asset:///images/ic_compose.png";
                
                nextPageAction.title = qsTr("Quote");
                nextPageAction.imageSource = "asset:///images/icon_quote.png";
                
                surveyAction.title = qsTr("Delete post");
                surveyAction.imageSource = "asset:///images/ic_delete.png";
                surveyAction.enabled = true;
            } else {
                replyAction.title = qsTr("Quote");
                replyAction.imageSource = "asset:///images/icon_quote.png";
                
                nextPageAction.title = qsTr("PM");
                nextPageAction.imageSource = "asset:///images/icon_mp.png";
            }
            
            prevPageAction.title = qsTr("Favorite");
            prevPageAction.imageSource = "asset:///images/icon_favorites.png";
            
            firstPageAction.title = qsTr("Quote more");
            firstPageAction.imageSource = "asset:///images/ic_select_more.png";
            
            lastPageAction.title = qsTr("Profile");
            lastPageAction.imageSource = "asset:///images/icon_contact.png";
            
            
        }
        
        function closeContextMenu() {
            
            replyAction.title = qsTr("Reply");
            replyAction.imageSource = "asset:///images/ic_compose.png";
            
            if(showThreadController.isLastPage()) {
                nextPageAction.title = qsTr("Refresh");
                nextPageAction.imageSource = "asset:///images/icon_refresh.png";
            } else {
                nextPageAction.title = qsTr("Next page");
                nextPageAction.imageSource = "asset:///images/icon_next.png";
            }
            
            if(!showThreadController.emptySurvey && showThreadController.actionSurvey) {
                prevPageAction.title = qsTr("Survey");
                prevPageAction.imageSource = "asset:///images/icon_stats.png";
                
                surveyAction.title = qsTr("Prev page");
                surveyAction.imageSource = "asset:///images/icon_prev.png";
            } else {
                prevPageAction.title = qsTr("Prev page");
                prevPageAction.imageSource = "asset:///images/icon_prev.png";
                
                surveyAction.title = qsTr("Survey");
                surveyAction.imageSource = "asset:///images/icon_stats.png";
            }
            surveyAction.enabled = !showThreadController.emptySurvey;
            
            
            firstPageAction.title = qsTr("First page");
            firstPageAction.imageSource = "asset:///images/icon_prev_all.png";
            
            lastPageAction.title = qsTr("Last page");
            lastPageAction.imageSource = "asset:///images/icon_next_all.png";
            
            threadWebView.evaluateJavaScript("unselectAll();");
        }
    }
    
    attachedObjects: [
        ShowThreadController {
            id: showThreadController
            
            onComplete: {
                activityIndicator.stop();

                if(showThreadController.isLastPage()) {
                    nextPageAction.title = qsTr("Refresh");
                    nextPageAction.imageSource = "asset:///images/icon_refresh.png";
                } else {
                    nextPageAction.title = qsTr("Next page");
                    nextPageAction.imageSource = "asset:///images/icon_next.png";
                }
                
                if(!showThreadController.emptySurvey && showThreadController.actionSurvey) {
                    prevPageAction.title = qsTr("Survey");
                    prevPageAction.imageSource = "asset:///images/icon_stats.png";
                    
                    surveyAction.title = qsTr("Prev page");
                    surveyAction.imageSource = "asset:///images/icon_prev.png";
                } else {
                    prevPageAction.title = qsTr("Prev page");
                    prevPageAction.imageSource = "asset:///images/icon_prev.png";
                    
                    surveyAction.title = qsTr("Survey");
                    surveyAction.imageSource = "asset:///images/icon_stats.png";
                }
                surveyAction.enabled = !showThreadController.emptySurvey;
                

                scrollView.requestFocus();
            
            }
            
            onNotifyPage: {
                eMessage.open();
            }
            
            onSearchStarted: {
                activityIndicator.start();
            }
        
        
        
        },
        ComponentDefinition {
            id: postPage
            source: "PostMessage.qml"
        
        },
        ComponentDefinition {
            id: recPageDef
            source: "ThreadPageNative.qml"
        },
        ComponentDefinition {
            id: imagePreview
            source: "ImagePreview.qml"
        },
        ComponentDefinition {
            id: contextPage
            source: "ListContextActions.qml"
        },
        ComponentDefinition {
            id: surveyPage
            source: "Survey.qml"
        },
        Delegate {
            id: eMessageDelegate
            source: "EMessage.qml"
        
        },
        ComponentDefinition {
            id: profile
            source: "Profile.qml"
        }, 
        Sheet {
            id: eMessage
            content: eMessageDelegate.object
            onOpenedChanged: {
                if (opened) {
                    eMessageDelegate.active = true;
                }
            }
            onClosed: {
                eMessageDelegate.active = false;
            }
        }
    ]
    
    actions: [
        ActionItem {
            id: replyAction
            title: qsTr("Reply")
            imageSource: "asset:///images/ic_compose.png"
            ActionBar.placement: ActionBarPlacement.Signature
            onTriggered: {
                console.log("replyAct!");
                if(replyAction.title == qsTr("Reply")) {
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
                
                if(replyAction.title == qsTr("Quote")) {
                    if(nextPageAction.title == qsTr("Cancel")) {
                        threadWebView.evaluateJavaScript("retrieveMultiselect();");
                    } else {
                        pageContainer.gotoSingleQuoteMessage(listItemSelected);
                    }
                    
                    pageContainer.closeContextMenu();
                }
            
                if(replyAction.title == qsTr("Edit")) {
                    pageContainer.gotoEditMessage(showThreadController.getEditUrl(parseInt(listItemSelected)));
                    pageContainer.closeContextMenu();
                }
            
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("r")
                }
            ]
        },
        ActionItem {
            id: firstPageAction
            title: qsTr("First page")
            imageSource: "asset:///images/icon_prev_all.png"
            onTriggered: {
                
                if(firstPageAction.title == qsTr("First page")) {
                    showThreadController.firstPage();
                    activityIndicator.start();
                    scrollRequested = 0;
                }
                
                if(firstPageAction.title == qsTr("Quote more")) {
                   
                    replyAction.title = qsTr("Quote");
                    replyAction.imageSource = "asset:///images/icon_quote.png";
                    
                    nextPageAction.title = qsTr("Cancel");
                    nextPageAction.imageSource = "asset:///images/ic_cancel.png";
                    
                    threadWebView.evaluateJavaScript("startMultiselect();");
                    
                }
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("f")
                }
            ]
        },
        ActionItem {
            id: lastPageAction
            title: qsTr("Last page")
            imageSource: "asset:///images/icon_next_all.png"
            onTriggered: {
                
                if(lastPageAction.title == qsTr("Last page")) {
                    showThreadController.lastPage();
                    activityIndicator.start();
                    scrollRequested = 0;
                }
                
                if(lastPageAction.title == qsTr("Profile")) {
                    if(!profilePage)
                        profilePage = profile.createObject();
                    
                    profilePage.profileUrl = showThreadController.getProfileUrl(parseInt(listItemSelected))
                    nav.push(profilePage);
                    pageContainer.closeContextMenu();
                }
                
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("l")
                }
            ]
        },
        ActionItem {
            id: surveyAction
            imageSource: "asset:///images/icon_stats.png"
            onTriggered: {
                
                if(surveyAction.title == qsTr("Prev page")) {
                    showThreadController.prevPage();
                    activityIndicator.start();
                    scrollRequested = 0;
                }
                
                if(surveyAction.title == qsTr("Survey")) {
                    if(!statPage)
                        statPage = surveyPage.createObject();
                    
                    statPage.survey = showThreadController.survey;
                    
                    nav.push(statPage);
                }
                
                if(surveyAction.title == qsTr("Delete post")) {
                    pageContainer.deletePost(listItemSelected, showThreadController.getMessageAuthor(parseInt(listItemSelected)));
                    pageContainer.closeContextMenu();
                }

            }
        },
        ActionItem {
            id: prevPageAction
            title: qsTr("Prev page")
            imageSource: "asset:///images/icon_prev.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                
                if(prevPageAction.title == qsTr("Prev page")) {
                    showThreadController.prevPage();
                    activityIndicator.start();
                    scrollRequested = 0;
                }
                
                if(prevPageAction.title == qsTr("Survey")) {
                    if(!statPage)
                        statPage = surveyPage.createObject();
                    
                    statPage.survey = showThreadController.survey;
                    
                    nav.push(statPage);
                }
                
                if(prevPageAction.title == qsTr("Favorite")) {
                    pageContainer.openContextActionPage(listItemSelected);
                    pageContainer.closeContextMenu();
                }
                
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("p")
                }
            ]
        },
        ActionItem {
            id: nextPageAction
            title: qsTr("Next page")
            imageSource: "asset:///images/icon_next.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                
                if(nextPageAction.title == qsTr("Next page")) {
                    showThreadController.nextPage();
                    activityIndicator.start();
                    scrollRequested = 0;
                }
                
                if(nextPageAction.title == qsTr("Quote")) {
                    pageContainer.gotoSingleQuoteMessage(listItemSelected);
                    pageContainer.closeContextMenu();
                }
                
                if(nextPageAction.title == qsTr("PM")) {
                    pageContainer.sendPrivateMessage(showThreadController.getMessageAuthor(parseInt(listItemSelected)));
                    pageContainer.closeContextMenu();
                }
                
                if(nextPageAction.title == qsTr("Cancel")) {
                    pageContainer.closeContextMenu();
                }
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("n")
                }
            ]
        },
        ActionItem {
            id: toTopAction
            title: qsTr("To Top")
            imageSource: "asset:///images/icon_top.png"
            onTriggered: {
                threadWebView.evaluateJavaScript("scrollToTop();");
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("t")
                }
            ]
        },
        ActionItem {
            title: qsTr("To Bottom")
            imageSource: "asset:///images/icon_bottom.png"
            onTriggered: {
                threadWebView.evaluateJavaScript("scrollToEndPage();");
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("b")
                }
            ]
        },
        ActionItem {
            title: qsTr("Clear cache")
            imageSource: "asset:///images/color/Clean_BW.png"
            onTriggered: {
                threadWebView.storage.clear();
            }
        },
        ActionItem {
            title: qsTr("Copy link")
            imageSource: "asset:///images/color/copy_BW.png"
            onTriggered: {
                showThreadController.doAction(11);
            }
        },
        ActionItem {
            title: qsTr("Intra. search")
            imageSource: "asset:///images/color/ic_search.png"
            onTriggered: {
                showThreadController.doAction(12);
            }
        }
    ]    
    onActionBarVisibilityChanged: {
        focusedItem.requestFocus();
    }
    
    onCreationCompleted: {
        showThreadController.setWebView(threadWebView);
        focusedItem = scrollView;
        focusedItemDepth = 1;
    }
    
    onUrlPageChanged: {
        showThreadController.showThread(urlPage);
        scrollRequested = 0;
        activityIndicator.start();
    }
    
    onNeedUpdateChanged: {
        if(needUpdate) {
            showThreadController.lastPage(true, true);
            activityIndicator.start();
            scrollRequested = 0;
            needUpdate = false;
        } 
    } 

}
