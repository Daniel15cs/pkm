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
	UiModel{ id: uiModel}

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
				text:"open"
				onClicked: dial.open()
			}
			// Button{
			// 	id: button
			// 	// id:testBtn
			// 	text: "Save"
			// 	// onClicked:dial.open()
			// // Connections {
			// // 	target: button
			// // 	 function onClicked() {dial.open()}
			// // }
			// }
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
		onAccepted: {
			pathText.text = selectedFile
			textArea.text = uiModel.readFile(selectedFile)
		}
	}
}
// Rectangle{
// 	id:rect
// 	color:"black"
//
//
// }
