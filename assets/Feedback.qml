import bb.cascades 1.4

Page {
    property string url
    
    titleBar: TitleBar {
        title: qsTr("Feedback")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                nav.pop();
            }
        }
    }
    
    Container {
        layout: DockLayout {
            
        }
        
        Container {  
            id: dataEmptyLabel
            visible: dataModel.empty //model.isEmpty() will not work  
            horizontalAlignment: HorizontalAlignment.Center  
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout {}
            
            Label {
                text: qsTr("No reviews.")
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            ListView {
                id: feedbackListView
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        
                        Container {
                            preferredHeight: ui.du(12)
                            id: listItemContainer
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Center
                            layout: DockLayout {
                            }
                            
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                
                                Label {
                                    text: ListItemData.from
                                    verticalAlignment: VerticalAlignment.Center
                                    minWidth: ui.du(18)
                                    maxWidth: ui.du(18)
                                }
                                
                                Label {
                                    text: ListItemData.role
                                    verticalAlignment: VerticalAlignment.Center
                                    minWidth: ui.du(16)
                                    maxWidth: ui.du(16)
                                }
                                
                                Label {
                                    text: ListItemData.advice
                                    verticalAlignment: VerticalAlignment.Center
                                    minWidth: ui.du(12)
                                    maxWidth: ui.du(12)
                                }
                                
                                Label {
                                    text: ListItemData.review
                                    verticalAlignment: VerticalAlignment.Bottom
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    multiline: true
                                }
                            }
                            
                            
                            
                            Divider {}
                        
                        }
                    }
                ]
                
                
                dataModel: GroupDataModel {
                    id: dataModel    
                    grouping: ItemGrouping.None
                    sortingKeys: ["timestamp"]
                    sortedAscending: true
                    
                    property bool empty: true
                    
                    
                    onItemAdded: {
                        empty = isEmpty();
                    }
                    onItemRemoved: {
                        empty = isEmpty();
                    }  
                    onItemUpdated: empty = isEmpty()  
                    
                    // You might see an 'unknown signal' error  
                    // in the QML-editor, guess it's a SDK bug.  
                    onItemsChanged: empty = isEmpty()      
                }
            }
        }
    }
    
    
    onCreationCompleted: {
        profileController.setListView(feedbackListView);
    }
    
    onUrlChanged: {
        profileController.loadFeedback(url);
    }
}
