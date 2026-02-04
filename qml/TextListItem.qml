import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
Item{
	id:baseItem
	property int _height: 30
	property int _width:100

	// required property string text
	required property var logicobject
	required property string typename
	required property var model
	// required property var listView

	property var _logic: logicobject
	property string _text: logicobject.text_p
	height:_height
	width:_width
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
				// console.log("_text: "+text+" logic:"+baseItem._logic.text_p)
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
				// baseItem._logic.deleteObj()
				if (index>=0){
					baseItem.model.removeRow(index);
				}
			}
		}
	}
}
