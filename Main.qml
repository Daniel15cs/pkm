import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import my 1.0;
// import PKM_03_qml;
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

			TextArea.flickable: TextArea{
				id: textArea
				focus: true
				anchors.fill: parent

				// anchors.left: rect.right
				// anchors.top: parent.top
				// anchors.bottom: parent.bottom
				// anchors.right: parent.right
				placeholderText: "Enter text"
				// textFormat: TextEdit.MarkdownText
				renderType:Text.CurveRendering
				// wrapMode: Text.Wrap
			}
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
