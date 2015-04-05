import bb.cascades 1.3
import conf.settings 1.0

Page {
    Container {
        id: headerContainer
        layout: StackLayout {    
            orientation: LayoutOrientation.TopToBottom
        }
        
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
        
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Top
        
        Container {
            preferredWidth: ui.du(1)
            preferredHeight: ui.du(1)
        }
        
        // --------------------------------------------------------------------------
        // Login settings
        Container {
            layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
            
            Label {
                text: qsTr("Color management and notifications")
                textStyle.fontSize: FontSize.Large
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Bottom
            }
        
        }
        Divider { }
        
        // -------------------------------------------------
        // green
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout { }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Container {
                    preferredWidth: ui.du(1.5)
                }
                
                Container {
                    preferredHeight: ui.du(10)
                    preferredWidth: ui.du(10)
                    background: Color.create("#a8cc0a") 
                    verticalAlignment: VerticalAlignment.Center
                }
                Label {
                    text: qsTr("Green") + " - Notification"
                    verticalAlignment: VerticalAlignment.Center
                }
                
            }
            
            
            ToggleButton {
                id: greenHubNotif
                checked: appSettings.notifGreen
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                onCheckedChanged: {
                    appSettings.notifGreen = greenHubNotif.checked;
                    appSettings.saveColors(); 
                }
            }                   
            
        }
        
        

        Divider { }
        
        // -------------------------------------------------
        // Blue
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout { }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Container {
                    preferredWidth: ui.du(1.5)
                }
                
                Container {
                    preferredHeight: ui.du(10)
                    preferredWidth: ui.du(10)
                    background: Color.create("#27bbc6") 
                    verticalAlignment: VerticalAlignment.Center
                }
                Label {
                    text: qsTr("Blue") + " - Notification"
                    verticalAlignment: VerticalAlignment.Center
                }
            
            }
            
            
            ToggleButton {
                id: blueHubNotif
                checked: appSettings.notifBlue
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                onCheckedChanged: {
                    appSettings.notifBlue = blueHubNotif.checked;
                    appSettings.saveColors(); 
                }
            }
        
        }
        
        
        Divider { }
        
        // -------------------------------------------------
        // orange
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout { }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Container {
                    preferredWidth: ui.du(1.5)
                }
                
                Container {
                    preferredHeight: ui.du(10)
                    preferredWidth: ui.du(10)
                    background: Color.create("#fe5e05") 
                    verticalAlignment: VerticalAlignment.Center
                }
                Label {
                    text: qsTr("Orange") + " - Notification"
                    verticalAlignment: VerticalAlignment.Center
                }
            
            }
            
            
            ToggleButton {
                id: orangeHubNotif
                checked: appSettings.notifOrange
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                onCheckedChanged: {
                    appSettings.notifOrange = orangeHubNotif.checked;
                    appSettings.saveColors(); 
                }
            }
        
        }
        
        
        Divider { }
        
        // -------------------------------------------------
        // pink
        
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout { }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Container {
                    preferredWidth: ui.du(1.5)
                }
                
                Container {
                    preferredHeight: ui.du(10)
                    preferredWidth: ui.du(10)
                    background: Color.create("#ef0061") 
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                Label {
                    text: qsTr("Pink") + " - Notification"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            
            }
            
            
            ToggleButton {
                id: pinkHubNotif
                checked: appSettings.notifPink
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                onCheckedChanged: {
                    appSettings.notifPink = pinkHubNotif.checked;
                    appSettings.saveColors(); 
                }
            }        
        }
    
        
        Divider { }
        
        
        // -------------------------------------------------
        // purple
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout { }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Container {
                    preferredWidth: ui.du(1.5)
                }
                
                Container {
                    preferredHeight: ui.du(10)
                    preferredWidth: ui.du(10)
                    background: Color.create("#795ae0") 
                    verticalAlignment: VerticalAlignment.Center
                }
                Label {
                    text: qsTr("Purple") + " - Notification"
                    verticalAlignment: VerticalAlignment.Center
                    preferredWidth: ui.du(38)
                }
            
            }
            
            
            ToggleButton {
                id: purpleHubNotif
                checked: appSettings.notifPurple
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                onCheckedChanged: {
                    appSettings.notifPurple = purpleHubNotif.checked;
                    appSettings.saveColors(); 
                }
            }
        }
        
        
        Divider { }
        
        
        
    }
    
    attachedObjects: [
        AppSettings {
            id: appSettings
        }
    ]
}
