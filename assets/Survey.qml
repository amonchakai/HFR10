import bb.cascades 1.4

Page {
    
    property string survey
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        ActivityIndicator {
            id: aIndic
            preferredHeight: 30
            horizontalAlignment: HorizontalAlignment.Center
        }
        
        ScrollView {    
            WebView {
                html: survey
                
                onMessageReceived: {
                    
                                        
                    var isSurvey = RegExp("SURVEY:([0-9]+)")
                    var match = message.data.match(isSurvey);
                    if(match) {
                        aIndic.start();
                        showThreadController.vote(match[1]);
                    }
                    
                    console.log(message.data);
                }
                                
                settings.textAutosizingEnabled: false 
                settings.zoomToFitEnabled: false  
            }
        }
        
    }
    
    onSurveyChanged: {
        aIndic.stop();
    }
}
