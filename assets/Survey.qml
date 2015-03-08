import bb.cascades 1.4

Page {
    
    property string survey
    
    ScrollView {
        Container {
            WebView {
                html: survey
                
                onMessageReceived: {
                    
                                        
                    var isSurvey = RegExp("SURVEY:([0-9]+)")
                    var match = message.data.match(isSurvey);
                    if(match)
                        showThreadController.vote(match[1]);
                    
                    console.log(message.data);
                }
                                
                settings.textAutosizingEnabled: false 
                settings.zoomToFitEnabled: false  
            }
        }
        
    }
}
