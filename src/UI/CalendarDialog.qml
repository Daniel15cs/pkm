import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "Select Date"
    standardButtons: Dialog.Ok | Dialog.Cancel
    anchors.centerIn: Overlay.overlay
    modal: true

    property date selectedDate: new Date()
    signal dateSelected(string formattedDate)

    ColumnLayout {
        spacing: 10
        RowLayout {
            Layout.fillWidth: true
            Button { text: "<"; onClicked: root.selectedDate = new Date(root.selectedDate.getFullYear(), root.selectedDate.getMonth() - 1, 1) }
            Text {
                text: Qt.formatDateTime(root.selectedDate, "MMMM yyyy")
                color: "white"
                font.bold: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
            Button { text: ">"; onClicked: root.selectedDate = new Date(root.selectedDate.getFullYear(), root.selectedDate.getMonth() + 1, 1) }
        }

        GridLayout {
            columns: 7
            columnSpacing: 2
            rowSpacing: 2

            Repeater {
                model: ["Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"]
                Text {
                    text: modelData
                    color: "#888"
                    font.pixelSize: 10
                    Layout.alignment: Qt.AlignCenter
                }
            }

            Repeater {
                id: dayRepeater
                model: 42
                delegate: Rectangle {
                    width: 30
                    height: 30
                    radius: 4
                    color: isSelected ? "cyan" : (isCurrentMonth ? "#333" : "#222")
                    opacity: isCurrentMonth ? 1.0 : 0.4
                    
                    property int dayNumber: {
                        let firstDay = new Date(root.selectedDate.getFullYear(), root.selectedDate.getMonth(), 1).getDay();
                        let adjustedFirstDay = (firstDay === 0) ? 6 : firstDay - 1;
                        return index - adjustedFirstDay + 1;
                    }
                    property date itemDate: new Date(root.selectedDate.getFullYear(), root.selectedDate.getMonth(), dayNumber)
                    property bool isCurrentMonth: itemDate.getMonth() === root.selectedDate.getMonth()
                    property bool isSelected: isCurrentMonth && itemDate.getDate() === root.selectedDate.getDate()

                    Text {
                        anchors.centerIn: parent
                        text: parent.dayNumber > 0 && parent.dayNumber <= new Date(root.selectedDate.getFullYear(), root.selectedDate.getMonth() + 1, 0).getDate() ? parent.dayNumber : ""
                        color: parent.isSelected ? "black" : "white"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: if (parent.dayNumber > 0) root.selectedDate = parent.itemDate
                    }
                }
            }
        }
    }

    onAccepted: {
        root.dateSelected(Qt.formatDateTime(root.selectedDate, "dd.MM.yyyy HH:mm"))
    }
}
