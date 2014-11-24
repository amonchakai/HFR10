import bb.cascades 1.2
import Network.SmileyPickerController 1.0
import com.netimage 1.0

Page {
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        verticalAlignment: VerticalAlignment.Fill
        
        ListView {
            id: smileyList
            horizontalAlignment: HorizontalAlignment.Center
            
            layout: GridListLayout {
                columnCount: 5
            }
            
            dataModel: GroupDataModel {
                id: theModel
                grouping: ItemGrouping.None
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        
                        ImageView {
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            id: avatarImg
                            scalingMethod: ScalingMethod.AspectFit
                            image: tracker.image
                            minHeight: tracker.height*3
                            maxHeight: tracker.height*3
                            minWidth: tracker.width*3
                            maxWidth: tracker.width*3
                            
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
                smileyToAdd = chosenItem.tag;
                nav.pop();
            }
        }   
        
        
        TextField {
            id: searchField
            verticalAlignment: VerticalAlignment.Bottom
            hintText:  qsTr("smiley")
            horizontalAlignment: HorizontalAlignment.Left
            inputMode: TextFieldInputMode.Chat
            input {
                submitKey: SubmitKey.Send
                onSubmitted: {
                    smileyPickerController.getSmiley(searchField.text);
                }
            }
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
        smileyPickerController.setListView(smileyList);
        smileyPickerController.loadDefautSmiley();
        
    }
}
