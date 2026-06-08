import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
pragma ComponentBehavior: Bound
Item{
	id:root
	height: _height
	width:_width
	x: logicObj.level*24 ?? 0
	property int _height: 25
	property int _width:  120
	required property var pageManager
	required property var model
	required property int index
	required property var logicObj
	
	function onCurrent(){
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

	RowLayout {
		anchors.fill: parent
		spacing: 2

		Button{
			text: "id:"+root.logicObj.p_pageId
			Layout.fillWidth: true
			Layout.fillHeight: true
			onPressed:{
				console.log("setcurrpage pressed: "+root.logicObj.p_pageId)
				root.pageManager.setCurrentPage(root.logicObj.p_pageId)
			}
			background:Rectangle{
				color:"#222"
				border.color: "gray"
				radius: 3
			}
		}

		Button{
			id:compBtn
			text: "x"
			Layout.preferredWidth: 25
			Layout.fillHeight: true
			background: Rectangle {
				opacity: enabled ? 1 : 0.3
				color:"#441111" 
				radius: 3
			}
			onClicked:{
				if (root.index>=0)
					clearConfirm.open()
			}
		}
	}
}
