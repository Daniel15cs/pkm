import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
pragma ComponentBehavior: Bound
Window {
	id:root
	width: 640
	height: 480
	visible: true
	title: "Test"
	color: "black"

	// FileModel{
	// id:fileModel
	// }
	UiModel{ 
		id: uiModel
		property string selectedFilePath
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
				
			// Text{
				// text: fileModel.test()
			// }
			Button{
				text:"Open"
				onClicked: dial.open()
			}
			Button{
				id: saveBtn
				text: "Save"
				onClicked:{
						if (uiModel.selectedFilePath!="") {
							// uiModel.writeFile(uiModel.selectedFilePath, textArea.text)
						}else{
							saveDial.fileMode = FileDialog.SaveFile
							saveDial.open()
						}
					}
			// Connections {
			// 	target: button
			// 	 function onClicked() {dial.open()}
			// }
			}
		}
	}
	Flickable{
		id: flickable
		anchors.right: parent.right
		anchors.left: menuRect.right
		anchors.top: parent.top
		anchors.bottom: parent.bottom

		// clip: true
    // contentWidth: textArea.width; contentHeight: textArea.height

			// MyListModel{ id:listModel 
			// }

		ListView{
			id:listGrid
			spacing:2
			anchors.fill:parent
			model: myListModel
			delegate: TextListItem{
				id:textDel
				model:myListModel
				_width: flickable.width
        required property int index
			}
			
			// Row{
			// 	required property string listdata
			// 	required property string andata
			// 	Label{
			// 		width:100
			// 		height:40
			// 		text: parent.listdata+":"+parent.andata
			// 		background: Rectangle {
			// 			opacity: enabled ? 1 : 0.3
			// 			color:"grey" 
			// 		}
			// 	}
			// 	Button{onClicked:{console.log(parent.listdata)}}
			// }
		}
		Label{
			text: "labelText"
			anchors.bottom:addBtn.bottom
		}
			Button{
				id:addBtn
				text:"+"
				width:parent.width
				anchors.bottom:parent.bottom
				onClicked:{
					myListModel.append()
				}
				background: Rectangle {
					// implicitWidth: 100
					// implicitHeight: 40
					opacity: enabled ? 1 : 0.3
					color: "grey"
				}
			}

	}
	FileDialog{
		id:dial
		fileMode: FileDialog.OpenFile	
		onAccepted: {
			// pathText.text = selectedFile
			uiModel.selectedFilePath = selectedFile
			// textArea.text = uiModel.readFile(selectedFile)
		}
	}
	FileDialog{
		id:saveDial
		fileMode: FileDialog.SaveFile	
		onAccepted: {
			uiModel.selectedFilePath = selectedFile
			// uiModel.writeFile(selectedFile,textArea.text)
		}
	}
}
