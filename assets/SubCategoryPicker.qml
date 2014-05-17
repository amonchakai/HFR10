import bb.cascades 1.2

Page {
    property string subcatXml
    id: subCatSelectPage
    signal slectionCompleted(int index);
    
    ListView {
        id: subcatpicker
        accessibility.name: "SubcatPickerListView"
        
        dataModel: XmlDataModel {
            source: "model/" + subcatXml
        }
        onTriggered: {
            pageCat.subCatIndex = indexPath[0];
            nav.pop();
        }
    }
}
