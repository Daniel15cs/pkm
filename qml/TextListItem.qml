import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
Item{
	id:baseItem
	property int _height: compText.contentHeight+8
	property int _width:100
	required property var logicobject
	required property var model
	property string _text: logicobject.text_p

	height:_height
	width:_width
	HoverHandler{ id:hover}
	Row{
		spacing:4
		TextArea{
			id: compText
			height:baseItem._height
			width:baseItem._width-25
			onTextChanged:{
				baseItem.logicobject.textChanged(text)
				if(baseItem._height<contentHeight)
					baseItem._height=contentHeight+8
				else if(baseItem._height>contentHeight+8)
					baseItem._height=contentHeight+8
			}
			verticalAlignment:TextEdit.AlignVCenter	
			text: baseItem._text
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
				if (index>=0){
					baseItem.model.removeRow(index);
				}
			}
		}
	}
}
