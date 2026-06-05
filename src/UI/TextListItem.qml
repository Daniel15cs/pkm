import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
Item{
	id:root
	required property int index
	required property var listView
	property var model: listView.model
	required property var logicObj

	// property string _text: logicobject?.text_p==null ? logicobject.text_p : ""
	property string _text:  logicObj.text_p ?? ""

	property int _height: compText.contentHeight+6
	property int _width:100
	height: _height
	property int leftMargin: logicObj.level*24 ?? 0
	width:_width - 24 - leftMargin
	x: leftMargin

	property Item _body: bodySlot
	property Item self: root
	property int _textMargin: 25
	property alias _compText:compText
	property string blockType:"textBlock"

	onHeightChanged:{
		var item = root.listView.itemAtIndex(index);
		if(item !=null)
			item.height = root.height;
	} 
	Component.onCompleted:{
		// baseItem.listView.currentItem.height = baseItem._height
		console.log("tli: logicobject: "+root.logicObj)
		// console.log("tli: currItem: "+baseItem.listView.currentItem)
	}
	function onCurrent(){
			compText.forceActiveFocus(Qt.TabFocusReason)
				//TODO:
			root.listView.currentItem.height = root.height
	}

	Row{
		HoverHandler{ id:hover}
		spacing:4
		Item{
			id:bodySlot
			height:root._height
			// width:25
		}
		TextArea{
			id: compText
			height:root._height
			width:root._width-root._textMargin
			verticalAlignment:TextArea.AlignVCenter
			persistentSelection: true
			text: root._text
			placeholderText: compText.activeFocus==true ? "Enter text or / for commands" :""
			implicitHeight: contentHeight
			wrapMode: TextArea.Wrap
			// focus:true

			background: Item{
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
				var currItem = root.listView.currentItem
				if(currItem)
					currItem.height = root._height
			}

			Keys.onPressed:function(event){
				if(event.key ===Qt.Key_Return || event.key === Qt.Key_Enter){
					if(event.modifiers & Qt.ShiftModifier){
						event.accepted = false
					} else{
						event.accepted = true
						let type = root.blockType
						if(compText.text==="/toggle") type ="toggleBlock"
						else if(compText.text==="/checkbox") type ="checkboxBlock"
						else if(compText.text==="/bullet") type ="bulletBlock"
						if(root.index==root.model.rowCount()){
							root.model.append(type)
							root.listView.incrementCurrentIndex()
						}
						else if(root.index<root.model.rowCount()){
							root.model.insert(type,root.index+1)
							root.listView.incrementCurrentIndex()
						}
					}
				}else if(compText.text ==="" && event.key===Qt.Key_Backspace){
					root.model.removeRow(root.index)
					root.listView.decrementCurrentIndex()
				}else if(event.key===Qt.Key_Tab){ // change block level
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
			HoverHandler{ id:btnHover}
			id:compBtn
			text: "x"
			height:25
			width:25
			visible: hover.hovered | btnHover.hovered
			// visible: true

			background: Rectangle {
				radius: 8
				opacity: enabled ? 1 : 0.3
				color:"#061E29" 
			}
			onClicked:{
				if (root.index>=0)
					root.model.removeRow(root.index);
			}
		}
	}
}
