import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
// import myPageBlock
pragma ComponentBehavior: Bound
ApplicationWindow {
	id:root
	width: 640
	height: 480
	visible: true
	title: "My_own_PKM"
	color: "black"
	// onActiveFocusItemChanged: print(activeFocusItem)

	// TODO: move this to pagesManager
	// FileModel{
	// id:fileModel
	// 	property string selectedFilePath
	// }

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

			// controller.p_rootPage.pageModel.parseJson(fileModel.readFile(selectedFile))
		}
	}
	FileDialog{
		id:saveDial
		fileMode: FileDialog.SaveFile	
		onAccepted: {
			// TODO: move this to pagesManager
			// fileModel.selectedFilePath = selectedFile
			// fileModel.writeFile(selectedFile, controller.p_rootPage.pageModel.listToJson());
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
					//TODO:
					if(controller.p_rootPage.pageModel.rowCount()>0)
						clearConfirm.open()
					else dial.open()
				}
			}
			Button{
				id: saveBtn
				text: "Save"
				onClicked:{
					// TODO: change way how it saving 
					// if (fileModel.selectedFilePath!="") {
					// 	fileModel.writeFile(fileModel.selectedFilePath, controller.p_rootPage.pageModel.listToJson());
					// }else{
					// 	saveDial.fileMode = FileDialog.SaveFile
					// 	saveDial.open()
					// }
					
					// something like this:
					// p_pageManager.save()
				}
			}
			Button{
				text:"Go back"
				onPressed:{
					// pageLoader.sourceComponent = pageItemdel
					// TODO:  change controller to manager
					// controller.getToOldPage()

					// p_pageManager.getToLastPage()
				}
			}
		}
	}
	Loader{
		id:pageLoader
		sourceComponent: pageItemdel
		anchors.right: parent.right
		anchors.left: menuRect.right
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		onLoaded:{
			// pageLoader.item.ctrl = controller
			// pageLoader.item.pageModel = controller.p_rootPage
			var currPage = p_pageManager.getCurrentPage();
			console.log(typeof currPage)
			console.log(currPage)
			console.log(currPage.data)
			console.log("id:" +currPage.data.id)
		}
	}
	Component{
		id:pageItemdel
		PageView{ 
			id:pageItem
			//TODO: change way to setting up page (from manager)
			// pageModel: controller.p_rootPage
			// ctrl:controller
			pageModel: p_pageManager.getCurrentPage().model
			// Component.onCompleted:{
			// 	pageItem.pageModel = rootPage
			// }
		}
	}


}
