import bb.cascades 1.2
import Network.PostMessageController 1.0
import Network.SmileyPickerController 1.0
import Network.SearchKeyRetriever 1.0
import com.netimage 1.0
import bb.system 1.2

NavigationPane {
    id: nav
    property int     navDepth
    property variant tpage
    property variant uploadPage
    
    Page {
        id: postMesssage
        objectName: "postMesssage"
        
        property variant tpage
        property variant uploadPage
        
        
        property string  hashCheck
        property string  postID
        property string  catID
        property string  page
        property string  pseudo
        property string  threadTitle
        property bool  	 addSignature
        
        property int 	 mode
        property string  editURL
        property string  quoteURL
        
        property string smileyToAdd
        property string subcatxml
        
        property bool   update
        
        property int selectionBegin
        property int selectionStop
        property int startDecreasing
        property bool anchored
        property int lastPosC
        
        actionBarVisibility: ChromeVisibility.Visible
        
        function isBBPassport() {
            return DisplayInfo.width == 1440 && DisplayInfo.height == 1440;
        }
        
        titleBar: TitleBar {
            id: pageTitleBar
            title: qsTr("Reply")
            
            acceptAction: ActionItem {
                id: sendAction
                title: qsTr ("Send")
                enabled: false
                
                onTriggered: {
                    switch (postMesssage.mode) {
                        case 1: 
                            postMessageController.postMessage(	 searchKey.haskKey 
                            , postMesssage.postID
                            , postMesssage.catID
                            , postMesssage.page
                            , postMesssage.pseudo
                            , message.text
                            , postMesssage.threadTitle
                            , postMesssage.addSignature
                            );
                            break;
                        
                        case 2:
                            postMessageController.postEdit(message.text);
                            postMesssage.mode = 1;
                            break;
                        
                        case 3:
                            postMessageController.postMessage(message.text);
                            postMesssage.mode = 1;
                            break;
                        
                        case 4:
                            postMessageController.postNewPrivateMessage(searchKey.haskKey
                            , postMesssage.pseudo
                            , postMesssage.addSignature
                            , newMessageCaption.text
                            , chooseDst.text
                            , message.text
                            );
                            postMesssage.mode = 1;
                            break;
                        
                        case 5:
                            postMessageController.postNewTopic(
                            newTopic.text
                            , sousCat.selectedValue
                            , message.text
                            );
                            postMesssage.mode = 1;
                            break;
                    }
                }
            }
            
            dismissAction: ActionItem {
                title: qsTr ("Cancel")
                
                onTriggered: {
                    postMesssage.mode = 1;
                    nav.pop()
                }
            }
        }
        
        
        Container {
            id: formContainer
            ActivityIndicator {
                id: activityIndicator
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                preferredHeight: orientationHandler.orientation == OrientationSupport.orientation.Portrait ? 60 : 80;
            }
            
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            
            function insertTag(tag) {
                if(postMesssage.selectionStop == 0) postMesssage.selectionStop = postMesssage.startDecreasing;
                
                if(postMesssage.selectionBegin>postMesssage.selectionStop) {
                    var tmp = postMesssage.selectionStop;
                    postMesssage.selectionStop = postMesssage.selectionBegin;
                    postMesssage.selectionBegin = tmp;
                }
                
                message.text = message.text.substring(0, postMesssage.selectionBegin) + "[" + tag + "]" + message.text.substring(postMesssage.selectionBegin, postMesssage.selectionStop) + "[/" + tag + "]" + message.text.substring(postMesssage.selectionStop);
            }
            
            TextField {
                id: chooseDst
                hintText: qsTr("Send MP to: ")   
                onTextChanged: {
                    if(newMessageCaption.text.length > 0 && chooseDst.text.length > 0)
                        sendAction.enabled = true;
                    else 
                        sendAction.enabled = false;
                }             
            }
            
            TextField {
                id: newMessageCaption
                visible: false
                hintText: qsTr("Title: ")
                onTextChanged: {
                    if(newMessageCaption.text.length > 0 && chooseDst.text.length > 0)
                        sendAction.enabled = true;
                    else 
                        sendAction.enabled = false;
                } 
            }
            TextField {
                id: newTopic
                visible: false
                hintText: qsTr("New topic")
            }
            DropDown {
                id: sousCat
                visible: false
            }
            TextArea {
                id: message
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                
                
                editor {
                    onSelectionEndChanged: {
                        if(selectionEnd != 0 && selectionEnd != postMesssage.selectionBegin)
                            postMesssage.selectionStop = selectionEnd;
                        postMesssage.anchored = false;
                    }
                    
                    onCursorPositionChanged: {
                        if(Math.abs(postMesssage.lastPosC-cursorPosition) > 1)
                            postMesssage.selectionStop = cursorPosition;
                        postMesssage.lastPosC = cursorPosition;
                    }
                    
                    onSelectionStartChanged: {
                        if(selectionStart < postMesssage.selectionBegin && !postMesssage.anchored) {
                            postMesssage.startDecreasing = postMesssage.selectionBegin;
                            postMesssage.anchored = true;
                        }
                        
                        if(selectionStart != 0 && selectionStart != postMesssage.selectionStop)
                            postMesssage.selectionBegin = selectionStart;
                    }
                
                }
            
            }
            
            background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#202020") : Color.create("#f5f5f5")
            
            
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                
                }
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#202020") : Color.create("#f5f5f5")
                Container {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ImageButton {
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/blackFace.png" : "asset:///images/whiteFace.png";
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        postMesssage.toogleEmoji();
                    }
                }
                Container {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ImageButton {
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_bold_black.png" : "asset:///images/icon_bold.png"
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        formContainer.insertTag("b");
                    }
                }
                Container {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ImageButton {
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_italic_black.png" : "asset:///images/icon_italic.png"
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        formContainer.insertTag("i");
                    }
                }
                Container {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ImageButton {
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ?  "asset:///images/icon_underline_black.png" : "asset:///images/icon_underline.png"
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        formContainer.insertTag("u");
                    }
                }
                Container {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ImageButton {
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ?  "asset:///images/icon_code_black.png" : "asset:///images/icon_code.png"
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        formContainer.insertTag("cpp");
                    }
                }
                Container {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ImageButton {
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_quote_black.png" : "asset:///images/icon_quote.png"
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        formContainer.insertTag("quote");
                    }
                }
                Container {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ImageButton {
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_image.png" : "asset:///images/icon_image_white.png"
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        if(!uploadPage)
                            uploadPage = imageUploader.createObject();
                        nav.push(uploadPage);
                    }
                }
                Container {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
            }
            
            
            Container {
                preferredHeight: 20
            }
            
            Container {
                id: emoticonsPicker
                preferredHeight: 0
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                
                
                Container {
                    preferredHeight: 20
                }
                
                attachedObjects: [
                    ImplicitAnimationController {
                        id: animList
                        propertyName: "preferredHeight"
                    }
                ]
                
                ListView {
                    id: smileyPickerList
                    layout: GridListLayout {
                        columnCount: postMesssage.isBBPassport() ? 9 : 7
                    }
                    dataModel: GroupDataModel {
                        id: theModel
                        grouping: ItemGrouping.None
                    }
                    
                    listItemComponents: [
                        ListItemComponent {
                            type: "item"
                            
                            Container {
                                id: listItemContainer
                                
                                ImageView {
                                    verticalAlignment: VerticalAlignment.Center
                                    horizontalAlignment: HorizontalAlignment.Center
                                    id: avatarImg
                                    scalingMethod: ScalingMethod.AspectFit
                                    minHeight: tracker.height* listItemContainer.ListItem.view.scalingFactor();
                                    maxHeight: tracker.height* listItemContainer.ListItem.view.scalingFactor();
                                    minWidth: tracker.width*  listItemContainer.ListItem.view.scalingFactor();
                                    maxWidth: tracker.width*  listItemContainer.ListItem.view.scalingFactor();
                                    image: tracker.image
                                    
                                    attachedObjects: [
                                        NetImageTracker {
                                            id: tracker
                                            
                                            source: ListItemData.localUrl                                    
                                        } 
                                    ]
                                }
                            }
                        }
                    ]
                    
                    function scalingFactor() {
                        return (postMesssage.isBBPassport() ? 3 : 2)
                    }
                    
                    onTriggered: {
                        var chosenItem = dataModel.data(indexPath);
                        message.text = message.text.substring(0, message.editor.cursorPosition)  + " " + chosenItem.tag + " " + message.text.substring(message.editor.cursorPosition);
                        postMesssage.toogleEmoji();
                    }
                }
                
                
                
                Container {
                    id: controlContainer
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    preferredHeight: 30
                    visible: false
                    
                    ImageButton {
                        id: prev
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_prev_black.png" : "asset:///images/icon_prev.png" 
                        preferredWidth: 80
                        onClicked: {
                            smileyPickerController.getPrevPage();
                        }
                    }
                    TextField {
                        id: searchField
                        inputMode: TextFieldInputMode.Chat
                        input {
                            submitKey: SubmitKey.Send
                            onSubmitted: {
                                smileyPickerController.getSmiley(searchField.text);
                                smileyPickerList.layout.columnCount = 5;
                            }
                        }
                    }
                    ImageButton {
                        id: search
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_refresh_black.png" :  "asset:///images/icon_refresh2.png"
                        preferredWidth: 80
                        onClicked: {
                            smileyPickerController.getSmiley(searchField.text);
                            smileyPickerList.layout.columnCount = 5;
                        }
                    }
                    ImageButton {
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_home_black.png" : "asset:///images/icon_home.png"
                        preferredWidth: 80
                        onClicked: {
                            smileyPickerController.loadDefautSmiley();
                            smileyPickerList.layout.columnCount =  (postMesssage.isBBPassport() ? 9 : 7);
                        }
                    }
                    ImageButton {
                        id: next
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_next_black.png" : "asset:///images/icon_next.png"
                        preferredWidth: 80
                        onClicked: {
                            smileyPickerController.getNextPage();
                        }
                    }
                
                }    
            }
        
        
        }
        
        function toogleEmoji() {
            if(emoticonsPicker.preferredHeight == 0) {
                emoticonsPicker.preferredHeight= (postMesssage.isBBPassport() ? 500 : 350);;
                controlContainer.visible = true;
            } else {
                emoticonsPicker.preferredHeight=0;
                controlContainer.visible=false;
            }
        }
        
        attachedObjects: [
            PostMessageController {
                id: postMessageController
                
                onComplete: {
                    _app.closeCard();
                }
                
                onMessageLoaded: {
                    message.text = message_loaded;
                    activityIndicator.stop();
                }
            },
            SearchKeyRetriever {
                id: searchKey
            }, 
            SmileyPickerController {
                id: smileyPickerController
            
            },
            ComponentDefinition {
                id: smileyPicker
                source: "SmileyPicker.qml"
            },

            ComponentDefinition {
                id: imageUploader
                source: "ImageUploader.qml"
            },
            OrientationHandler {
                id: orientationHandler
            }
        ]
        
        actions: [
            ActionItem {
                title: "Smiley"
                imageSource: "asset:///images/whiteFace.png"
                onTriggered: {
                    if(!tpage)
                        tpage = smileyPicker.createObject(0);
                    nav.push(tpage);
                }
            },
            ActionItem {
                title: qsTr("Quote")
                imageSource: "asset:///images/icon_quote.png"
                onTriggered: {
                    message.editor.insertPlainText("[quote]" + message.editor.selectedText + "[/quote]");
                }
            },
            ActionItem {
                title: qsTr("Code")
                imageSource: "asset:///images/icon_code.png"
                onTriggered: {
                    message.editor.insertPlainText("[cpp]" + message.editor.selectedText + "[/cpp]");
                }
            },
            ActionItem {
                title: qsTr("Bold")
                imageSource: "asset:///images/icon_bold.png"
                onTriggered: {
                    message.editor.insertPlainText("[b]" + message.editor.selectedText + "[/b]");
                }
            },
            ActionItem {
                title: qsTr("Italic")
                imageSource: "asset:///images/icon_italic.png"
                onTriggered: {
                    message.editor.insertPlainText("[i]" + message.editor.selectedText + "[/i]");
                }
            },
            ActionItem {
                title: qsTr("Underline")
                imageSource: "asset:///images/icon_underline.png"
                onTriggered: {
                    message.editor.insertPlainText("[u]" + message.editor.selectedText + "[/u]");
                }
            }
        ]
        
        onEditURLChanged: {
            if(editURL == "") return;
            
            postMessageController.getEditMessage(editURL);
            activityIndicator.start();
            mode = 2;
            editURL = "";
        }
        
        onQuoteURLChanged: {
            if(quoteURL == "") return;
            
            postMessageController.getQuotedMessages(quoteURL);
            activityIndicator.start();
            mode = 3;
            
            quoteURL = "";
        }
        
        onCreationCompleted: {
            postMesssage.mode = 4;
            smileyPickerController.setListView(smileyPickerList);
            smileyPickerController.loadDefautSmiley();
            postMessageController.setDropdown(sousCat);
            searchKey.getKey();
        }
        
        onSmileyToAddChanged: {
            if(smileyToAdd == "")
                return;
            
            message.text = message.text.substring(0, message.editor.cursorPosition)  + " " + smileyToAdd + " " + message.text.substring(message.editor.cursorPosition);
        
        }
        
        onModeChanged: {
            if(mode == 4)
                newMessageCaption.visible = true;
            else 
                newMessageCaption.visible = false;
        
        }
        
        onUpdateChanged: {
            if(!update)
                return;
            
            if(mode == 4)
                newMessageCaption.visible = true;
            else 
                newMessageCaption.visible = false;
            
            if(mode == 5) {
                newTopic.visible = true;
                sousCat.visible = true;
                postMessageController.getSubCatsInfo(subcatxml);
                activityIndicator.start();
            } else {
                newTopic.visible = false;
                sousCat.visible = false;
            }
            
            update = false;
        }


    
    }
        
    onPopTransitionEnded: {
        --navDepth;
        if(navDepth == 0) {
            _app.closeCard();
        }
        
    }
    
    onPushTransitionEnded: {
        ++navDepth;
    }
    
    
} 
