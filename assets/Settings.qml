import bb.cascades 1.2
import Network.LoginController 1.0

NavigationPane {
    id: nav
    
	Page {
        id: settingPage
        property string userName;
        
	    Container {
	        layout: StackLayout {}
	        
	        Label {
	            id: userLabel
                text: qsTr("User: ") + userName
            }

	        
	        // Commit button
	        Button {
	            id: loginButton
	            text: qsTr("Connect")
	            horizontalAlignment: HorizontalAlignment.Fill
	            onClicked: {
                    var page = loginPage.createObject();
                    nav.push(page);
	            }
	            visible: !loginController.isLogged()
	        }
	        
	        
	        // --------------------------------------------------------------------------
	        // log out
	        
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
	    }
	    
	    
	    attachedObjects: [
            LoginController {
                id: loginController
            },
            ComponentDefinition {
                id: loginPage
                source: "LoginForm.qml"
            
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
