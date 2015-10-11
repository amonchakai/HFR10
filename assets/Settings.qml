import bb.cascades 1.3
import Network.LoginController 1.0
import conf.settings 1.0

NavigationPane {
    id: nav
    property variant tpage
    property variant colorPage
    property variant logPage
    property variant blacklistPage
    signal done ()
    
	Page {
        id: settingsPage
        
        
        titleBar: TitleBar {
            title: qsTr("Settings")
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    // Emit the custom signal here to indicate that this page needs to be closed
                    // The signal would be handled by the page which invoked it
                    nav.done();
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Save")
                onTriggered: {
                    appSettings.saveSettings(); 
                    nav.done();
                }
            }
        }
        
        ScrollView {
            id: settingPage
            property string userName;
            
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
                        text: qsTr("Profile")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                    
                }
                Divider { }
                                
                Container {
                    layout: DockLayout { }
                    background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#131313") : Color.create("#fafafaff")
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Left
                        
                        Container {
                            preferredWidth: ui.du(.1)
                        }
                        
                        
                        ImageView {
                            imageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/avatar_white.png" : "asset:///images/avatar_black.png"
                            preferredHeight: ui.du(3)
                            preferredWidth: ui.du(3)
                            verticalAlignment: VerticalAlignment.Center
                            scalingMethod: ScalingMethod.AspectFit
                            
                        }
                        
                        TextField {
                            enabled: false
                            id: userLabel
                            text: settingPage.userName
                            verticalAlignment: VerticalAlignment.Center
                            
                        }

                    }
                                        
                    
                    
                    // Commit button
                    Button {
                        id: loginButton
                        text: qsTr("Connect")
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        preferredWidth: ui.du(30)
                        onClicked: {
                            if(!tpage)
                                tpage = loginPage.createObject();
                            nav.push(tpage);
                        }
                        visible: !loginController.isLogged()
                    }
                    
                    
                    Button {
                        id: logOutButton
                        text: qsTr("log out");
                        preferredWidth: ui.du(30)
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        onClicked: {
                            loginController.logOut();
                            loginButton.setVisible(true);
                            logOutButton.setVisible(false);
                            userLabel.setText(qsTr("User: "));
                        }
                        visible: loginController.isLogged()
                    }
                    
                }
    
    	            	        
                // --------------------------------------------------------------------------
                // Font size settings
                Container {
                    preferredHeight: ui.du(4)
                }
                // --------------------------------------------------------------------------
                // Login settings
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Visual appearance")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                    
    	        Label {
                    margin.leftOffset: ui.du(1)
    	            id: setFontSizeLabel
                    text: qsTr("Font size: ") + appSettings.fontSize.toString() + "px"
                }
    	            
    	        Slider {
    	            id: sliderFontSize
    	            fromValue: 15
                    toValue: 60
                    value: appSettings.fontSize
    	            horizontalAlignment: HorizontalAlignment.Fill
    	                
    	                
    	            onValueChanged: {
    	                appSettings.fontSize = value;
    	                setFontSizeLabel.text = qsTr("Font size: ") + appSettings.fontSize.toString() + "px"
    	             }
    
                }
    	        
    	        
                // --------------------------------------------------------------------------
                // smiley settings
                
                Label {
                    margin.leftOffset: ui.du(1)
                    id: setSmileySizeLabel
                    text: qsTr("Smiley size: ") + (appSettings.smileySize*40).toString() + "%"
                }
                
                Slider {
                    id: sliderSmileySize
                    fromValue: 1
                    toValue: 10
                    value: appSettings.smileySize
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    
                    onValueChanged: {
                        appSettings.smileySize = value;
                        setSmileySizeLabel.text = qsTr("Smiley size: ") + (appSettings.smileySize*20).toString() + "%"
                    }
                
                }
                
                // --------------------------------------------------------------------------
                // Theme setting
                DropDown {
                    id: theme
                    title: qsTr("Visual Theme")
                    options: [
                        Option {
                            text: qsTr("Bright")
                            selected: appSettings.theme == 1 ? true : false
                            value: 1
                        },
                        Option {
                            text: qsTr("Dark")
                            selected: appSettings.theme == 2 ? true : false
                            value: 2
                        } 
                    ]
                    onSelectedOptionChanged: {
                        appSettings.theme = theme.selectedOption.value;
                    }
                    
                } 
                
                
                Container {
                    preferredHeight: ui.du(4)
                }
                // --------------------------------------------------------------------------
                // Features settings
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Feature settings")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                
                
                Container {
                    preferredHeight: 90
                    horizontalAlignment: HorizontalAlignment.Fill
                    layout: DockLayout {  }
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Right
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        ToggleButton {
                            verticalAlignment: VerticalAlignment.Center
                            
                            id: togglebutton
                            checked: appSettings.autoRefresh == 1
                            onCheckedChanged: {
                                if(checked)
                                    appSettings.autoRefresh = 1;
                                else 
                                    appSettings.autoRefresh = 0;
                            }
                        }
                        Container {
                            preferredWidth: ui.du(0.5)
                        }
                        
                    }
                    Label {
                        margin.leftOffset: ui.du(1)
                         id: status
                         verticalAlignment: VerticalAlignment.Center
                         horizontalAlignment: HorizontalAlignment.Left
                         text: qsTr("Refresh favorite automatically")
                    }
                }
                
                
                Container {
                    preferredHeight: 90
                    horizontalAlignment: HorizontalAlignment.Fill
                    layout: DockLayout {  }
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Right
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        ToggleButton {
                            verticalAlignment: VerticalAlignment.Center
                            
                            id: togglebuttonLeaveApp
                            checked: appSettings.askLeaveApp == 1
                            onCheckedChanged: {
                                if(checked)
                                    appSettings.askLeaveApp = 1;
                                else 
                                    appSettings.askLeaveApp = 0;
                            }
                        }
                        Container {
                            preferredWidth: ui.du(0.5)
                        }
                    
                    }
                    Label {
                        margin.leftOffset: ui.du(1)
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Left
                        text: qsTr("Ask before leaving application")
                    }
                }
                
                Button {
                    id: blacklist
                    horizontalAlignment: HorizontalAlignment.Fill
                    text: qsTr("Manage blacklist")
                    onClicked: {
                        if(!blacklistPage)
                            blacklistPage = blackListManagement.createObject();
                        nav.push(blacklistPage);
                    }
                }
                
                Container {
                    preferredHeight: ui.du(4)
                }
                // --------------------------------------------------------------------------
                // hub settings
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Hub integration")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                
                Container {
                    preferredHeight: 90
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    
                    layout: DockLayout { }
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Right
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        
                        ToggleButton {
                            verticalAlignment: VerticalAlignment.Center
                            
                            id: togglebuttonHub
                            checked: appSettings.hubIntegration
                   
                            onCheckedChanged: {
                                appSettings.hubIntegration = checked;
                             
                                colorManagementButton.visible = togglebuttonHub.checked;
                                hubRefreshRateSlider.visible  = togglebuttonHub.checked;
                                    
                            }
                        }
                        
                        Container {
                            preferredWidth: ui.du(0.5)
                        }
                        
                    }
                    Label {
                        margin.leftOffset: ui.du(1)
                        id: statusHub
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Left
                        text: qsTr("Enable HUB integration")
                    }
                }
                
                Button {
                    id: updateHub
                    visible: togglebuttonHub.checked
                    horizontalAlignment: HorizontalAlignment.Fill
                    text: qsTr("Update HUB")
                    onClicked: {
                        appSettings.updateHub();
                    }
                }
                
                Button {
                    id: colorManagementButton
                    visible: togglebuttonHub.checked
                    horizontalAlignment: HorizontalAlignment.Fill
                    text: qsTr("Color management")
                    onClicked: {
                        if(!colorPage)
                            colorPage = colorManagement.createObject();
                        nav.push(colorPage);
                    }
                }
                
                function refreshToIndex() {
                    switch(appSettings.hubRefreshRate) {
                        case 15:
                            return 0;
                            
                        case 20:
                            return 1;
                            
                        case 30:
                            return 2;
                            
                        case 45:
                            return 3;
                            
                        case 60:
                            return 4;
                            
                        case 90:
                            return 5;
                            
                        case 120:
                            return 6;
                            
                        case 180:
                            return 7;
                    }
                    
                    return 0;
                }
                
                DropDown {
                    visible: togglebuttonHub.checked
                    id: hubRefreshRateSlider
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    title: qsTr("Refresh HUB every: ") 
                    options: [
                        Option {
                            text: "15 minutes"
                            value: 15
                        },
                        Option {
                            text: "20 minutes"
                            value: 20
                        },
                        Option {
                            text: "30 minutes"
                            value: 30
                        },
                        Option {
                            text: "45 minutes"
                            value: 45
                        },
                        Option {
                            text: "60 minutes"
                            value: 60
                        },
                        Option {
                            text: "90 minutes"
                            value: 90
                        },
                        Option {
                            text: "120 minutes"
                            value: 120
                        },
                        Option {
                            text: "180 minutes"
                            value: 180
                        }
                    ]
                    
                    onSelectedOptionChanged: {
                        appSettings.hubRefreshRate = selectedOption.value;
                    }
                    
                    selectedIndex: headerContainer.refreshToIndex();
                    
                }
                

                
                Container {
                    preferredHeight: ui.du(4)
                }
                // --------------------------------------------------------------------------
                // Cache settings
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Maintenance")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                
                Button {
                    text: qsTr("Clear image cache")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        loginController.clearImageCache();
                    }
                }           
                  
                Container {
                    layout: DockLayout { }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Enable logs")
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        margin.leftOffset: ui.du(1)
                    }
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        
                        ToggleButton {
                            id: enableLogs
                            verticalAlignment: VerticalAlignment.Center
                            checked: appSettings.enableLogs
                            onCheckedChanged: {
                                appSettings.enableLogs = checked;
                            }
                        }    
                        
                        Container {
                            preferredWidth: ui.du(0.5)
                        }    
                    }
                    
                
                }
                
                Button {
                    text: qsTr("Logs")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        if(!logPage)
                            logPage = applicationLog.createObject();
                        nav.push(logPage);
                    }
                }
    	    }
    	    
    	    
    	    attachedObjects: [
                LoginController {
                    id: loginController
                },
                ComponentDefinition {
                    id: loginPage
                    source: "LoginForm.qml"
                
                },
                AppSettings {
                    id: appSettings
                },
                ComponentDefinition {
                    id: applicationLog
                    source: "ApplicationLog.qml"
                },
                ComponentDefinition {
                    id: colorManagement
                    source: "ColorManagement.qml"
                },
                ComponentDefinition {
                    id: blackListManagement
                    source: "BlackListManagement.qml"
                }
    	    ]
    	    
            onUserNameChanged: {
                console.debug("name changed");
                userLabel.setText(userName);
            }
    	}
    } 
	
	onCreationCompleted: {
        settingPage.userName =  loginController.savedlogin;
    }
	
	onPopTransitionEnded: {
	    
        userLabel.setText(loginController.savedlogin);
        loginButton.setVisible(!loginController.isLogged());
        logOutButton.setVisible(loginController.isLogged());
    }
	
}
