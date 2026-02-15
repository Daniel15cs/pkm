import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
Item{
	id:root
	height: _height
	width:_width
	property int _height: 20
	property int _width:  100
	required property var model
	required property int index
	// required property var listView
	// required property var pageLoader
	required property var controller
	property var pageModel: model.getLogic(root.index)
	function onCurrent(){
	}
	Component.onCompleted:{
		console.log("\nlistItem ctrl:"+controller)
		console.log("listItem rootpage:" +root.controller.p_rootPage)
		console.log("listItem logic: "+pageModel)
		console.log("listItem model: "+root.model+"\n")
	}
	Row{
		Button{
			// text: "PageBtn"
			text: root.pageModel.pageName
			height: root._height
			width:root._width
			onPressed:{
				console.log(root.pageModel)
				console.log("listItem rootpageold:" +root.controller.p_rootPage)
				root.controller.setRootPage(root.pageModel)
				console.log("listItem rootpage:" +root.controller.p_rootPage)
			}
			background:Rectangle{
				color:"Black"
			}
		}
	}

}
