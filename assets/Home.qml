import bb.cascades 1.2

Page {
    Container {
        
        ListView {
            
            dataModel: XmlDataModel{
                source: "model/listCategories.xml"
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    
                    CustomListItem {
                        dividerVisible: true
                        highlightAppearance: HighlightAppearance.Default
                        
                        Container {
                            id: contentContainer
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            verticalAlignment: VerticalAlignment.Fill
                            horizontalAlignment: HorizontalAlignment.Fill
                            
                            
                            Label {
                                id: itemText
                                text: ListItemData.title
                                verticalAlignment: VerticalAlignment.Center
                                textStyle.base: SystemDefaults.TextStyles.TitleText
                                accessibilityMode: A11yMode.Collapsed 
                            }    
                            
                        } // container in one ITEM
                    } // custom item
                } //end list ITEM
            ] //end list
        } // end view
    } //end container
} // end page


