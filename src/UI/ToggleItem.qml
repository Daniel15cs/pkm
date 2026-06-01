import QtQuick
import QtQuick.Controls
Item {
	id: root
	required property var model
	required property int index
	required property var listView
	property int _width:500

	property bool expanded: true

	readonly property int headerHeight: 25

	width: _width
	height: headerHeight + (expanded ? panel.implicitHeight : 0)
	onHeightChanged: root.listView.itemAtIndex(root.index).height = root.height

	function onCurrent(){
		tta.forceActiveFocus(Qt.TabFocusReason)
		btn.highlighted = !btn.highlighted
	}
	Column {
		anchors.fill: parent
		spacing: 4

		Button {
			id:btn
			width: root.headerHeight
			height: root.headerHeight
			text: root.expanded ? "v" : ">"
			onClicked:{
				root.listView.currentIndex = root.index
				root.onCurrent()
				root.listView.currentItem.height = root.height
				root.expanded = !root.expanded
			} 
		}

		Rectangle {
			id: panel
			width: parent.width
			color: "blue"
			clip: true

			implicitHeight: content.implicitHeight + 10

			height: root.expanded ? implicitHeight : 0

			Behavior on height {
				NumberAnimation { duration: 50}
			}

			Item {
				id: content
				width: parent.width

				implicitHeight: column.implicitHeight

				Column {
					id: column
					width: parent.width
					spacing: 4

					TextArea {
						id:tta
						width: parent.width
						text: "test 1"
						wrapMode: TextEdit.Wrap
					}

					TextArea {
						width: parent.width
						text: "test 2"
						wrapMode: TextEdit.Wrap
					}
				}
			}
		}
	}
}
// Item{
// 	// property int _height: 100
// 	property bool expanded:true
// 	// width: 
// 	// height: root.expanded? content.implicitHeight +40: 40
// 	// onHeightChanged: listView.view.height = height
// 	readonly property int collapsedHeight:40
// 	height: expanded ? (panel.implicitHeight+ collapsedHeight) : collapsedHeight
// 	Column{
// 		// height: content.height
// 		anchors.fill:parent
// 		spacing:4
//
// 		// TextListItem{
// 		// 	model:root.model
// 		// 	index:root.index
// 		// 	listView:root.listView
// 		// 	_textMargin:100
// 		// 	blockType:"toggle"
// 		// 	_body.width:20
// 		//
// 		// 	// _body.data: 
// 		// }
//
// 		Button{
// 			width:30
// 			height: root.collapsedHeight
// 			text:">>"
// 			onClicked:{
// 				root.expanded = !root.expanded
// 			}
// 		}
// 		Rectangle{
// 			id: panel
// 			clip:true
// 			color: "blue"
// 			width:parent.width
// 			// height:parent.height-40
//
// 			// width: 100
// 			implicitHeight: content.implicitHeight+10
// 			height: root.expanded ? implicitHeight : 0
// 			Behavior on height{
// 				NumberAnimation{
// 					duration: 70
// 				}
// 			}
// 			Item{
// 				id:content
// 				// anchors.fill: paren
// 				width: root.width
// 				implicitHeight: col.implicitHeight
// 				Column{
// 					id: col
// 					spacing:4
// 					TextArea{
// 						id:tta
// 						text:"test"
// 						wrapMode:TextEdit.Wrap
// 						width: 50
// 						height:50
// 						implicitHeight: contentHeight
// 						// onHeightChanged: root.height=contentHeight+50
// 					}
// 					TextArea{
// 						// id:tta
// 						text:"t2est"
// 						wrapMode:TextEdit.Wrap
// 						width: 50
// 						height:50
// 						implicitHeight: contentHeight
// 						// onHeightChanged: root.height=contentHeight+50
// 					}
// 				}
// 			}
// 		}
// 	}
// }
