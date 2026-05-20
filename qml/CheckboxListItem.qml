import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
TextListItem{
	_textMargin:55
	blockType:"checkboxBlock"
	_body.width:25

	_body.data: CheckBox{
		width:25
		height: self._height
		tristate:true
		checkState: self.logicobject?.checkState_p
		onClicked:{
			self._compText.font.strikeout = checkState==Qt.Checked
			self.logicobject.stateChanged(checkState)
		}
	}
}
