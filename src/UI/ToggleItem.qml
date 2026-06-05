import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
Item {
	id: root
	required property var pageManager
	required property var model
	required property int index
	required property var listView
	required property var logicObj

	property var pageModel: root.model
	property var page: pageModel.p_pageData

	property int _width:500
	readonly property int headerHeight: 25
	width: _width
	height: headerHeight //+ (expanded ? panel.implicitHeight : 0)
	x: logicObj.level*24 ?? 0

	property alias _compText:compText
	// property string _text: logicobject?.p_text
	readonly property string blockType:"toggleBlock"


	onHeightChanged:{
		var item = root.listView.itemAtIndex(index);
		if(item !=null)
		item.height = root.height;
	} 

	function onCurrent(){
		compText.forceActiveFocus(Qt.TabFocusReason)
		// btn.highlighted = !btn.highlighted
	}

	Component.onCompleted:{
		// console.log("lo: "+ root.logicObj)
		// console.log("pM: "+ root.pageModel)
	}


	HoverHandler{ id:hover}
	Column {
		anchors.fill: parent
		spacing: 4
		Row{
			// spacing: 4
			Button {
				id:btn
				width: root.headerHeight
				height: root.headerHeight
				text: root.logicObj.p_expanded ? "v" : ">"
				onClicked:{
					root.listView.currentIndex = root.index
					root.onCurrent()
					root.listView.currentItem.height = root.height
					root.model.toggle(root.logicObj)
				} 
				background: Rectangle {
					opacity: enabled ? 1 : 0.3
					color:"black" 
				}
			}
			TextArea{ // text in header 
				id: compText
				height:root.headerHeight
				width:root.width-50
				verticalAlignment:TextArea.AlignVCenter
				persistentSelection: true
				text: root.logicObj.p_text
				placeholderText: compText.activeFocus==true ? "Enter text" :""
				implicitHeight: contentHeight
				wrapMode: TextArea.Wrap
				// focus:true

				background: Rectangle{
					color: "transparent"
					// anchors.fill: parent
					// Rectangle{ anchors.fill:parent; color: "Grey"}
				}
				onPressed:{
					root.listView.currentIndex = root.index
					if(compText.activeFocus==false)
					root.onCurrent()
				}

				onTextChanged:{
					root.logicObj.setText(text)
					root.listView.currentItem.height = root.height
				}

				Keys.onPressed:function(event){
					if(event.key ===Qt.Key_Return || event.key === Qt.Key_Enter){
						if(event.modifiers & Qt.ShiftModifier){
							event.accepted = false
						}
						else if(event.modifiers & Qt.ControlModifier){
							event.accepted = true
							root.model.toggle(root.logicObj)
						}
						else{
							event.accepted = true
							if(root.index==root.pageModel.rowCount()-1){
								root.pageModel.append(root.blockType)
								root.listView.incrementCurrentIndex()
							}
							else if(root.index<root.pageModel.rowCount()-1){
								root.pageModel.insert(root.blockType,root.index+1)
								root.listView.incrementCurrentIndex()
							}
						}
					}else if(compText.text ==="" && event.key===Qt.Key_Backspace){
						root.pageModel.removeRow(root.index)
						root.listView.decrementCurrentIndex()
					}
					else if(event.key===Qt.Key_Tab){ // change block level
						event.accepted = true
						//increase level
						console.log("indent")
						root.model.addIndentBlock(root.logicObj)
					}
					else if(event.key===Qt.Key_Backtab){
						//degrease level
						event.accepted = true
						console.log("rm indent")
						root.model.rmIndentBlock(root.logicObj)
					}
				}
			}

			Button{
				id:compBtn
				text: "x"
				height:25
				width:25
				visible:hover.hovered

				background: Rectangle {
					opacity: enabled ? 1 : 0.3
					color:"black" 
				}
				onClicked:{
					if (root.index>=0)
					root.pageModel.removeRow(root.index);
				}
			}
		}
	}//main column
}//root item
