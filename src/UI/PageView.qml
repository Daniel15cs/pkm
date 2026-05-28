import QtQuick
import QtQuick.Controls
// import QtQuick.Dialogs
pragma ComponentBehavior: Bound
Item{
	id: root
	required property var pageModel
	required property var page
	required property var pageManager
	// property var ctrl
	//WARNING: pageModel is null after  setRootpage	
	
	// property var pageModel: ctrl.p_rootPage
	// property var listModel: ctrl.p_rootPage.pageModel
	// required property var loader
	Component.onCompleted:{
		// console.log("pageView ctrl:"+ctrl)
		// console.log("pageView pagemodel:"+pageModel)
		// console.log("pageView rootpage:" +root.ctrl.p_rootPage)
		// console.log("pageView pageModel:" +root.ctrl.p_rootPage.pageModel)
	}
	Flickable{
		id: flickable
		anchors.fill:parent
	
		ListView{
			id:listGrid
			anchors.fill:parent
			model: root.pageModel
			spacing:2

			onCurrentIndexChanged:{
				// console.log("currind: "+currentIndex)
				if(currentItem){
					currentItem._loader.item.onCurrent()
				}
			}

			Keys.onUpPressed: decrementCurrentIndex()
			Keys.onDownPressed: incrementCurrentIndex()
			delegate: Item{
				id:delItem
				width: flickable.width
				height:24
				required property int index
				property alias _loader:loader

				Loader{ id:loader 
					Component.onCompleted:{
						var _model = listGrid.model
						var type = _model.getLogic(delItem.index).typeName()
						if(type === "textBlock"){
							loader.sourceComponent = textDel
						} else if(type === "checkboxBlock"){
							loader.sourceComponent = checkboxDel
						} else if(type === "pageBlock"){
							loader.sourceComponent = pageListItemDel
						} else{
							loader.sourceComponent = textDel
						}
					}
				}

				Component{
					id:textDel
					TextListItem{
						model:listGrid.model
						_width: flickable.width
						index: delItem.index
						listView: listGrid
					}
				}
				Component{
					id:checkboxDel
					CheckboxListItem{
						model:listGrid.model
						_width: flickable.width
						index: delItem.index
						listView: listGrid
					}
				}
				Component{
					id:pageListItemDel
					PageListItem{
						model:listGrid.model
						_width: flickable.width
						index: delItem.index
						pageManager: root.pageManager

						
						// listView: listGrid
						// pageLoader: root.loader
					}
				}
			}

			HoverHandler{ 
				id:hover
			}
			header:Item{
				width:parent.width
				height: 100
				TextField{
					id: pageTitle
					// TODO:
					text: page.p_data.id
					placeholderText: "Unnamed"
					width:parent.width
					font.pixelSize: 22
					font.bold: true
					horizontalAlignment: Text.AlignHCenter
					background: Item{}
					onEditingFinished:{
						// TODO:
						// root.pageModel.setPageName(text)
					}

				}
			}
			// footer: Item{ 
				// width:parent.width
				// anchors.top:parent.bottom
				// height:50
				// Rectangle{
				// 	anchors.fill:parent
				// 	color:"Grey"
				// }

				Button{
					HoverHandler{ id:btnHover}
					id:addBtn
					text:"+"
					anchors.topMargin:50
					// anchors.fill:parent
					anchors.bottom:parent.bottom
					width:parent.width

					visible:hover.hovered | btnHover.hovered
					onClicked:{
						itemMenu.popup()
					}
					background: Rectangle {
						opacity: enabled ? 1 : 0.3
						color: "black"
					}
					Menu{
						id:itemMenu
						Menu{
							title: "Add block"
							Action{ text :"Text"
								onTriggered:{
									listGrid.model.append("textBlock")
									listGrid.currentIndex = listGrid.model.rowCount()-1
								} 
							}
							Action{ text :"Checkbox"
								onTriggered: {
									listGrid.model.append("checkboxBlock")
									listGrid.currentIndex = listGrid.model.rowCount()-1
								}
							}
							Action{ text :"Page"
								onTriggered:{
									// TODO: proper page append AND block to this pageModel
									// root.pageManager.appendPageToList(root.pageModel.getPageData.id)
									listGrid.model.append("pageBlock")
									listGrid.currentIndex = listGrid.model.rowCount()-1
								}
							}
						}
					}
				}
			// }
		}
	}
}
