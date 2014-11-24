import bb.cascades 1.2
import Network.SearchKeyRetriever 1.0

NavigationPane {
    id: nav
    property variant tpage
    
	Page {
        
	    ScrollView {
	        
    	    Container {
    	        id: pageContainer
    	        layout: StackLayout {
    	            orientation: LayoutOrientation.TopToBottom
                }
    	        verticalAlignment: VerticalAlignment.Fill
    	        
    	        Label {
                 text: qsTr("Search")
                }
    	        TextField {
                	id: searchField
                    hintText: qsTr("Search")
                }
    	        
                DropDown {
                    id: searchType
                    Option {
                        text: qsTr("Every words")
                        value: 1
                    }
                    Option {
                        text: qsTr("At least one word")
                        value: 2
                        selected: true
                    }
                    Option {
                        text: qsTr("Advanced research")
                        value: 0
                    }
                    title: qsTr("Type of search")
                    
                }
                
                Label {
                    text: qsTr("From author")
                }
                TextField {
                    id: author
                    hintText: qsTr("Author")
                }
                
                
                Label {
                    text: qsTr("Search settings")
                }            
                DropDown {
                    id: categoryType
                    Option {
                        text: qsTr("Hardware")
                        value: 1
                    }
                    Option {
                        text: qsTr("Hardware - Périphériques")
                        value: 16
                    }
                    Option {
                        text: qsTr("Ordinateurs portables")
                        value: 15
                    }
                    Option {
                        text: qsTr("Technologies Mobiles")
                        value: 23
                    }
                    Option {
                        text: qsTr("Overclocking, Cooling & Modding")
                        value: 2
                    }
                    Option {
                        text: qsTr("Apple")
                        value: 25
                    }
                    Option {
                        text: qsTr("Video & Son")
                        value: 3
                    }
                    Option {
                        text: qsTr("Photo numérique")
                        value: 14
                        selected: true
                    }
                    Option {
                        text: qsTr("Jeux Video")
                        value: 5
                    }
                    Option {
                        text: qsTr("Windows & Software")
                        value: 4
                    }
                    Option {
                        text: qsTr("Réseaux grand public / SoHo")
                        value: 22
                    }
                    Option {
                        text: qsTr("Systèmes & Réseaux Pro")
                        value: 21
                    }
                    Option {
                        text: qsTr("Linux et OS Alternatifs")
                        value: 11
                    }
                    Option {
                        text: qsTr("Programmation")
                        value: 10
                    }
                    Option {
                        text: qsTr("Graphisme")
                        value: 12
                    }
                    Option {
                        text: qsTr("Achats & Ventes")
                        value: 6
                    }
                    Option {
                        text: qsTr("Emploi & Etudes")
                        value: 8
                    }
                    Option {
                        text: qsTr("Seti et projets distribués")
                        value: 9
                    }
                    Option {
                        text: qsTr("Discussions")
                        value: 13
                    }
                    title: qsTr("Category")
                }
                
                DropDown {
                    id: searchIn
                    Option {
                        text: qsTr("Subject title")
                        value: 1
                        selected: true
                    }
                    Option {
                        text: qsTr("Subject title and post content")
                        value: 3
                    }
                    Option {
                        text: qsTr("Post content")
                        value: 0
                    }
                    title: qsTr("Search in")
                }
                
                DropDown {
                    id: numberOfMessages
                    Option {
                        text: qsTr("20 first results")
                        value: 20
                        selected: true
                    }
                    Option {
                        text: qsTr("50 first results")
                        value: 50
                    }
                    Option {
                        text: qsTr("100 first results")
                        value: 100
                    }
                    Option {
                        text: qsTr("200 first results")
                        value: 200
                    }
                    title: qsTr("Display")
                }
                
                DropDown {
                    id: sortBy
                    Option {
                        text: qsTr("Timestamp last post")
                        value: 0
                        selected: true
                    }
                    Option {
                        text: qsTr("Timestamp last matching post")
                        value: 1
                    }
                    title: qsTr("Sort by")
                }
                
                
                Button {
                    id: submit
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Bottom
                    text: qsTr("Submit")
                    
                    onClicked: {
                        pageContainer.openSearchResult();
                    }
                }
                
                function openSearchResult() {
                    if(!tpage)
                        tpage = searchResult.createObject(0);
                    tpage.author = author.text;
                    tpage.searchType = searchType.selectedValue;
                    tpage.categoryType = categoryType.selectedValue;
                    tpage.searchIn = searchIn.selectedValue;
                    tpage.numberOfMessages = numberOfMessages.selectedValue;
                    tpage.sortBy = sortBy.selectedValue;
                    tpage.search = searchField.text;
                    tpage.hashKey = searchKeyRetriever.haskKey;
                    tpage.refresh = true;
                    
                    nav.push(tpage);
                }
    	    }
    	}
	    
        
	    
	    
        attachedObjects: [
            ComponentDefinition {
                id: searchResult
                source: "SearchResult.qml"
            },
            SearchKeyRetriever {
                id: searchKeyRetriever
            }
        ]
        
        
        onCreationCompleted: {
            searchKeyRetriever.getKey();
        }
	}
}
