import bb.cascades 1.2
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
            background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            preferredHeight: 40
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            Label {
                text: qsTr("Color management and notifications")
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
        
        // -------------------------------------------------
        // green
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            preferredHeight: 130
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Container {
                    preferredWidth: 15
                }
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                Container {
                    preferredHeight: 100
                    preferredWidth: 100
                    background: Color.create("#a8cc0a") 
                    verticalAlignment: VerticalAlignment.Center
                }
                Label {
                    text: qsTr("Green") + " - Notification"
                    verticalAlignment: VerticalAlignment.Center
                    preferredWidth: 380
                }
                ToggleButton {
                    id: greenHubNotif
                    checked: appSettings.notifGreen
                    verticalAlignment: VerticalAlignment.Center
                    onCheckedChanged: {
                        appSettings.notifGreen = greenHubNotif.checked;
                        appSettings.saveColors(); 
                    }
                }
                
            }
        }
        Container {
            preferredHeight: 2
            horizontalAlignment: HorizontalAlignment.Fill
            background: Color.LightGray
        }
        
        // -------------------------------------------------
        // green
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            preferredHeight: 130
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Container {
                    preferredWidth: 15
                }
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                Container {
                    preferredHeight: 100
                    preferredWidth: 100
                    background: Color.create("#27bbc6") 
                    verticalAlignment: VerticalAlignment.Center
                }
                Label {
                    text: qsTr("Blue") + " - Notification"
                    verticalAlignment: VerticalAlignment.Center
                    preferredWidth: 380
                }
                ToggleButton {
                    id: blueHubNotif
                    checked: appSettings.notifBlue
                    verticalAlignment: VerticalAlignment.Center
                    onCheckedChanged: {
                        appSettings.notifBlue = blueHubNotif.checked;
                        appSettings.saveColors(); 
                    }
                }
            
            }
        }
        Container {
            preferredHeight: 2
            horizontalAlignment: HorizontalAlignment.Fill
            background: Color.LightGray
        }
        
        // -------------------------------------------------
        // orange
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            preferredHeight: 130
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Container {
                    preferredWidth: 15
                }
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                Container {
                    preferredHeight: 100
                    preferredWidth: 100
                    background: Color.create("#fe5e05") 
                    verticalAlignment: VerticalAlignment.Center
                }
                Label {
                    text: qsTr("Orange") + " - Notification"
                    verticalAlignment: VerticalAlignment.Center
                    preferredWidth: 380
                }
                ToggleButton {
                    id: orangeHubNotif
                    checked: appSettings.notifOrange
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                    onCheckedChanged: {
                        appSettings.notifOrange = orangeHubNotif.checked;
                        appSettings.saveColors(); 
                    }
                }
            
            }
        }
        Container {
            preferredHeight: 2
            horizontalAlignment: HorizontalAlignment.Fill
            background: Color.LightGray
        }
        
        // -------------------------------------------------
        // pink
    
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            preferredHeight: 130
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
                    
            Container {
                  layout: StackLayout {
                       orientation: LayoutOrientation.LeftToRight
                   }
                        
                  Container {
                       preferredWidth: 15
                       horizontalAlignment: HorizontalAlignment.Left
                  }
                  horizontalAlignment: HorizontalAlignment.Fill
                  verticalAlignment: VerticalAlignment.Center
                  Container {
                      preferredHeight: 100
                      preferredWidth: 100
                      background: Color.create("#ef0061") 
                      verticalAlignment: VerticalAlignment.Center
                      horizontalAlignment: HorizontalAlignment.Left
                  }
                  Label {
                       text: qsTr("Pink") + " - Notification"
                       verticalAlignment: VerticalAlignment.Center
                       horizontalAlignment: HorizontalAlignment.Center
                       preferredWidth: 380
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
        }
        Container {
            preferredHeight: 2
            horizontalAlignment: HorizontalAlignment.Fill
            background: Color.LightGray
        }
        
        
        // -------------------------------------------------
        // purple
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            preferredHeight: 130
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Container {
                    preferredWidth: 15
                }
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                Container {
                    preferredHeight: 100
                    preferredWidth: 100
                    background: Color.create("#795ae0") 
                    verticalAlignment: VerticalAlignment.Center
                }
                Label {
                    text: qsTr("Purple") + " - Notification"
                    verticalAlignment: VerticalAlignment.Center
                    preferredWidth: 380
                }
                ToggleButton {
                    id: purpleHubNotif
                    checked: appSettings.notifPurple
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                    onCheckedChanged: {
                        appSettings.notifPurple = purpleHubNotif.checked;
                        appSettings.saveColors(); 
                    }
                }
            
            }
        }
        Container {
            preferredHeight: 2
            horizontalAlignment: HorizontalAlignment.Fill
            background: Color.LightGray
        }
        
        
        
    }
    
    attachedObjects: [
        AppSettings {
            id: appSettings
        }
    ]
}
