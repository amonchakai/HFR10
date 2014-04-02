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
    
    property bool 	 editMode
    property string  editURL
    
    
    titleBar: TitleBar {
        id: pageTitleBar
        title: qsTr("Reply")
        
        acceptAction: ActionItem {
            title: qsTr ("Send")
            
            onTriggered: {
                if(!editMode) {
                	postMessageController.postMessage(	 hashCheck 
                    							   , postID
                    							   , catID
                    							   , page
                    							   , pseudo
                    							   , message.text
                    							   , threadTitle
                                                   , addSignature
                    )
                } else {
                    postMessageController.postEdit(message.text);
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
        }
    ]
    
    onEditURLChanged: {
        postMessageController.getEditMessage(editURL);
        activityIndicator.start();
        editMode = true;
    }
    
    onCreationCompleted: {
        editMode = false;
    }
}
