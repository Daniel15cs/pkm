import QtQuick
import QtQuick.Controls
// import QtQuick.Dialogs
pragma ComponentBehavior: Bound
Item{
	id: root
	Flickable{
		id: flickable
		anchors.fill:parent
	
		ListView{
			id:listGrid
			anchors.fill:parent
			model: myListModel
			spacing:2

			onCurrentIndexChanged:{
				console.log("currind: "+currentIndex)
				if(currentItem)
					currentItem._loader.item.onCurrent()
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
			}

			HoverHandler{ 
				id:hover
			}
			header:Item{
				width:parent.width
				height: 100
				TextField{
					id: pageTitle
					text:  "New page"
					placeholderText: "Unnamed"
					width:parent.width
					font.pixelSize: 22
					font.bold: true
					horizontalAlignment: Text.AlignHCenter
					background: Item{}

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
						}
					}
				}
			// }
		}
	}
}
