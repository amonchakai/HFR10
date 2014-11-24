import bb.cascades 1.2
import bb.cascades.pickers 1.0
import Network.ImageUploaderController 1.0

Page {
    property string imageToUpload
    
    ScrollView {
        id: scrollView
        accessibility.name: "scrollView"
        
        Container {
            id: headerContainer
            
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
            
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top
            
            ProgressIndicator {
                id: uploadingProgress
                accessibility.name: "uploadingProgress"
                horizontalAlignment: HorizontalAlignment.Fill
                fromValue: 0
                toValue: 100
                visible: false
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
                    text: qsTr("Upload an image to Rehost")
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
            
            Button {
                id: pickImageButton
                text: qsTr("Pick an image")
                horizontalAlignment: HorizontalAlignment.Fill
                
                onClicked: {
                    filePicker.open();
                }
            }
            
                       
            Container {
                id: actionGroup
                visible: false
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom 
                }
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top
                
                Container {
                    background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    preferredHeight: 40
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Label {
                        text: qsTr("Size")
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
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Button {
                        id: miniButton
                        text: qsTr("Mini")         
                        onClicked: {
                            postMesssage.smileyToAdd = uploader.mini;
                            nav.pop();
                        }           
                    }
                    
                    Button {
                        id: previewButton
                        text: qsTr("Preview")
                        onClicked: {
                            postMesssage.smileyToAdd = uploader.preview;
                            nav.pop();
                        } 
                    }
                    
                    Button {
                        id: originalButton
                        text: qsTr("Original")
                        onClicked: {
                            postMesssage.smileyToAdd = uploader.original;
                            nav.pop();
                        } 
                    }
                }
            
            }
            
            Button {
                id: uploadButton
                text: qsTr("Upload")
                horizontalAlignment: HorizontalAlignment.Fill
                enabled: false
                
                onClicked: {
                    uploadingProgress.visible = true;
                    uploader.upload(imageToUpload);
                }
            }
            
            ImageView {
                id: preview
                accessibility.name: "preview"
                horizontalAlignment: HorizontalAlignment.Center
                scalingMethod: ScalingMethod.AspectFill
            }
            
            onCreationCompleted: {
                uploadButton.enabled = false;
                actionGroup.visible = false;
                uploader.setProgressIndicator(uploadingProgress);
            }
            
            
            
            attachedObjects: [
                FilePicker {
                    id:filePicker
                    type : FileType.Picture
                    title : "Select Picture"
                    directories : ["/accounts/1000/shared/misc"]
                    onFileSelected : {
                        console.log("FileSelected signal received : " + selectedFiles);
                        if(selectedFiles.length > 0) {
                            imageToUpload = selectedFiles[0];
                            preview.setImageSource(selectedFiles[0]);
                            uploadButton.enabled = true;
                        }
                    }
                },
                ImageUploaderController {
                    id: uploader 
                    
                    onComplete: {
                        preview.preferredHeight = 500;
                        preview.scalingMethod = ScalingMethod.AspectFit
                        uploadButton.enabled = false;
                        actionGroup.visible = true;
                        uploadingProgress.visible = false;
                    }
                }
            ]
        }
    }
    
    onImageToUploadChanged: {
        actionGroup.visible = false;
    }
}
