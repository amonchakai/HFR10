import bb.cascades 1.2
import bb.system 1.2

Page {
    Container {
        id: displayCategoryListContainer
    	//Title container
    	Container {
    	    horizontalAlignment: HorizontalAlignment.Center
    	    layout: DockLayout {}
        	Label {
        	    //TODO Load title from model or some argument
                text: qsTr("HFR 4 BB") + Retranslate.onLocaleOrLanguageChanged
                textStyle.fontSize: FontSize.Default
                textStyle.fontWeight: FontWeight.Bold
                textStyle.textAlign: TextAlign.Center
            }
        }

        //List container
        Container {
            ListView {
                dataModel: XmlDataModel {
                    source: "model/sample-category-list.xml"
                }
                attachedObjects: [
                    SystemToast {
                        id: tempCatLauncherToast
                        body: "Ouvrir categorie"
                    }
                ]
                onSelectionChanged: {
                    if(selected) {
                        //TODO Display page of the corresponding category
                        tempCatLauncherToast.show();
                    }
                }
            }
     	}    
    }
}
