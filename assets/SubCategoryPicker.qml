import bb.cascades 1.2

Page {
    property string messageID
    property string  subcatXml
    
    id: subCatSelectPage
    signal slectionCompleted(int index);
    
    ListView {
        id: subcatpicker
        accessibility.name: "SubcatPickerListView"
        
        dataModel: XmlDataModel {
            source: "file:" + subcatXml
        }
        onTriggered: {
            pageCat.subCatIndex = indexPath[0];
            console.log('Selected sub cat: ' + pageCat.subCatIndex)
            nav.pop();
        }
    }
}
