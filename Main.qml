import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
pragma ComponentBehavior: Bound
ApplicationWindow {
	id:root
	width: 640
	height: 480
	visible: true
	title: "My_own_PKM"
	color: "black"


	FileModel{
	id:fileModel
		property string selectedFilePath
	}
	MessageDialog{
		id:clearConfirm
		title: "Confirm"
		informativeText:"Are you sure to clean current page?"
		buttons: MessageDialog.Ok | MessageDialog.Cancel
		onAccepted:{ dial.open() }
	}
	FileDialog{
		id:dial
		fileMode: FileDialog.OpenFile	
		onAccepted: {
			pathText.text = selectedFile
			fileModel.selectedFilePath = selectedFile

			myListModel.parseJson(fileModel.readFile(selectedFile))
		}
	}
	FileDialog{
		id:saveDial
		fileMode: FileDialog.SaveFile	
		onAccepted: {
			fileModel.selectedFilePath = selectedFile
			fileModel.writeFile(selectedFile, myListModel.listToJson());
		}
	}
	Rectangle{
		id: menuRect
		color:"grey"
		width: root.width/4
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: flickable.left
		Grid{
			columns:1
			Text{
				id: pathText
				text: "Path"
			}
			Button{
				text:"Open"
				onClicked: {
					if(listGrid.model.rowCount()>0)
						clearConfirm.open()
					else dial.open()
				}
			}
			Button{
				id: saveBtn
				text: "Save"
				onClicked:{
						if (fileModel.selectedFilePath!="") {
							fileModel.writeFile(fileModel.selectedFilePath, myListModel.listToJson());
						}else{
							saveDial.fileMode = FileDialog.SaveFile
							saveDial.open()
						}
					}
			}
		}
	}
	Flickable{
		id: flickable
		anchors.right: parent.right
		anchors.left: menuRect.right
		anchors.top: parent.top
		anchors.bottom: parent.bottom
	
		ListView{
			id:listGrid
			anchors.fill:parent
			model: myListModel

			onCurrentIndexChanged:{
				console.log("currind: "+currentIndex)
				currentItem._loader.item.onCurrent()
			}

			Keys.onUpPressed: decrementCurrentIndex()
			Keys.onDownPressed: incrementCurrentIndex()
			delegate: Item{
				id:delItem
				width: flickable.width
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
					text:  "New page"
					placeholderText: "Unnamed"
					width:parent.width
					font.pixelSize: 22
					font.bold: true
					horizontalAlignment: Text.AlignHCenter
					background: Item{}

				}
			}
			footer: Item{ 
				width:parent.width
				// anchors.top:parent.bottom
				height:200
				// Rectangle{
				// 	anchors.fill:parent
				// 	color:"Grey"
				// }

				Button{
					HoverHandler{ id:btnHover}
					id:addBtn
					text:"+"
					// width:parent.width
					// anchors. bottom:parent.bottom
					anchors.fill:parent
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
			}
		}
	}
}
