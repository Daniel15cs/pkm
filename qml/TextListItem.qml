import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound
Item{
	id:baseItem
	property int _height: 30
	property int _width:parent?.width
	required property var model
	property string _text
	property int index
	height:_height
	width:_width
	Row{
		spacing:4
		TextArea{
			id: compText
			height:baseItem._height
			width:baseItem._width-25

			onTextChanged:{
				console.log("on text change ")
				if(baseItem._height<contentHeight)
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
			background: Rectangle {
				// implicitWidth: 100
				// implicitHeight: 40
				opacity: enabled ? 1 : 0.3
				color:"black" 
			}
			onClicked:{
				console.log("textis: "+baseItem._text)
				baseItem.model?.remove(baseItem.index)
			}
		}
	}
}
