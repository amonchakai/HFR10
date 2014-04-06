import bb.cascades 1.2
import conf.settings 1.0

Container {
    id: postRenderContainer
    function adjustBackGroundAndcolor(style) {
        if(style == VisualStyle.Bright) {
        	return "} ";
        } else {
            return "background-color:#000000; color:#FFFFFF; } ";
        }   
    }
 
    WebView {
        id: post
        html: "<!DOCTYPE html><html><head><style type=\"text/css\">"
        			+ "table {border-left:5px solid lightgray; } th {text-align:left; text-decoration:underline;} "	// render quotation table
                    + "body {font-size:" + appSettings.fontSize.toString()  + "px; " + postRenderContainer.adjustBackGroundAndcolor(Application.themeSupport.theme.colorTheme.style) // switch webview color based on theme
                    + "p {font-size:" + appSettings.fontSize.toString() + "px;} "
        		+ "</style>" 
        		+ "</head><body>" + ListItemData.post + "</body></html>"
        settings.textAutosizingEnabled: false

		onNavigationRequested: {
    		if(request.navigationType != WebNavigationType.Other) {
    		    request.action = WebNavigationRequestAction.Ignore;
                headerContainer.ListItem.view.invokeWebBrowser(request.url);
    		} else { 
    			request.action = WebNavigationRequestAction.Accept;
    		}
    	}
    }
    
    attachedObjects: [
        Settings {
            id: appSettings
        }
    ]
    
    
}

 