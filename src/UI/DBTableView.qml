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

    HorizontalHeaderView {
        id: header
        syncView: tableView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        z: 2
        delegate: Rectangle {
            required property var display
            implicitHeight: 30
            color: "#222"
            border.color: "#444"
            Text {
                anchors.centerIn: parent
                text: display || ""
                color: "white"
                font.bold: true
            }
        }
    }

    Connections {
        target: root.dbModel
        function onSchemaChanged() { tableView.forceLayout() }
    }

    TableView {
        id: tableView
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: pModel
        
        columnWidthProvider: function(column) { 
            if (column === 0) return 200;
            let schema = root.dbModel.schema;
            let prop = schema[column-1];
            if (prop && !prop.visibleTable) return 0;
            return 150; 
        }

        delegate: Rectangle {
            id: del
            required property var display
            required property var model
            implicitWidth: 100
            implicitHeight: 25
            color: "#333"
            border.color: "#444"
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (model.column === 0) {
                        if (root.dbModel.manager) {
                            root.dbModel.manager.setCurrentPage(model.pageId)
                        }
                    } else {
                        let prop = root.dbModel.schema[model.column-1];
                        if (prop) {
                            if (prop.type === "creation_date") return;
                            editDialog.openEdit(model.pageId, prop.id, prop.name, prop.type, prop.values, del.display);
                        }
                    }
                }
            }
            
            Text {
                anchors.centerIn: parent
                text: del.display || ""
                color: "white"
                elide: Text.ElideRight
                width: parent.width - 10
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    Dialog {
        id: editDialog
        title: "Edit " + propName
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel
        
        property int pageId
        property int propId
        property string propName
        property string propType
        property var propValues
        
        Column {
            spacing: 10
            width: 200
            
            Loader {
                id: editLoader
                width: parent.width
                sourceComponent: {
                    if (editDialog.propType === "status" || editDialog.propType === "select") return comboComp;
                    return textComp;
                }
            }
        }
        
        Component {
            id: textComp
            TextField {
                id: textInput
                text: ""
                onAccepted: editDialog.accept()
                Component.onCompleted: text = editDialog.initialValue
            }
        }
        
        Component {
            id: comboComp
            ComboBox {
                id: comboInput
                model: editDialog.propValues
                currentIndex: Math.max(0, model.indexOf(editDialog.initialValue))
            }
        }
        
        property string initialValue
        function openEdit(id, pid, name, type, values, val) {
            pageId = id
            propId = pid
            propName = name
            propType = type
            propValues = values
            initialValue = val
            open()
        }
        
        onAccepted: {
            if (!editLoader.item) return;
            let newVal = editLoader.item.hasOwnProperty("currentText") ? editLoader.item.currentText : (editLoader.item.text || "");
            root.dbModel.setPropertyByPageId(pageId, propId, newVal);
        }
    }
}
