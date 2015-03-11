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
    
    property string  urlPage
    property string  caption
    property bool 	 needUpdate
    property string  tentativeNewURL
    
    property int    scrollRequested
    property string listItemSelected;
    
    actionBarVisibility: ChromeVisibility.Hidden
    
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
                            console.log(match + " === " + match[1])
                            pageContainer.showContextMenu(match[1]);
                        }
                        isContext = RegExp("RANDOM_TAP");
                        match = message.data.match(isContext);
                        if(match)
                            pageContainer.closeContextMenu();
                        
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
            Container {
                id: contextMenu
                layout: DockLayout {
                }
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Right
                
                property bool isVisible;
                property bool multiSelect;
                                
                
                function show() {
                    if(!isVisible) {
                        isVisible = true;
                        editButton.visible = true;
                        sendMP.visible = true;
                        addFavorite.visible = true;
                        quote.visible = true;
                        quoteMore.visible = true;
                        deleteButton.visible = true;
                        cancelButton.visible = false;
                        
                        composeNewActionBar.visible = false;
                        nextPageNewActionBar.visible = false;
                        statsNewActionBar.visible = false;
                    }
                }
                
                function close() {
                    if(multiSelect) return;
                    
                    if(isVisible) {
                        
                        addFavorite.visible = false;
                        editButton.visible = false;
                        sendMP.visible = false;
                        quote.visible = false;
                        quoteMore.visible = false;
                        deleteButton.visible = false;
                        cancelButton.visible = false;
                        
                        isVisible = false;
                        threadWebView.evaluateJavaScript("unselectAll();")
                        
                        composeNewActionBar.visible = true;
                        nextPageNewActionBar.visible = true;
                        statsNewActionBar.visible = !showThreadController.emptySurvey;
                    }
                }                
            }     
        }
        
        
        function showContextMenu(messageID) {
            console.log("show");
            listItemSelected = (messageID);
            console.log(messageID + " -- " + parseInt(messageID) + " === " + listItemSelected)
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
        
        function addToFavorite(responseID) {
            //showThreadController.addToFavorite(responseID);
            console.log('Add to fav!')
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
            tentativeNewURL = urlPage;
            leaveAppDialog.show();
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
        
        
        Container {
            preferredHeight: ui.du(12)
            horizontalAlignment: HorizontalAlignment.Fill
            
            background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#262626") : Color.create("#f0f0f0")
            layout: DockLayout {
                
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Container {
                    preferredWidth: ui.du(2)
                    preferredHeight: ui.du(2)
                }
                
                ImageButton {
                    verticalAlignment: VerticalAlignment.Center
                    
                    preferredHeight: ui.du(10)
                    preferredWidth: ui.du(10)
                    
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_left_blue_black.png" : "asset:///images/icon_left_blue.png"
                    
                    onClicked: {
                        nav.pop();
                    }
                
                }

                ImageButton {
                    verticalAlignment: VerticalAlignment.Center
                    
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_feed.png" : "asset:///images/icon_feed_black.png"
                    
                    onClicked: {
                        actionPicker.toogleActionPicker();
                    }
                }
                
                Label {
                    verticalAlignment: VerticalAlignment.Center
                    text: qsTr("Actions")
                    textStyle.fontSize: FontSize.Small
                }
            }    
            
            Container {
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                // Action bar actions

                
                ImageButton {
                    verticalAlignment: VerticalAlignment.Center
                    id: composeNewActionBar
                    
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_write_rounded.png" : "asset:///images/icon_write_rounded_black.png"
                
                    onClicked: {
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
                }
                
                ImageButton {
                    verticalAlignment: VerticalAlignment.Center
                    id: statsNewActionBar
                    visible: false
                    
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_stats_rounded.png" : "asset:///images/icon_stats_rounded_black.png"
                    
                    onClicked: {
                        if(!statPage)
                            statPage = surveyPage.createObject();
                        
                        statPage.survey = showThreadController.survey;
                        
                        nav.push(statPage);
                    
                    }
                }
                
                ImageButton {
                    verticalAlignment: VerticalAlignment.Center
                    id: nextPageNewActionBar
                    
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_next_rounded.png" : "asset:///images/icon_next_rounded_black.png"
                    
                    onClicked: {
                        showThreadController.nextPage();
                        activityIndicator.start();
                        scrollRequested = 0;
                    }
                
                }
                
                
                // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                // Context menu
                
                ImageButton {
                    id: editButton
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_write_rounded.png" : "asset:///images/icon_write_rounded_black.png"
                    verticalAlignment: VerticalAlignment.Center
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    visible: false
                    
                    onClicked: {
                        pageContainer.gotoEditMessage(showThreadController.getEditUrl(parseInt(listItemSelected)));
                        contextMenu.close();
                    }
                
                }
                
                ImageButton {
                    id: addFavorite
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_favorite_rounded.png" : "asset:///images/icon_favorite_rounded_black.png"
                    verticalAlignment: VerticalAlignment.Center
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    visible: false

                    onClicked: {
                        pageContainer.openContextActionPage(listItemSelected);
                        contextMenu.close();
                    }
                }
                
                ImageButton {
                    id: sendMP
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_mp_rounded.png" : "asset:///images/icon_mp_rounded_black.png"
                    verticalAlignment: VerticalAlignment.Center
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    visible: false
                    
                    onClicked: {
                        pageContainer.sendPrivateMessage(showThreadController.getMessageAuthor(parseInt(listItemSelected)));
                        contextMenu.close();
                    }
                }
                
                ImageButton {
                    id: quote
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_quote_rounded.png" : "asset:///images/icon_quote_rounded_black.png"
                    verticalAlignment: VerticalAlignment.Center
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    visible: false
                    
                    onClicked: {
                        if(!contextMenu.multiSelect) {
                            pageContainer.gotoSingleQuoteMessage(listItemSelected);
                            contextMenu.close();
                        } else {
                            threadWebView.evaluateJavaScript("retrieveMultiselect();");
                            contextMenu.multiSelect = false;
                            contextMenu.close();
                        }
                    }
                }
                
                ImageButton {
                    id: quoteMore
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_quote_more_rounded.png" : "asset:///images/icon_quote_more_rounded_black.png"
                    verticalAlignment: VerticalAlignment.Center
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    visible: false
                    
                    onClicked: {
                        threadWebView.evaluateJavaScript("startMultiselect();");
                        contextMenu.multiSelect = true;
                        visible = false;
                        editButton.visible = false;
                        sendMP.visible = false;
                        addFavorite.visible = false;
                        deleteButton.visible = false;
                        cancelButton.visible = true;
                        quote.visible=true;
                        
                    }
                
                }
                
                ImageButton {
                    id: deleteButton
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_trash_rounded.png" : "asset:///images/icon_trash_rounded_black.png"
                    verticalAlignment: VerticalAlignment.Center
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    visible: false
                    
                    onClicked: {
                        pageContainer.deletePost(listItemSelected, showThreadController.getMessageAuthor(parseInt(listItemSelected)));
                        contextMenu.close();
                    }
                
                }
                
                ImageButton {
                    id: cancelButton
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_dismiss_rounded.png" : "asset:///images/icon_dismiss_rounded_black.png"
                    verticalAlignment: VerticalAlignment.Center
                    preferredHeight: ui.du(8)
                    preferredWidth: ui.du(8)
                    visible: false
                    
                    onClicked: {
                        contextMenu.multiSelect = false;
                        contextMenu.close();
                    }
                }
                
                Container {
                    preferredWidth: ui.du(2)
                    preferredHeight: ui.du(2)
                }
            }        
        }
        
        Container {
            id: actionPicker
            horizontalAlignment: HorizontalAlignment.Fill
            preferredHeight: 0
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            
            Container {
                preferredHeight: 20
                preferredWidth: 1000
            }
            
            attachedObjects: [
                ImplicitAnimationController {
                    id: animList
                    propertyName: "preferredHeight"
                }
            ]
            
            
            ListView {
                id: actionPickerList
                horizontalAlignment: HorizontalAlignment.Fill
                
                layout: GridListLayout {
                    columnCount: 6
                    headerMode: ListHeaderMode.Sticky
                }
                
                dataModel: GroupDataModel {
                    id: theModel
                    grouping: ItemGrouping.ByFullValue
                    sortingKeys: ["category"]
                    sortedAscending: false
                    
                }
                
                
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        
                        Container {
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            layout: StackLayout {
                                orientation: LayoutOrientation.TopToBottom
                            }
                            
                            Container {
                                preferredHeight: ui.du(2)
                            }
                            
                            ImageView {
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                                id: avatarImg
                                scalingMethod: ScalingMethod.AspectFit
                                image: tracker.image
                                minHeight: ui.du(7)
                                maxHeight: ui.du(7)
                                minWidth: ui.du(7)
                                maxWidth: ui.du(7)
                                
                                attachedObjects: [
                                    NetImageTracker {
                                        id: tracker
                                        source: ListItemData.image                                    
                                    } 
                                ]
                            }
                            
                            Label {
                                text: ListItemData.caption
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Center
                                textStyle.fontSize: FontSize.XSmall
                            }
                        
                        }
                    }
                ]
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    
                    // it is easier to call "reply" from qml...
                    if(chosenItem.action == 8) {
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
                    
                    } else if (chosenItem.action == 9) {
                        if(!statPage)
                            statPage = surveyPage.createObject();
                        
                        statPage.survey = showThreadController.survey;
                        
                        nav.push(statPage);
                    } else {
                        showThreadController.doAction(chosenItem.action);
                    }
                    
                    
                    actionPicker.toogleActionPicker();
                }
            }
            
            function toogleActionPicker() {
                if(actionPicker.preferredHeight == 0) {
                    actionPicker.preferredHeight=500;
                } else {
                    actionPicker.preferredHeight=0;
                }
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
                    nextPageNewActionBar.defaultImageSource = Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_refresh_rounded.png" : "asset:///images/icon_refresh_rounded_black.png"
                } else {
                    nextPageAction.title = qsTr("Next page");
                    nextPageAction.imageSource = "asset:///images/icon_next.png"
                    nextPageNewActionBar.defaultImageSource = Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_next_rounded.png" : "asset:///images/icon_next_rounded_black.png"
                }
                
                if(showThreadController.actionSurvey)
                    statsNewActionBar.defaultImageSource = Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_stats_rounded_orange.png" : "asset:///images/icon_stats_rounded_black_orange.png"
                else
                    statsNewActionBar.defaultImageSource = Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_stats_rounded.png" : "asset:///images/icon_stats_rounded_black.png"
                    
                
                statsNewActionBar.visible = !showThreadController.emptySurvey;
                if(statPage)
                    statPage.survey = showThreadController.survey;
                
                scrollView.requestFocus();
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
            
            query.invokeTargetId: "sys.browser";
            query.invokeActionId: "bb.action.OPEN"
            
            
            query {
                onUriChanged: {
                    linkInvocation.query.updateQuery();
                    //linkInvocation.query.invokeTargetId = "sys.browser";
                    //linkInvocation.query.mimeType = "text/html";
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
        ComponentDefinition {
            id: contextPage
            source: "ListContextActions.qml"
        },
        ComponentDefinition {
            id: surveyPage
            source: "Survey.qml"
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
            shortcuts: [
                Shortcut {
                    key: qsTr("r")
                }
            ]
        },
        ActionItem {
            title: qsTr("First page")
            imageSource: "asset:///images/icon_prev_all.png"
            onTriggered: {
                showThreadController.firstPage();
                activityIndicator.start();
                scrollRequested = 0;
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("f")
                }
            ]
        },
        ActionItem {
             title: qsTr("Last page")
             imageSource: "asset:///images/icon_next_all.png"
             onTriggered: {
                 showThreadController.lastPage();
                 activityIndicator.start();
                 scrollRequested = 0;
             }
             shortcuts: [
                 Shortcut {
                     key: qsTr("l")
                 }
             ]
        },
        ActionItem {
            title: qsTr("Prev page")
            imageSource: "asset:///images/icon_prev.png"
            onTriggered: {
                showThreadController.prevPage();
                activityIndicator.start();
                scrollRequested = 0;
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
        		showThreadController.nextPage();
                activityIndicator.start();
                scrollRequested = 0;
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("n")
                }
            ]
        },
        ActionItem {
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
        }
    ]    
    onActionBarVisibilityChanged: {
        focusedItem.requestFocus();
    }
    
    onCreationCompleted: {
        contextMenu.isVisible = false;
        showThreadController.setWebView(threadWebView);
        showThreadController.setActionListView(actionPickerList);
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
        	showThreadController.lastPage(true);
        	activityIndicator.start();
            scrollRequested = 0;
            needUpdate = false;
        } 
    } 
    
}
