import bb.cascades 1.2
import Network.PostMessageController 1.0

Page {
    id: postMesssage
    property string  hashCheck
    property string  postID
    property string  catID
    property string  page
    property string  pseudo
    property string  threadTitle
    property bool  	 addSignature
    
    property int 	 mode
    property string  editURL
    
    property string quoteURL
    
    property string smileyToAdd
    
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
                        break;
                    
                    case 3:
                        postMessageController.postMessage(message.text);
                        break;
                }
            }
        }
        
        dismissAction: ActionItem {
            title: qsTr ("Cancel")
            
            onTriggered: {
                nav.pop()
            }
        }
    }

            
    Container {
        ActivityIndicator {
            id: activityIndicator
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 60
        }
        
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        TextArea {
            id: message
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }
    }
    
    attachedObjects: [
        PostMessageController {
            id: postMessageController
            
            onComplete: {
                nav.pop()
                pageThread.needUpdate = true;
            }
            
            onMessageLoaded: {
                message.text = message_loaded;
                activityIndicator.stop();
            }
        },
        ComponentDefinition {
            id: smileyPicker
            source: "SmileyPicker.qml"
        }
    ]
    
    actions: [
        ActionItem {
            title: "Smiley"
            imageSource: "asset:///images/whiteFace.png"
            onTriggered: {
                var page = smileyPicker.createObject(0);
                nav.push(page);
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
            onTriggered: {
                message.editor.insertPlainText("[b]" + message.editor.selectedText + "[/b]");
            }
        },
        ActionItem {
            title: qsTr("Italic")
            onTriggered: {
                message.editor.insertPlainText("[i]" + message.editor.selectedText + "[/i]");
            }
        },
        ActionItem {
            title: qsTr("Underline")
            onTriggered: {
                message.editor.insertPlainText("[u]" + message.editor.selectedText + "[/u]");
            }
        }
    ]
    
    onEditURLChanged: {
        postMessageController.getEditMessage(editURL);
        activityIndicator.start();
        mode = 2;
    }
    
    onQuoteURLChanged: {
        postMessageController.getQuotedMessages(quoteURL);
        activityIndicator.start();
        mode = 3;
    }
    
    onCreationCompleted: {
        mode = 1;
    }
    
    onSmileyToAddChanged: {
        if(smileyToAdd == "")
        	return;
        	
        message.editor.insertPlainText(smileyToAdd);
        
    }
}
