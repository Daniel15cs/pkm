import QtQuick
import QtQuick.Controls
import PKM_03_qml
pragma ComponentBehavior: Bound

Item {
    id: root
    required property var dbModel
    property alias proxyModel: pModel

    DatabaseSortProxyModel {
        id: pModel
        sourceModel: root.dbModel
    }

    TableView {
        id: tableView
        anchors.fill: parent
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: pModel
        
        columnWidthProvider: function(column) { return 150; }

				delegate: Rectangle {
					id: del
					required property var display
					required property var model
					implicitWidth: 100
					implicitHeight: 25
					color: "#333"
					border.color: "#444"
					Button{
						onClicked:{
							if(root.dbModel.manager){
								root.dbModel.manager.setCurrentPage(del.model.pageId)
							}else{
								console.log("No manager for note: " + del.model.pageId)
							}
						} 
						// background:{}
						anchors.fill:parent
					}
					Text {
						anchors.centerIn: parent
						text: del.display || ""
						color: "white"
					}
				}
			}
		}
