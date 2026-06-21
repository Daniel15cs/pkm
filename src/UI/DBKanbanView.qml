import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import PKM_03_qml

Item {
    id: root
    required property var dbModel
    // For Kanban, sorting is less common, but we'll expose a dummy or the first column's proxy if needed
    property var proxyModel: null 

    RowLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 5

        KanbanColumn {
            title: "To-Do"
            status: "To-Do"
            dbModel: root.dbModel
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        KanbanColumn {
            title: "In Progress"
            status: "In-progress" 
            dbModel: root.dbModel
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        KanbanColumn {
            title: "Done"
            status: "Done"
            dbModel: root.dbModel
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
