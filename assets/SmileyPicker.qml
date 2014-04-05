import bb.cascades 1.2
import Network.SmileyPickerController 1.0

Page {
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        WebView {
            id: smileyList
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }
        
        TextField {
            id: searchField
            hintText:  qsTr("smiley")
            horizontalAlignment: HorizontalAlignment.Left
        }
    }
    
    actions: [
        ActionItem {
            title: qsTr("Default")
            imageSource: "asset:///images/icon_home.png"
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        ActionItem {
            title: qsTr("Search")
            imageSource: "asset:///images/icon_browse.png"
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        ActionItem {
            title: qsTr("Next page")
            imageSource: "asset:///images/icon_next.png"
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        ActionItem {
            title: qsTr("Previous page")
            imageSource: "asset:///images/icon_prev.png"
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
