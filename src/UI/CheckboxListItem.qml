import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
TextListItem{
	_textMargin:55
	blockType:"checkboxBlock"
	_body.width:25
	x: logicObj.level*24 ?? 0

	_body.data: CheckBox{
		id: control
		width:25
		height: self._height
		tristate:true
		checkState: self.logicObj?.checkState_p ?? Qt.Unchecked

		Component.onCompleted: {
			self._compText.font.strikeout = checkState === Qt.Checked
		}

		onClicked:{
			self._compText.font.strikeout = checkState === Qt.Checked
			self.logicObj.setState(checkState)
		}
	}
}
