import bb.cascades 1.2

Container {
    
    id: surveyHeader
    
    function setSurvey(page) {
        survey.html = page;
    }
    
    horizontalAlignment: HorizontalAlignment.Fill
    layout: DockLayout {
    }
    
    //TODO: add last refreshed item
    Container {
        id: surveyContainer
        horizontalAlignment: HorizontalAlignment.Fill
        
        WebView {
            id: survey
            html: "" 
            
            onMessageReceived: {
                showThreadController.vote(message.data)
            }
        }
    }
}
