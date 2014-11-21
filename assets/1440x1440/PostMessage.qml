import bb.cascades 1.2
import Network.PostMessageController 1.0
import com.netimage 1.0
import Network.SmileyPickerController 1.0

Page {
    id: postMesssage
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
    property string  recipient
    property string  quoteURL
    
    property string smileyToAdd
    property string subcatxml
    
    property bool   update
    
    actionBarVisibility: ChromeVisibility.Visible
    
    titleBar: TitleBar {
        id: pageTitleBar
        title: qsTr("Reply")
        
        acceptAction: ActionItem {
            title: qsTr ("Send")
            
            onTriggered: {
                switch (mode) {
                	case 1: 
                		postMessageController.postMessage(	 hashCheck 
                    							   , postID
                    							   , catID
                    							   , page
                    							   , pseudo
                    							   , message.text
                    							   , threadTitle
                                                   , addSignature
                        );
                        break;
                    
                    case 2:
                        postMessageController.postEdit(message.text);
                        mode = 1;
                        break;
                    
                    case 3:
                        postMessageController.postMessage(message.text);
                        mode = 1;
                        break;
                    
                    case 4:
                        postMessageController.postNewPrivateMessage(hashCheck
                            									  , pseudo
                            									  , addSignature
                            									  , newMessageCaption.text
                            									  , recipient
                            									  , message.text
                        );
                        mode = 1;
                        break;
                    
                    case 5:
                        postMessageController.postNewTopic(
                                 newTopic.text
                               , sousCat.selectedValue
                               , message.text
                        );
                        mode = 1;
                        break;
                }
            }
        }
        
        dismissAction: ActionItem {
            title: qsTr ("Cancel")
            
            onTriggered: {
                mode = 1;
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
            message.editor.insertPlainText("[" + tag + "]" + message.editor.selectedText + "[/" + tag + "]" );
        }
        
        TextField {
            id: newMessageCaption
            visible: false
            hintText: qsTr("New MP to: ") + recipient
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
            
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
                
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            Container {
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
            ImageButton {
                defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/blackFace.png" : "asset:///images/whiteFace.png";
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    toogleEmoji();
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
                    columnCount: 9
                }
                dataModel: GroupDataModel {
                    id: theModel
                    grouping: ItemGrouping.None
                }
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        
                        Container {
                            
                            ImageView {
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                                id: avatarImg
                                scalingMethod: ScalingMethod.AspectFit
                                minHeight: tracker.height*3
                                maxHeight: tracker.height*3
                                minWidth: tracker.width*3
                                maxWidth: tracker.width*3
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
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    message.text = message.text.substring(0, message.editor.cursorPosition)  + " " + chosenItem.tag + " " + message.text.substring(message.editor.cursorPosition);
                    toogleEmoji();
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
                        smileyPickerList.requestFocus();
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
                            smileyPickerList.requestFocus();
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
                        smileyPickerList.requestFocus();
                    }
                }
                ImageButton {
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_home_black.png" : "asset:///images/icon_home.png"
                    preferredWidth: 80
                    onClicked: {
                        smileyPickerController.loadDefautSmiley();
                        smileyPickerList.layout.columnCount = 9;
                        smileyPickerList.requestFocus();
                    }
                }
                ImageButton {
                    id: next
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? "asset:///images/icon_next_black.png" : "asset:///images/icon_next.png"
                    preferredWidth: 80
                    onClicked: {
                        smileyPickerController.getNextPage();
                        smileyPickerList.requestFocus();
                    }
                }
            
            }    
        }
        
        
    }
    
    function toogleEmoji() {
        if(emoticonsPicker.preferredHeight == 0) {
            emoticonsPicker.preferredHeight=500;
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
                nav.pop()
                message.text = "";
                mode = 1;
                newMessageCaption.text = "";
                pageThread.needUpdate = true;
            }
            
            onMessageLoaded: {
                message.text = message_loaded;
                activityIndicator.stop();
            }
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
        mode = 1;
        smileyPickerController.setListView(smileyPickerList);
        smileyPickerController.loadDefautSmiley();
        postMessageController.setDropdown(sousCat);
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
