import bb.cascades 1.2
import Network.LoginController 1.0


Page {
    Container {
        layout: StackLayout {}
        
        // Login + Text field
        Label {
            text: qsTr("Login")
        }
        TextField {
            id: loginField
            accessibility.name: "login"
        }   
        
        
        // Password + Text field
        Label {
            text: qsTr("Password")
        }
        TextField {
            id: passwordField
            inputMode: TextFieldInputMode.Password
            accessibility.name: "password"

        }
        
        attachedObjects: [
            LoginController {
                id: loginController
            }
        ]
        
        // Commit button
        Button {
            text: qsTr("OK")
            horizontalAlignment: HorizontalAlignment.Fill
            onClicked: {
                loginController.login(loginField.text,passwordField.text)
            }
        }
            
    }
}
