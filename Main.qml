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
		onAccepted:{p_pageManager.uploadList()}
	}
	// FileDialog{
	// 	id:dial
	// 	fileMode: FileDialog.OpenFile	
	// 	onAccepted: {
	// 		pathText.text = selectedFile
	// 		fileModel.selectedFilePath = selectedFile
	//
	// 		// controller.p_rootPage.pageModel.parseJson(fileModel.readFile(selectedFile))
	// 	}
	// }
	// FileDialog{
	// 	id:saveDial
	// 	fileMode: FileDialog.SaveFile	
	// 	onAccepted: {
	// 		// TODO: move this to pagesManager
	// 		// fileModel.selectedFilePath = selectedFile
	// 		// fileModel.writeFile(selectedFile, controller.p_rootPage.pageModel.listToJson());
	// 	}
	// }
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
				text:"Upload"
				onClicked: {
					//TODO:
					if(p_pageManager.getCurrentPage().model.rowCount()>0)
						clearConfirm.open()
					else p_pageManager.uploadList()
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
					p_pageManager.savePagesList()
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
			var model = currPage.model;
			// console.log("curpage:" +currPage)
			// console.log("pdata: " +currPage.p_data)
			// console.log("id:" +currPage.p_data.id)
			// console.log("model: " +model);
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
			page: p_pageManager.getCurrentPage()
			// Component.onCompleted:{
			// 	pageItem.pageModel = rootPage
			// }
		}
	}


}
