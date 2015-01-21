import bb.cascades 1.2
import Network.ExploreCategoryController 1.0
import bb.system 1.2

Page {
    id: pageCat
    property string  urlPage
    property string  caption
    property string  subCatXml
    property int 	 flagType
    property int     subCatIndex
    property variant tpage
    property variant createPage
    property variant subCatPickerPage
        
    titleBar: TitleBar {
        id: segmentedTitle
        kind: TitleBarKind.Segmented
        scrollBehavior: TitleBarScrollBehavior.Sticky
        
        // The segmented control decides which filter should be set on the
        // dataModel used by the photo bucket list.
        options: [
            Option {
//                text: qsTr("All") + Retranslate.onLanguageChanged
                imageSource: "asset:///images/icon_global2.png"
                value: ("all")
            },
            Option {
//                text: qsTr("Fav.") + Retranslate.onLanguageChanged
                imageSource: "asset:///images/icon_favorite.png"
                value: ("favorites")
            },
            Option {
//                text: qsTr("Follow") + Retranslate.onLanguageChanged
                imageSource: "asset:///images/icon_drap_participe.png"
                value: ("participate")
            },
            Option {
//                text: qsTr("Read") + Retranslate.onLanguageChanged
                imageSource: "asset:///images/icon_drap_lecture.png"
                value: ("read")
            }
        ]
        
        
        
        onSelectedValueChanged: {
            if (selectedValue == "all")
                flagType = 0;
                
            if(selectedValue == "participate")
                flagType = 1;
            	
            if(selectedValue == "favorites")
                flagType = 3;
                
            if(selectedValue == "read")
                flagType = 2;
        }
    }
    
    Container {
        ActivityIndicator {
            id: activityIndicator
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 60
        }
        
        ListView {            
            objectName: "listCats"
            
            // ------------------------------------------------------------------
            // Pull to refresh
            signal refreshTriggered()
            property bool loading: false
            id: listCats
            leadingVisualSnapThreshold: 2.0
            leadingVisual: RefreshHeader {
                id: refreshHandler
                onRefreshTriggered: {
                    listCats.refreshTriggered();
                }
            }
            onTouch: {
                refreshHandler.onListViewTouch(event);
            }
            onLoadingChanged: {
                refreshHandler.refreshing = refreshableList.loading;
                
                if(!refreshHandler.refreshing) {
                    // If the refresh is done 
                    // Force scroll to top to ensure that all items are visible
                    scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
                }
            }
            
            // ------------------------------------------------------------------
            // view
            focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
            
            dataModel: GroupDataModel {
                id: theModel
                sortingKeys: ["category"]
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Header {
                        title: ListItemData
                    }
                },
                ListItemComponent {
                    type: "item"
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        id: titleContainer
                        function typeFromReadStatus(read) {
                            if(read)
                                return FontWeight.Bold;
                            else 
                                return FontWeight.Normal;
                        }
                        
                        Container {
                            preferredWidth: 8
                        }

                        Container {
                            
                            
                            layout: StackLayout {
                                orientation: LayoutOrientation.TopToBottom
                            }
                            verticalAlignment: VerticalAlignment.Top
                            Label {
                                text: ListItemData.title
                                textStyle.fontWeight: titleContainer.typeFromReadStatus(ListItemData.read)
                            }
                            
                            Container {
                                layout: DockLayout {
                                }
                                horizontalAlignment: HorizontalAlignment.Fill
                                
                                Label {
                                    text: ListItemData.lastAuthor + " - " + ListItemData.timestamp
                                    horizontalAlignment: HorizontalAlignment.Right
                                    textStyle {
                                        base: SystemDefaults.TextStyles.SmallText
                                        color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#00a8df")  : Color.Blue
                                    }
                                }
                                Container {
                                    id: pageNumContainter
                                    
                                    // ---------------------------------------------------------------
                                    // layout functions...
                                    
                                    function getFlag(flagCode) {
                                        switch(flagCode) {
                                            case 0:
                                                return "";
                                                
                                            case 1:
                                                return "asset:///images/icon_drap_participe.gif"
                                                
                                            case 2:
                                                return "asset:///images/icon_drap_lecture.gif"
                                                
                                            case 3:
                                                return "asset:///images/icon_favori.gif"
                                                
                                        }
                                        return "";
                                    
                                    }
                                    
                                    
                                    // ---------------------------------------------------------------
                                    // list item itself...
    
                                    
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    horizontalAlignment: HorizontalAlignment.Left
                                    ImageView {
                                        imageSource: pageNumContainter.getFlag(ListItemData.flagType)
                                        horizontalAlignment: HorizontalAlignment.Left
                                    }
                                    Label {
                                        text: ListItemData.pages
                                        horizontalAlignment: HorizontalAlignment.Right
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SmallText
                                            color: Color.Gray
                                        }
                                    }
                                }
                                
                            }
                            Divider {}
                        }
                        
                        contextActions: [
                            ActionSet {
                                title: qsTr("Navigation")
                                
                                ActionItem {
                                    title: qsTr("First page")
                                    imageSource: "asset:///images/icon_prev_all.png"
                                    onTriggered: {
                                        titleContainer.ListItem.view.gotoPage(ListItemData.urlFirstPage, ListItemData.title)
                                    }
                                }
                                ActionItem {
                                    title: qsTr("Last page")
                                    imageSource: "asset:///images/icon_next_all.png"
                                    onTriggered: {
                                        titleContainer.ListItem.view.gotoPage(ListItemData.urlLastPage, ListItemData.title)
                                    }
                                }
                                ActionItem {
                                    title: qsTr("Last page read")
                                    imageSource: "asset:///images/icon_next.png"
                                    onTriggered: {
                                        titleContainer.ListItem.view.gotoPage(ListItemData.urlLastPostRead, ListItemData.title)
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
            
            function gotoPage(urlFirstPage, titleTopic) {
                if(!tpage) {
                    tpage = threadPage.createObject();
                }
                // Set the url of the page to load and thread caption. 
                tpage.urlPage = urlFirstPage
                tpage.caption   = titleTopic
                
                nav.push(tpage);
            }
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                
                // Create the content page and push it on top to drill down to it.
                if(!tpage) {
                    tpage = threadPage.createObject();
                }
                
                // Set the url of the page to load and thread caption. 
                if(chosenItem.urlLastPostRead != "")
                    tpage.urlPage = chosenItem.urlLastPostRead;
                else
                    tpage.urlPage = chosenItem.urlFirstPage;
                tpage.caption   = chosenItem.title
                
                nav.push(tpage);
            }
            
            onRefreshTriggered: {
                activityIndicator.start();
                exploreCategoryController.refresh();
            }
            
        }
        
    }
    
    attachedObjects: [
        ExploreCategoryController {
            id: exploreCategoryController
            
            onComplete: {
                activityIndicator.stop()
                listCats.requestFocus();
                listTopic = listCats;
            }
        }, 
        ComponentDefinition {
            id: threadPage
            source: "ThreadPage.qml"
        },
        ComponentDefinition {
            id: subcatPicker
            source: "SubCategoryPicker.qml"
        },
        ComponentDefinition {
            id: postMessage
            source: "PostMessage.qml"
        }
    ] 
    
    actions: [
        ActionItem {
            title: qsTr("Sub-Category")
            imageSource: "asset:///images/icon_category.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if(!subCatPickerPage)
                	subCatPickerPage = subcatPicker.createObject(this);
                subCatPickerPage.subcatXml = subCatXml;
                focusedItem = listCats;
                listTopic = listCats;
                nav.push(subCatPickerPage);
                // pickSubCat.show();
            }
            shortcuts: [
                Shortcut {
                    key: qsTr("s")
                }
            ]
        },
        ActionItem {
            title: qsTr("New topic")
            imageSource: "asset:///images/icon_write.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                if(!createPage)
                    createPage = postMessage.createObject();
                createPage.subcatxml = subCatXml;
                createPage.mode = 5;
                createPage.update = true;
                nav.push(createPage);
            }
        },
        ActionItem {
            title: qsTr("First page")
            imageSource: "asset:///images/icon_prev_all.png"
            onTriggered: {
                exploreCategoryController.firstPage();
                activityIndicator.start();
            }
        },
        ActionItem {
            title: qsTr("Prev page")
            imageSource: "asset:///images/icon_prev.png"
            onTriggered: {
                exploreCategoryController.prevPage();
                activityIndicator.start();
            }
        },
        ActionItem {
            title: qsTr("Next page")
            imageSource: "asset:///images/icon_next.png"
            onTriggered: {
                exploreCategoryController.nextPage();
                activityIndicator.start();
            }            
        },
        ActionItem {
            title: qsTr("Refresh")
            imageSource: "asset:///images/icon_refresh.png"
            onTriggered: {
                exploreCategoryController.refresh();
            }
        }
        
    ]
    onCreationCompleted: {
        focusedItem = listCats;
        focusedItemDepth = 1;
    }
    onUrlPageChanged: {
        exploreCategoryController.setListView(listCats);
        exploreCategoryController.listTopics(urlPage);
        exploreCategoryController.loadSubCats(subCatXml);
        activityIndicator.start();
    }
    
    onFlagTypeChanged: {
        activityIndicator.start();
        exploreCategoryController.filterByFlag(flagType);
    }
    onSubCatIndexChanged: {
        exploreCategoryController.listSubCat(subCatIndex);
        segmentedTitle.setSelectedIndex(0);
        activityIndicator.start();
    }
}
