import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
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
					required property var model
						id: del
            width: root.width
            
            contentItem: ColumnLayout {
                spacing: 2
                Text {
                    text: model.display ?? ""
                    color: "white"
                    font.bold: true
                    Layout.fillWidth: true
                }
                Flow {
                    Layout.fillWidth: true
                    spacing: 10
                    Repeater {
                        model: root.dbModel.schema
                        delegate: Text {
                            required property var modelData
                            visible: modelData.visibleList
                            text: {
                                let val = root.dbModel.getPropertyByPageId(model.pageId, modelData.id);
                                if (val === undefined || val === null || val === "") return "";
                                return modelData.name + ": " + (Array.isArray(val) ? val.join(", ") : val);
                            }
                            color: "#aaa"
                            font.pixelSize: 11
                        }
                    }
                }
            }

            onClicked: root.dbModel.manager ? root.dbModel.manager.setCurrentPage(model.pageId) : console.log("No manager for note: " + model.pageId)
            
            background: Rectangle {
                color: del.highlighted ? "#444" : "#222"
                border.color: "#333"
            }
        }
    }
}
