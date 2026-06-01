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

	MessageDialog{
		id:clearConfirm
		title: "Confirm"
		informativeText:"Are you sure to clean current page?"
		buttons: MessageDialog.Ok | MessageDialog.Cancel
		onAccepted:{p_pageManager.uploadList()}
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
				text:"Upload"
				onClicked: {
					if(p_pageManager.p_currentPage.model.rowCount()>0)
						clearConfirm.open()
					else p_pageManager.uploadList()
				}
			}
			Button{
				id: saveBtn
				text: "Save"
				onClicked:{
					p_pageManager.savePagesList()
				}
			}
			Button{
				text:"Go back"
				onPressed:{
					p_pageManager.getToLastPage()
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
			var currPage = p_pageManager.p_currentPage;
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
			pageManager: p_pageManager
			pageModel: p_pageManager.p_currentPage.model
		}
	}


}
