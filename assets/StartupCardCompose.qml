import bb.cascades 1.2
import Network.PostMessageController 1.0
import Network.SearchKeyRetriever 1.0
import com.netimage 1.0
import bb.system 1.2

NavigationPane {
    id: nav
    property int     navDepth
    property variant tpage
    property variant uploadPage
    
    Page {
        id: postMesssage        
        objectName: "postMesssage"
        
        property string  hashCheck
        property string  postID
        property string  catID
        property string  page
        property string  pseudo
        property string  threadTitle
        property bool  	 addSignature
        
        property int 	 mode
        property string  editURL
        property string  quoteURL
        
        property string smileyToAdd
        
        actionBarVisibility: ChromeVisibility.Visible
        
        titleBar: TitleBar {
            id: pageTitleBar
            title: qsTr("New message")
            
            acceptAction: ActionItem {
                title: qsTr ("Send")
                
                onTriggered: {
                    postMessageController.postNewPrivateMessage(searchKeyRetriever.haskKey
                                , postMesssage.pseudo
                            , true
                            , newMessageCaption.text
                            , recipient.text
                            , message.text
                            );
                }
            }
            
            dismissAction: ActionItem {
                title: qsTr ("Cancel")
                
                onTriggered: {
                    nav.pop();
                }
            }
        }
        
        
        Container {
            id: formContainer
            ActivityIndicator {
                id: activityIndicator
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                preferredHeight: orientationHandler.orientation == OrientationSupport.orientation.Portrait ? 60 : 80;
            }
            
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            function adjustBackGroundAndcolor(style) {
                if(style == VisualStyle.Bright) {
                    return "} ";
                } else {
                    return "background-color:#000000; color:#DDDDDD; } ";
                }   
            }
            
            function chooseIconSet(style) {
                if(style == VisualStyle.Bright) {
                    return    "<td><img src=\"local:///assets/images/blackFace.png\" alt=\"smiley\" title=\"smiley\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_bold_black.png\" alt=\"bold\" title=\"bold\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_italic_black.png\" alt=\"italic\" title=\"italic\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_underline_black.png\" alt=\"underline\" title=\"underline\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_code_black.png\" alt=\"code\" title=\"code\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_quote_black.png\" alt=\"quote\" title=\"quote\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_image.png\" alt=\"image\" title=\"image\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" ;
                } else {
                    return    "<td><img src=\"local:///assets/images/whiteFace.png\" alt=\"smiley\" title=\"smiley\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_bold.png\" alt=\"bold\" title=\"bold\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_italic.png\" alt=\"italic\" title=\"italic\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_underline.png\" alt=\"underline\" title=\"underline\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_code.png\" alt=\"code\" title=\"code\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_quote.png\" alt=\"quote\" title=\"quote\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" 
                    + "<td><img src=\"local:///assets/images/icon_image_white.png\" alt=\"image\" title=\"image\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>" ; 
                }
            }
            
            WebView {
                id: webviewActionBar
                html: "<!DOCTYPE html><html><head><style type=\"text/css\">"
                + "table { table-layout:fixed; width: 800px; border-spacing: 30px 0px; } th {text-align:left; text-decoration:underline;} "	// render quotation table
                + "body {font-size:25px; " + formContainer.adjustBackGroundAndcolor(Application.themeSupport.theme.colorTheme.style)  // switch webview color based on theme
                + "p {font-size:25px;} "
                + "</style>" 
                + "</head><body>" + "<table style=\"margin-left:auto; margin-right:auto; \" ><tr>" 
                + formContainer.chooseIconSet(Application.themeSupport.theme.colorTheme.style)
                + "</tr></table>" + "</body></html>"
                preferredHeight: 60
                horizontalAlignment: HorizontalAlignment.Fill
                
                onMessageReceived: {
                    if(message.data == "smiley") {
                        if(!tpage)
                            tpage = smileyPicker.createObject(0);
                        nav.push(tpage);
                    } else {
                        if(message.data == "bold") {
                            formContainer.insertTag("b");
                        } else { 
                            if (message.data == "italic") {
                                formContainer.insertTag("i");
                            } else { 
                                if(message.data == "underline") {
                                    formContainer.insertTag("u");
                                } else { 
                                    if(message.data == "code") {
                                        formContainer.insertTag("cpp");
                                    } else {
                                        if(message.data == "quote") {
                                            formContainer.insertTag("quote");
                                        } else { 
                                            if(message.data == "image") {
                                                if(!uploadPage)
                                                    uploadPage = imageUploader.createObject(this);
                                                nav.push(uploadPage);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            function insertTag(tag) {
                message.editor.insertPlainText("[" + tag + "]" + message.editor.selectedText + "[/" + tag + "]" );
            }
            
            TextField {
                id: recipient
                visible: true
                hintText: qsTr("New MP to... ")
            }
            
            TextField {
                id: newMessageCaption
                visible: true
                hintText: qsTr("Subject MP: ")
            }
            TextArea {
                id: message
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            
            }
        }
        
        attachedObjects: [
            PostMessageController {
                id: postMessageController
                
                onComplete: {
                    nav.pop()
                    message.text = "";
                    mode = 1;
                    newMessageCaption.text = "";
                    pageThread.needUpdate = true;
                }
                
                onMessageLoaded: {
                    message.text = message_loaded;
                    activityIndicator.stop();
                }
            },
            ComponentDefinition {
                id: smileyPicker
                source: "SmileyPicker.qml"
            },
            ComponentDefinition {
                id: imageUploader
                source: "ImageUploader.qml"
            },
            OrientationHandler {
                id: orientationHandler
                onOrientationAboutToChange: {
                    if (orientation == UIOrientation.Landscape) {
                        webviewActionBar.preferredHeight = 80;
                    } else {
                        webviewActionBar.preferredHeight = 60;
                    }
                }
            },
            SearchKeyRetriever {
                id: searchKeyRetriever
            }
        ]
        
        actions: [
            ActionItem {
                title: "Smiley"
                imageSource: "asset:///images/whiteFace.png"
                onTriggered: {
                    if(!tpage)
                        tpage = smileyPicker.createObject(0);
                    nav.push(tpage);
                }
            },
            ActionItem {
                title: qsTr("Quote")
                imageSource: "asset:///images/icon_quote.png"
                onTriggered: {
                    message.editor.insertPlainText("[quote]" + message.editor.selectedText + "[/quote]");
                }
            },
            ActionItem {
                title: qsTr("Code")
                imageSource: "asset:///images/icon_code.png"
                onTriggered: {
                    message.editor.insertPlainText("[cpp]" + message.editor.selectedText + "[/cpp]");
                }
            },
            ActionItem {
                title: qsTr("Bold")
                imageSource: "asset:///images/icon_bold.png"
                onTriggered: {
                    message.editor.insertPlainText("[b]" + message.editor.selectedText + "[/b]");
                }
            },
            ActionItem {
                title: qsTr("Italic")
                imageSource: "asset:///images/icon_italic.png"
                onTriggered: {
                    message.editor.insertPlainText("[i]" + message.editor.selectedText + "[/i]");
                }
            },
            ActionItem {
                title: qsTr("Underline")
                imageSource: "asset:///images/icon_underline.png"
                onTriggered: {
                    message.editor.insertPlainText("[u]" + message.editor.selectedText + "[/u]");
                }
            }
        ]
        
        onCreationCompleted: {
            mode = 4;
            searchKeyRetriever.getKey();
        }
        
        onSmileyToAddChanged: {
            if(smileyToAdd == "")
                return;
            
            message.editor.insertPlainText(smileyToAdd);
        
        }
        
    }
    
    onPopTransitionEnded: {
        --navDepth;
        
        if(navDepth == 0) {
            _app.closeCard();
        }
        
    }
    
    onPushTransitionEnded: {
        ++navDepth;
    }
    
    
} 