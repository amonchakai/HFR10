import bb.cascades 1.2

Page {
    Container {
        id: displayTopicListContainer
    	//Title container with category name
    	Container {
            horizontalAlignment: HorizontalAlignment.Center
            layout: DockLayout {}
        	Label {
                //TODO Load title from model or some argument
                text: "Hardware"
                textStyle.fontSize: FontSize.Default
                textStyle.fontWeight: FontWeight.Bold
                textStyle.textAlign: TextAlign.Center
            }
        }

        //List container for all topic in the current category
        Container {
            ListView {
                dataModel: XmlDataModel {
                    source: "model/sample-topic-list.xml"
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "listItem"
                        StandardListItem {
                            title: ListItemData.title
                            status: ListItemData.nbPages
                            description: ""
                        }
                    }
                ]
            }
     	}
        
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            layout: DockLayout {}
            Label {
                //TODO Retreive data from model
                text: "Page 1/500"
                textStyle.fontSize: FontSize.XSmall
                textStyle.fontWeight: FontWeight.Bold
                textStyle.textAlign: TextAlign.Center
            }
        }    
    }
}
