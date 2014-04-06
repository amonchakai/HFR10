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
            html: "<!DOCTYPE html><html><head><style type=\"text/css\">"
            + "table {border-left:5px solid lightgray; } th {text-align:left; text-decoration:underline;} "	// render quotation table
            + "body {font-size:25px; } "  // switch webview color based on theme
            + "p {font-size:25px;} "
            + "</style>" 
            + "</head><body>" + "totooooooo" + "</body></html>"
            settings.textAutosizingEnabled: false
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            
            onMessageReceived: {
                postMesssage.smileyToAdd = message.data;
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
