import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
TextListItem{
	_textMargin:55
	blockType:"bulletBlock"
	_body.width:25
	x: logicObj.level*24 ?? 0

	_body.data: Rectangle{
		width: 10
		height: 10
		radius: 10
		color: "white"
		anchors.centerIn: parent
	}
}
