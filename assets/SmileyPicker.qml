import bb.cascades 1.2
import Network.SmileyPickerController 1.0

Page {
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        verticalAlignment: VerticalAlignment.Fill
        
        WebView {
            id: smileyList
            settings.textAutosizingEnabled: false
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            
            onMessageReceived: {
                postMesssage.smileyToAdd = message.data;
                console.log("smileyPicker")
                nav.pop();
            }
        }
        
        TextField {
            id: searchField
            verticalAlignment: VerticalAlignment.Bottom
            hintText:  qsTr("smiley")
            horizontalAlignment: HorizontalAlignment.Left
        }
    }
    
    actions: [
        ActionItem {
            title: qsTr("Default")
            imageSource: "asset:///images/icon_home.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                smileyPickerController.loadDefautSmiley();
            }
        },
        ActionItem {
            title: qsTr("Search")
            imageSource: "asset:///images/icon_browse.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                smileyPickerController.getSmiley(searchField.text);
            }
        },
        ActionItem {
            title: qsTr("Next page")
            imageSource: "asset:///images/icon_next.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                smileyPickerController.getNextPage();
            }
        },
        ActionItem {
            title: qsTr("Previous page")
            imageSource: "asset:///images/icon_prev.png"
            onTriggered: {
                smileyPickerController.getPrevPage();
            }
        }
    ]
    
    attachedObjects: [
    	SmileyPickerController {
            id: smileyPickerController
        }    
    ]
    
    onCreationCompleted: {
        smileyPickerController.setWebView(smileyList);
        smileyPickerController.loadDefautSmiley();
        
    }
}
