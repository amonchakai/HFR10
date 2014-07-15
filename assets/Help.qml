import bb.cascades 1.2

Page {
    signal done ()
    
    titleBar: TitleBar {
        title: qsTr("About")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                done();
            }
        }
    }
    
    ScrollView {
        id: scrollView
        accessibility.name: "scrollView"
        
	    WebView {
            accessibility.name: "viewHelp"
	        
	        url: "local://assets/help/about_fr.html"        
	        
	        onNavigationRequested: {
	            if(request.navigationType != WebNavigationType.Other) {
	                request.action = WebNavigationRequestAction.Ignore;
	                
	                invokeWebBrowser(request.url);
	            
	            } else { 
	                request.action = WebNavigationRequestAction.Accept;
	            }
	        }
	        
	        function invokeWebBrowser(urlPage) {
	            linkInvocation.query.uri = urlPage;
	        }
	        
	        attachedObjects: [
	            Invocation {
	                id: linkInvocation
	                
	                query {
	                    onUriChanged: {
	                        linkInvocation.query.updateQuery();
	                    }
	                }
	                
	                onArmed: {
	                    trigger("bb.action.OPEN");
	                }
	            }
	        ]
	    }
    }
}
