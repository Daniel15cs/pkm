import QtQuick
import QtQuick.Controls
import PKM_03_qml
// pragma ComponentBehavior: Bound

Item {
    id: root
    property var dbModel
    property alias proxyModel: pModel

    DatabaseSortProxyModel {
        id: pModel
        sourceModel: root.dbModel
    }

    ListView {
        anchors.fill: parent
        clip: true
        model: pModel

        delegate: ItemDelegate {
					required property var propertyValue
					required property var model
						id: del
            width: root.width
            text: (model.display ?? "") + " | " + (propertyValue || "")
            onClicked: root.dbModel.manager ? root.dbModel.manager.setCurrentPage(model.pageId) : console.log("No manager for note: " + model.pageId)
            
            background: Rectangle {
                color: del.highlighted ? "#444" : "#222"
                border.color: "#333"
            }
            contentItem: Text {
                text: parent.text
                color: "white"
                verticalAlignment: Text.AlignVCenter
                leftPadding: 10
            }
        }
    }
}
