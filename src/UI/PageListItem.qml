import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
pragma ComponentBehavior: Bound
Item{
	id:root
	height: _height
	width:_width
	property int _height: 20
	property int _width:  100
	required property var pageManager
	required property var model
	required property int index
	// required property var listView
	// required property var pageLoader
	
	property var blockLogic: model.getLogic(root.index)
	function onCurrent(){
	}
	Component.onCompleted:{
		// console.log("\nlistItem ctrl:"+controller)
		// console.log("listItem rootpage:" +root.controller.p_rootPage)
		// console.log("listItem logic: "+pageModel)
		// console.log("listItem model: "+root.model+"\n")
		// console.log(root.model)
		// console.log(root.model.p_pageData)
	}
	MessageDialog{
		id:clearConfirm
		title: "Confirm"
		informativeText:"Are you sure to remove selected page?"
		buttons: MessageDialog.Ok | MessageDialog.Cancel
		onAccepted:{
			root.model.removeRow(root.index);
		} 
	}
	Row{
		Button{
			spacing: 4
			// text: "PageBtn"
			text: "id:"+root.blockLogic.p_pageId
			// text:"test"
			height: root._height
			width:root._width-25
			onPressed:{
				// console.log(root.model)
				// console.log("listItem rootpageold:" +root.controller.p_rootPage)
				// console.log("pageListItem page id: " +root.model.p_pageData.id)
				// console.log("pageListItem pageData: " +root.model.p_pageData)
				//TODO: page manager is undefined
				root.pageManager.setCurrentPage(root.blockLogic.p_pageId)
				//TODO:
				// root.controller.setRootPage(root.pageModel)
			}
			background:Rectangle{
				color:"Black"
			}
		Button{
			id:compBtn
			text: "x"
			height:root._height
			width:25
			// visible:root.hovered

			background: Rectangle {
				opacity: enabled ? 1 : 0.3
				color:"black" 
			}
			onClicked:{
				if (root.index>=0)
					clearConfirm.open()
			}
		}
		}
	}

}
