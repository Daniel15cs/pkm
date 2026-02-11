import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
Item{
	id:baseItem
	height: _height
	width:_width
	property int _height: compText.contentHeight+8
	property int _width:100
	required property var model
	required property int index
	required property var listView
	property var logicobject: model.getLogic(baseItem.index)
	property string _text: logicobject?.text_p

	function onCurrent(){
		Qt.callLater(function(){
			compText.forceActiveFocus()
			baseItem.listView.currentItem.height = baseItem.height
		})
	}

	HoverHandler{ id:hover}
	Row{
		spacing:4
		TextArea{
			id: compText
			height:baseItem._height
			width:baseItem._width-25
			verticalAlignment:TextEdit.AlignVCenter
			persistentSelection: true
			text: baseItem._text
			placeholderText: compText.activeFocus==true ? "Enter text or / for commands" :""
			implicitHeight: contentHeight
			wrapMode: TextArea.Wrap

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
				baseItem.logicobject.textChanged(text)
				baseItem.listView.currentItem.height = baseItem._height
			}

			Keys.onPressed:function(event){
				if(event.key ===Qt.Key_Return || event.key === Qt.Key_Enter){
					if(event.modifiers & Qt.ShiftModifier){
						event.accepted = false
					} else{
						event.accepted = true
						baseItem.model.append("textBlock")
						baseItem.listView.incrementCurrentIndex()
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
			height:baseItem._height
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
