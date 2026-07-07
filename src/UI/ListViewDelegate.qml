import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
Item{
	id:root
	required property var pageManager
	required property var pageModel
	required property var listView
	required property var logicobject
	property var page: pageManager.p_currentPage

	//TODO:
	implicitHeight: loader.item ? loader.item.height : 24
	height:24
	required property int index
	required property var flickable
	property alias _loader:loader

	Loader{ id:loader 
	Component.onCompleted:{
		var _model = root.listView.model
		var type = root.logicobject.typeName()
		if(type === "textBlock"){
			loader.sourceComponent = textDel
		} else if(type === "checkboxBlock"){
			loader.sourceComponent = checkboxDel
		} else if(type === "bulletBlock"){
			loader.sourceComponent = bulletDel
		} else if(type === "pageBlock"){
			loader.sourceComponent = pageListItemDel
		}else if(type==="toggleBlock"){
			loader.sourceComponent = toggleDel
		}
		else{
			loader.sourceComponent = textDel
		}
		root.listView.forceLayout()
	}
}

Component{
	id:textDel
	TextListItem{
		model:root.listView.model
		_width: root.flickable.width
		index: root.index
		listView: root.listView
		logicObj: root.logicobject
	}
}
Component{
	id:checkboxDel
	CheckboxListItem{
		model:root.listView.model
		_width: root.flickable.width
		index: root.index
		listView: root.listView
		logicObj: root.logicobject
	}
}
Component{
	id:bulletDel
	BulletListItem{
		model:root.listView.model
		_width: root.flickable.width
		index: root.index
		listView: root.listView
		logicObj: root.logicobject
	}
}
Component{
	id:pageListItemDel
	PageListItem{
		model:root.listView.model
		_width: root.flickable.width
		index: root.index
		pageManager: root.pageManager
		logicObj: root.logicobject
	}
}
Component{
	id:toggleDel
	ToggleItem{
		model:root.listView.model
		_width: root.flickable.width
		index: root.index
		listView: root.listView
		pageManager: root.pageManager
		logicObj: root.logicobject
	}
}
	}
