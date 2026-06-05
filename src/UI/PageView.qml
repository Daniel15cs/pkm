import QtQuick
import QtQuick.Controls
// import QtQuick.Dialogs
pragma ComponentBehavior: Bound
Item{
	id: root
	required property var pageManager
	required property var pageModel
	property var page: pageManager.p_currentPage
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
		id: flick
		anchors.fill:parent
		ListView{
			id:listGrid
			anchors.fill:parent
			model: root.pageModel
			spacing:2
			reuseItems: false
			implicitHeight: contentHeight

			onCurrentIndexChanged:{
				// console.log("currind: "+currentIndex)
				if(currentItem){
					currentItem._loader.item.onCurrent()
				}
			}

			Keys.onUpPressed: decrementCurrentIndex()
			Keys.onDownPressed: incrementCurrentIndex()
			delegate: ListViewDelegate{
				width: flick.width
				flickable: flick
				pageManager: root.pageManager
				pageModel: root.pageModel
				listView: listGrid

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
					text: root.page.p_data.id
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
				Behavior on visible{
					NumberAnimation{
						duration: 100
					}
				}
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
						Action{ text :"Bullet"
							onTriggered: {
								listGrid.model.append("bulletBlock")
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
						Action{ text:"Toggle"
							onTriggered:{
								listGrid.model.append("toggleBlock")
								listGrid.currentIndex = listGrid.model.rowCount()-1
							}
						}
					}
				}
			}
				// } //footer
		}
	}
}
