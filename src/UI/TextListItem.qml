import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
Item{
	id:baseItem
	property int _height: compText.contentHeight+6
	property int _width:100
	required property var model
	required property int index
	required property var listView
	property var logicobject: model.getLogic(baseItem.index)
	// property string _text: logicobject?.text_p==null ? logicobject.text_p : ""
	property string _text: logicobject?.text_p
	height: _height
	width:_width
	property Item _body: bodySlot
	property Item self: baseItem
	property int _textMargin: 25
	property alias _compText:compText
	property string blockType:"textBlock"
	onHeightChanged:{
		var item = baseItem.listView.itemAtIndex(index);
		if(item !=null)
		item.height = baseItem.height;
	} 
	// Component.onCompleted:{
	// 	baseItem.listView.currentItem.height = baseItem._height
	// }
	function onCurrent(){
			compText.forceActiveFocus(Qt.TabFocusReason)
				//TODO:
			baseItem.listView.currentItem.height = baseItem.height
	}

	HoverHandler{ id:hover}
	Row{
		spacing:4
		Item{
			id:bodySlot
			height:baseItem._height
			// width:25
		}
		TextArea{
			id: compText
			height:baseItem._height
			width:baseItem._width-baseItem._textMargin
			verticalAlignment:TextEdit.AlignVCenter
			persistentSelection: true
			text: baseItem._text
			placeholderText: compText.activeFocus==true ? "Enter text or / for commands" :""
			implicitHeight: contentHeight
			wrapMode: TextArea.Wrap
			// focus:true

			background: Item{
				// anchors.fill: parent
				// Rectangle{ anchors.fill:parent; color: "Grey"}
			}
			onPressed:{
					baseItem.listView.currentIndex = baseItem.index
					if(compText.activeFocus==false)
						baseItem.onCurrent()
			}

			onTextChanged:{
				baseItem.logicobject.setText(text)
				baseItem.listView.currentItem.height = baseItem._height==null ? 10 : baseItem._height
			}

			Keys.onPressed:function(event){
				if(event.key ===Qt.Key_Return || event.key === Qt.Key_Enter){
					if(event.modifiers & Qt.ShiftModifier){
						event.accepted = false
					} else{
						event.accepted = true
						if(baseItem.index==baseItem.model.rowCount()){
							baseItem.model.append(baseItem.blockType)
							baseItem.listView.incrementCurrentIndex()
						}
						else if(baseItem.index<baseItem.model.rowCount()){
							baseItem.model.insert(baseItem.blockType,baseItem.index+1)
							baseItem.listView.incrementCurrentIndex()
						}
					}
				}else if(compText.text ==="" && event.key===Qt.Key_Backspace){
					baseItem.model.removeRow(baseItem.index)
					baseItem.listView.decrementCurrentIndex()
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
				if (baseItem.index>=0)
					baseItem.model.removeRow(baseItem.index);
			}
		}
	}
}
