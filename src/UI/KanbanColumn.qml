import QtQuick
import QtQuick.Controls
import PKM_03_qml
pragma ComponentBehavior: Bound

Column {
	id: root
	property string title
	property string status
	property int _width: 100
	required property var dbModel
	Rectangle{
		// anchors.fill: parent
		color: "grey"
	}

	Text {
		text: root.title
		font.bold: true
		color: "white"
		padding: 5
	}

	ListView {
		id: cardList
		width: root._width
		height: parent.height - 30
		// clip: true
		model: DatabaseSortProxyModel {
			id: columnProxy
			sourceModel: root.dbModel
			filterStatus: root.status

			Component.onCompleted: {
				let pid = root.dbModel.getPropertyIdByName("status")
				if (pid !== -1) setFilterPropertyId(pid)
				else setFilterPropertyId(1) // Fallback to 1
			}
		}

		delegate: ItemDelegate {
			required property var pageId
			required property var model
			id: cardDelegate
			width: root._width
			height: 50
			text: model.display || ""

			// Drag and Drop
			// Drag.active: dragArea.drag.active
			// Drag.active: dragArea.drag.active && dragArea.pressed
			Drag.hotSpot.x: root._width / 2
			Drag.hotSpot.y: height / 2
			Drag.mimeData: { 
				"text/plain": pageId.toString(), 
				"pageId": pageId.toString(),
				"sourceStatus":root.status
			}
			Drag.dragType: Drag.Automatic

			onClicked: root.dbModel.manager ? root.dbModel.manager.setCurrentPage(model.pageId) : console.log("No manager for note: " + model.pageId)

			background: Rectangle {
				color: dragArea.drag.active ? "#3a3a3a" : "#2a2a2a"
				radius: 5
				border.color: dragArea.drag.active ? "cyan" : "#444"
				border.width: dragArea.drag.active ? 2 : 1
				opacity: dragArea.drag.active ? 0.7 : 1.0
			}

			contentItem: Text {
				text: cardDelegate.text
				color: "white"
				elide: Text.ElideRight
				verticalAlignment: Text.AlignVCenter
				leftPadding: 10
			}

			MouseArea {
				id: dragArea
				anchors.fill: parent
				drag.target: cardDelegate

				// drag.minimumX: cardDelegate.parent.mapFromItem(null,0,0).x
				// drag.maximumX: cardDelegate.parent.mapFromItem(null,Window.window.width-cardDelegate.width,0).x
				// drag.minimumY: cardDelegate.parent.mapFromItem(null,0,0).y
				// drag.maximumY: cardDelegate.parent.mapFromItem(null,Window.window.width-cardDelegate.width,0).y

				drag.onActiveChanged: cardDelegate.Drag.active = drag.active

				onReleased: {
					if (cardDelegate.Drag.target) {
						// Drop was handled by DropArea
					}
					cardDelegate.x = 0
					cardDelegate.y = 0
				}
			}
		}

		DropArea {
			anchors.fill: parent
			onEntered:(drag)=>{
				if(drag.getDataAsString("sourceStatus")===root.status){
					drag.accepted=false
				}
			}
			onDropped: (drop) => {
				let pageIdStr = drop.getDataAsString("pageId")
				if (!pageIdStr) return

				let pageId = parseInt(pageIdStr)
				let pid = root.dbModel.getPropertyIdByName("status")
				if (pid !== -1) {
					for (let i = 0; i < root.dbModel.rowCount(); ++i) {
						// Use Role 257 for PageIdRole
						if (root.dbModel.data(root.dbModel.index(i, 0), 257) === pageId) {
							root.dbModel.setProperty(i, pid, root.status)
							// root.dbModel.refresh() // Force refresh to update internal vector
							break
						}
					}
				}
			}
		}
	}
}
