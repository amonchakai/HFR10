import bb.cascades 1.4
import Network.ProfileController 1.0

Page {
    property string profileUrl
    property variant feedbackPage
    
    titleBar: TitleBar {
        title: qsTr("Profile")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                nav.pop();
            }
        }
    }
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        ActivityIndicator {
            id: activityIndicator
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 60
        }
        
        ScrollView {
            
            Container {
                layout: StackLayout {}
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
                
                Container {
                    preferredWidth: ui.du(1)
                    preferredHeight: ui.du(1)
                }
                
                // --------------------------------------------------------------------------
                // Login settings
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("General information")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
            
                Container {
                    layout: DockLayout {
                        
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Pseudo")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: pseudo
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Email")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: email
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Birthday")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: birthday
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Personal website")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: site
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Gender")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: gender
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("City")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: city
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Job")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: job
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Hobby")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: hobby
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    minHeight: ui.du(4)
                    maxHeight: ui.du(4)
                }
                
                // --------------------------------------------------------------------------
                // Login settings
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Information related to the forum")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Status")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: status
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Number of messages posted")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: numberOfMessages
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Registration timestamp")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: registrationTimestamp
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Last message timestamp")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: lastMessage
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
            
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Quote")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        id: quote
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.textAlign: TextAlign.Right
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Signature")
                        textStyle.fontSize: FontSize.Default
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }
                    leftPadding: ui.du(1)
                    rightPadding: ui.du(1)
                }
                
                WebView {
                    id: signature
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                
                Container {
                    minHeight: ui.du(4)
                    maxHeight: ui.du(4)
                }
                
                // --------------------------------------------------------------------------
                // Feedback
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Feedback")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                Container {
                    layout: DockLayout {
                        
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        
                        Button {
                            id: showFeedbackVG
                            preferredWidth: horizontalAlignment.Fill
                            text: qsTr("Video games")
                            onClicked: {
                                if(!feedbackPage)
                                    feedbackPage = feedback.createObject();
                                feedbackPage.url = profileController.transationVG;
                                
                                nav.push(feedbackPage);
                            }
                        }
                        
                        Button {
                            id: showFeedback
                            preferredWidth: horizontalAlignment.Fill
                            text: qsTr("Buy & Sales")
                            onClicked: {
                                if(!feedbackPage)
                                    feedbackPage = feedback.createObject();
                                feedbackPage.url = profileController.transation;
                                
                                nav.push(feedbackPage);
                            }
                        }
                    }
                }
                
                
                Container {
                    minHeight: ui.du(4)
                    maxHeight: ui.du(4)
                }
                
                // --------------------------------------------------------------------------
                // Feedback
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Blacklist")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: qsTr("BlackList user")
                        color: Color.Red
                        onClicked: {
                            profileController.blacklistUser();
                        }
                    }
                }
            }
        }
    }
    
    
    onProfileUrlChanged: {
        profileController.loadProfile(profileUrl);
        activityIndicator.start();
    }
    
    attachedObjects: [
        ProfileController {
            id: profileController
            
            onComplete: {
                activityIndicator.stop();
                
                pseudo.text = profileController.pseudo;
                email.text = profileController.email;
                birthday.text = profileController.birthday;
                gender.text = profileController.gender;
                city.text = profileController.city;
                job.text = profileController.job;
                hobby.text = profileController.hobby;
                status.text = profileController.status;
                numberOfMessages.text = profileController.postCount;
                registrationTimestamp.text = profileController.registrationTime;
                lastMessage.text = profileController.lastmessage;
                if(Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark) {
                    quote.text = "<html><body>" + profileController.citation + "</body></html>";
                    signature.html = "<html><body>" + profileController.signature + "</body></html>";
                    site.text = "<html><body>" + profileController.site + "</body></html>";
                } else {
                    quote.text = "<html><head><style>html {background: #000000; color: #ffffff; }</style></head><body>" + profileController.citation + "</body></html>";
                    signature.html = "<html><head><style>html {background: #000000; color: #ffffff; }</style></head><body>" + profileController.signature + "</body></html>";
                    site.text = "<html><head><style>html {background: #000000; color: #ffffff; }</style></head><body>" + profileController.site + "</body></html>";
                }
                
                
                showFeedbackVG.text = qsTr("Video games ") + profileController.nbSalesVG;
                showFeedback.text = qsTr("Buy & Sales ") + profileController.nbSales;
                
                headerContainer.requestFocus();
                
            }
        },
        ComponentDefinition {
            id: feedback
            source: "Feedback.qml"
        }
    ]
}
