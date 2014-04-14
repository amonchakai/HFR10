import bb.cascades 1.2
import Network.LoginController 1.0
import conf.settings 1.0

NavigationPane {
    id: nav
    property variant tpage
    
	Page {
        id: settingPage
        property string userName;
        
	    Container {
	        layout: StackLayout {}
	        
            // --------------------------------------------------------------------------
            // Login settings
            
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
                    appSettings.saveSettings(); 
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
                    appSettings.saveSettings(); 
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
            Settings {
                id: appSettings
            }
	    ]
	    
        onUserNameChanged: {
            console.debug("name changed");
            userLabel.setText(qsTr("User: ") + userName);
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
