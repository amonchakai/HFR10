import bb.cascades 1.2
import conf.settings 1.0

Container {
    
    id: postRenderContainer
    function adjustBackGroundAndcolor(style) {
        if(style == VisualStyle.Bright) {
        	return "} ";
        } else {
            return "background-color:#000000; color:#DDDDDD; } a:link {color:#3cb1f4; } a:visited {color:#3cb1f4; }  ";
        }   
    }
    
    WebView {
        id: post
        html: "<!DOCTYPE html><html><head><style type=\"text/css\">"
        	        + "div.quote { border-left:5px solid lightgray; padding-left:5px; } div.header { font-weight:bold;  text-decoration:underline } "	// render quotation table
                    + "body {font-size:" + appSettings.fontSize.toString()  + "px; " + postRenderContainer.adjustBackGroundAndcolor(Application.themeSupport.theme.colorTheme.style) // switch webview color based on theme
                    + "p {font-size:" + appSettings.fontSize.toString() + "px;} img {max-width:100%; }"
                    + "</style><script>function sendURL(url) { navigator.cascades.postMessage(url); }</script>" 
                    	+ "</head><body>" + ListItemData.post 
        		+ "</body></html>"
        settings.textAutosizingEnabled: false

		onNavigationRequested: {
    		if(request.navigationType != WebNavigationType.Other) {
                request.action = WebNavigationRequestAction.Ignore;
                
    		    if(request.url.toString().substr(0,31) == "http://forum.hardware.fr/forum2") {
                    headerContainer.ListItem.view.redirectWithinApp(request.url.toString().substring(24));
                } else {
                    var urlTopic = RegExp("sujet_[0-9]+_[0-9]+.htm")
                    if(urlTopic.test(request.url.toString()))
                        headerContainer.ListItem.view.redirectWithinApp(request.url.toString().substring(24));
                    else
                        headerContainer.ListItem.view.invokeWebBrowser(request.url);
                }

    		} else { 
    			request.action = WebNavigationRequestAction.Accept;
    		}
    	}
		onMessageReceived: {
            headerContainer.ListItem.view.pushNewUrl(message.data)
      	}
		
		onLoadingChanged: {
            if(loadRequest.status == WebLoadStatus.Succeeded)
                headerContainer.ListItem.view.notifyWebViewLoaded();
		}
    }
    
    attachedObjects: [
        Settings {
            id: appSettings
        }
    ]
    
    
}

 