import bb.cascades 1.2
import conf.settings 1.0

Page {
    property string urlPage
    
    Container {
        id: headerContainer
        layout: StackLayout {    
            orientation: LayoutOrientation.TopToBottom
        }
        
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Top
        
        function themeStyleToHeaderColor(style){
            switch (style) {
                case VisualStyle.Bright:
                    return Color.create(0.96,0.96,0.96);
                case VisualStyle.Dark: 
                    return Color.create(0.15,0.15,0.15);
                default :
                    return Color.create(0.96,0.96,0.96);    
            }
            return Color.create(0.96,0.96,0.96); 
        }
        
        Container {
            background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            preferredHeight: 40
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            Label {
                text: qsTr("Color tag")
                textStyle.fontSize: FontSize.XSmall
            }
        } 
        Container {
            background: Color.create("#00A7DE") 
            minHeight: 4
            maxHeight: 4
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
        }
        Container {
            minHeight: 10
            maxHeight: 10
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
        }
        
        ListView {
            id: colorPickerView
            accessibility.name: "colorPickerView"
            
            dataModel: XmlDataModel {
                source: "asset:///model/ColorList_" + qsTr("en") + ".xml"
            }
            
            listItemComponents: [
                 ListItemComponent {
                    type: "item"
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Center
                        preferredHeight: 130
                        
                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            horizontalAlignment: HorizontalAlignment.Left
                            verticalAlignment: VerticalAlignment.Center
                            
                            Container {
                                preferredHeight: 100
                                preferredWidth: 100
                                background: Color.create(ListItemData.color)
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Left
                            }
                            
                            Label {
                                text: ListItemData.name
                                verticalAlignment: VerticalAlignment.Center
                            }
                        }
                        Divider {}
                    }
                    
                    
                    
                }
            ]
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                chosenTag = chosenItem.value;
                
                var postId = urlPage.match("post=[0-9]+"); postId = postId.toString().substring(5);
                var catId = urlPage.match("cat=[0-9]+"); catId = catId.toString().substring(4);
                appSettings.setTag(postId + "@" + catId, chosenItem.value);
                appSettings.saveColors();
                nav.pop();
            }
        }
        
    }
    
    attachedObjects: [
        AppSettings {
            id: appSettings
        }
    ]
}
