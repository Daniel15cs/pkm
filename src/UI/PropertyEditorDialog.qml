import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "Edit Values"
    standardButtons: Dialog.Ok
    anchors.centerIn: Overlay.overlay
    modal: true
    width: 300
    height: 400

    property int propId
    property string propType
    property var dbModel
    property var pageId
    property var propValues: []
    property var currentValues: []

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        TextField {
            id: searchField
            placeholderText: "Search or add new..."
            Layout.fillWidth: true
            color: "white"
            background: Rectangle { color: "#222"; radius: 4; border.color: "#444" }
            onAccepted: {
                if (text.length > 0 && !root.propValues.includes(text)) {
                    root.dbModel.addPropertyValue(root.propId, text);
                    // Force refresh propValues from schema
                    let schema = root.dbModel.schema;
                    for (let i = 0; i < schema.length; i++) {
                        if (schema[i].id === root.propId) {
                            root.propValues = schema[i].values;
                            break;
                        }
                    }
                    text = "";
                }
            }
        }

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: root.propValues.filter(v => v.toLowerCase().includes(searchField.text.toLowerCase()))
            
            delegate: ItemDelegate {
                width: listView.width
                text: modelData
                highlighted: root.currentValues.includes(modelData)
                
                contentItem: RowLayout {
                    Text {
                        text: modelData
                        color: "white"
                        Layout.fillWidth: true
                    }
                    CheckBox {
                        id: cb
                        checked: {
                            if (root.propType === "select") return root.currentValues === modelData;
                            if (!root.currentValues) return false;
                            let arr = [];
                            if (Array.isArray(root.currentValues)) arr = root.currentValues;
                            else if (typeof root.currentValues === 'object' && root.currentValues.length !== undefined) arr = Array.from(root.currentValues);
                            else arr = [root.currentValues];
                            return arr.indexOf(modelData) !== -1;
                        }
                        onToggled: {
                            let newVals;
                            if (root.propType === "select") {
                                newVals = checked ? modelData : "";
                            } else {
                                // Multi-select
                                let arr = [];
                                if (Array.isArray(root.currentValues)) {
                                    arr = Array.from(root.currentValues);
                                } else if (root.currentValues && typeof root.currentValues === 'object' && root.currentValues.length !== undefined) {
                                    arr = Array.from(root.currentValues);
                                } else if (root.currentValues) {
                                    arr = [root.currentValues];
                                }

                                if (checked) {
                                    if (arr.indexOf(modelData) === -1) arr.push(modelData);
                                } else {
                                    let idx = arr.indexOf(modelData);
                                    if (idx !== -1) arr.splice(idx, 1);
                                }
                                newVals = arr;
                            }
                            root.currentValues = newVals;
                            root.dbModel.setPropertyByPageId(root.pageId, root.propId, newVals);
                        }
                    }
                }
            }
        }
    }
}
