import bb.cascades 1.2
import Network.LoginController 1.0
import conf.settings 1.0

NavigationPane {
    id: nav
    property variant tpage
    property variant colorPage
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
    	        
                // --------------------------------------------------------------------------
                // Login settings
                Container {
                    background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    preferredHeight: 50
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Label {
                        text: qsTr("Login settings")
                        textStyle.fontSize: FontSize.XSmall
                    }
                } 
                Container {
                    background: Color.create("#0089bf") 
                    minHeight: 4
                    maxHeight: 4
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                
    	        Label {
    	            id: userLabel
                    text: qsTr("User: ") + settingPage.userName
                }
    
    	        
    	        // Commit button
    	        Button {
    	            id: loginButton
    	            text: qsTr("Connect")
    	            horizontalAlignment: HorizontalAlignment.Fill
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
    	            horizontalAlignment: HorizontalAlignment.Fill
    	            onClicked: {
    	                loginController.logOut();
    	                loginButton.setVisible(true);
    	                logOutButton.setVisible(false);
                        userLabel.setText(qsTr("User: "));
    	            }
    	            visible: loginController.isLogged()
    	        }
    	        
    	        
                // --------------------------------------------------------------------------
                // Font size settings
                
                Container {
                    background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    preferredHeight: 50
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Label {
                        text: qsTr("Visual appearance")
                        textStyle.fontSize: FontSize.XSmall
                    }
                } 
                Container {
                    background: Color.create("#0089bf") 
                    minHeight: 4
                    maxHeight: 4
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                    
    	        Label {
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
                    background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    preferredHeight: 50
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Label {
                        text: qsTr("Feature settings")
                        textStyle.fontSize: FontSize.XSmall
                    }
                } 
                Container {
                    background: Color.create("#0089bf") 
                    minHeight: 4
                    maxHeight: 4
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                
                Container {
                    preferredHeight: 90
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
                    Label {
                         id: status
                         verticalAlignment: VerticalAlignment.Center
                         text: qsTr("Refresh favorite automatically")
                    }
                }
                
                /*
                Button {
                    horizontalAlignment: HorizontalAlignment.Fill
                    text: qsTr("Color management")
                    onClicked: {
                        if(!colorPage)
                            colorPage = colorManagement.createObject();
                        nav.push(colorPage);
                    }
                }
                */
                
                // --------------------------------------------------------------------------
                // hub settings
                /*
                Container {
                    background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    preferredHeight: 40
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Label {
                        text: qsTr("Hub integration")
                        textStyle.fontSize: FontSize.XSmall
                    }
                } 
                Container {
                    background: Color.create("#0089bf") 
                    minHeight: 4
                    maxHeight: 4
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                
                
                Container {
                    preferredHeight: 90
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    ToggleButton {
                        verticalAlignment: VerticalAlignment.Center
                        id: togglebuttonHub
                        checked: true
                        onCheckedChanged: {
                            appSettings.hubIntegration = togglebutton.checked;
                            appSettings.saveSettings();
                            
                        }
                    }
                    Label {
                        id: statusHub
                        verticalAlignment: VerticalAlignment.Center
                        text: qsTr("Enable HUB integration")
                    }
                }
                
                
                Label {
                    id: hubRefreshRate
                    text: qsTr("Refresh HUB every: ") + (hubRefreshRateSlider.value).toString() + " minutes."
                }
                
                Slider {
                    id: hubRefreshRateSlider
                    fromValue: 1
                    toValue: 120
                    value: appSettings.hubRefreshRate
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    
                    onValueChanged: {
                        appSettings.hubRefreshRate = hubRefreshRateSlider.value;
                        hubRefreshRate.text = qsTr("Refresh HUB every: ") + (appSettings.hubRefreshRate).toString() + " minutes."
                        
                    }
                
                }
*/
                
                
                
                // --------------------------------------------------------------------------
                // cache settings
                
                Container {
                    background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    preferredHeight: 50
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Label {
                        text: qsTr("Maintenance")
                        textStyle.fontSize: FontSize.XSmall
                    }
                } 
                Container {
                    background: Color.create("#0089bf") 
                    minHeight: 4
                    maxHeight: 4
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                
                
                Button {
                    text: qsTr("Clear image cache")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        loginController.clearImageCache();
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
                    id: colorManagement
                    source: "ColorManagement.qml"
                
                }
    	    ]
    	    
            onUserNameChanged: {
                console.debug("name changed");
                userLabel.setText(qsTr("User: ") + userName);
            }
    	}
    } 
	
	onCreationCompleted: {
        settingPage.userName =  loginController.savedlogin;
    }
	
	onPopTransitionEnded: {
	    
        userLabel.setText(qsTr("User: ") + loginController.savedlogin);
        loginButton.setVisible(!loginController.isLogged());
        logOutButton.setVisible(loginController.isLogged());
    }
	
}
