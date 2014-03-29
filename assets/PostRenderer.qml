import bb.cascades 1.2


Container {
 
    WebView {
        id: post
        html: "<!DOCTYPE html><html><head><style type=\"text/css\">"
        			+ "table {border-left:5px solid lightgray; } th {text-align:left; text-decoration:underline;} "	// render quotation table
                    + "body {font-size:25px;} "
                    + "p {font-size:25px;} "
        		+ "</style>" 
        		+ "</head><body>" + ListItemData.post + "</body></html>"
        settings.textAutosizingEnabled: false

    }
    
    
}

 