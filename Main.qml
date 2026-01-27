import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
pragma ComponentBehavior: Bound
// 
// import my 1.0;
//
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
							uiModel.writeFile(uiModel.selectedFilePath, textArea.text)
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

			ListModel{ id:listModel }
			Component{
					id:textDel
				TextListItem{
					model:listModel
				}
			}

		ListView{
			id:listGrid
			spacing:2
			anchors.fill:parent
			model: listModel
			delegate: textDel
			Button{
				text:"+"
				width:parent.width
				anchors.bottom:parent.bottom
				onClicked:{
					listModel.append({_text:1,index:listModel.count-1})
					// console.log("element is added by btn")
				}
				background: Rectangle {
					// implicitWidth: 100
					// implicitHeight: 40
					opacity: enabled ? 1 : 0.3
					color: "black"
				}
			}
		}
			// TextArea.flickable: TextArea{
			// 	id: textArea
			// 	focus: true
			// 	anchors.right: parent.right
			// 	anchors.left:parent.left
			// 	// anchors.top:parent.top
			// 	height: 20
			//
			// 	// anchors.left: rect.right
			// 	// anchors.top: parent.top
			// 	// anchors.bottom: parent.bottom
			// 	// anchors.right: parent.right
			// 	placeholderText: "Enter text"
			// 	// textFormat: TextEdit.MarkdownText
			// 	renderType:Text.CurveRendering
			// 	// wrapMode: Text.Wrap
			// }
	}
	FileDialog{
		id:dial
		fileMode: FileDialog.OpenFile	
		onAccepted: {
			// pathText.text = selectedFile
			uiModel.selectedFilePath = selectedFile
			textArea.text = uiModel.readFile(selectedFile)
		}
	}
	FileDialog{
		id:saveDial
		fileMode: FileDialog.SaveFile	
		onAccepted: {
			uiModel.selectedFilePath = selectedFile
			uiModel.writeFile(selectedFile,textArea.text)
		}
	}
}
// Rectangle{
// 	id:rect
// 	color:"black"
//
//
// }
