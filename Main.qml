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
	// onActiveFocusItemChanged: print(activeFocusItem)


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
		anchors.right: pageItemdel.left
		Grid{
			columns:1
			Text{
				id: pathText
				text: "Path"
			}
			Button{
				text:"Open"
				onClicked: {
					if(myListModel.rowCount()>0)
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
	Loader{
		sourceComponent: pageItemdel
		anchors.right: parent.right
		anchors.left: menuRect.right
		anchors.top: parent.top
		anchors.bottom: parent.bottom
	}
	Component{
		id:pageItemdel
		PageItem{ 
			id:pageItem
		}
}


}
