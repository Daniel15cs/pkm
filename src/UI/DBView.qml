import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import PKM_03_qml
pragma ComponentBehavior: Bound

Item {
	id: root
	required property var pageManager
	required property var page

	// Reference to the active proxy model for sorting
	property var activeProxyModel: viewLoader.item ? viewLoader.item.proxyModel : null
	property string activeViewType: {
		if (viewLoader.source.toString().includes("DBTableView.qml")) return "table";
		if (viewLoader.source.toString().includes("DBKanbanView.qml")) return "kanban";
		if (viewLoader.source.toString().includes("DBListView.qml")) return "list";
		return "table";
	}

	ColumnLayout {
		anchors.fill: parent
		spacing: 0
		clip: true

		// Header
		Rectangle {
			Layout.fillWidth: true
			Layout.preferredHeight: 100
			color: "#1a1a1a"
			z: 2

			ColumnLayout {
				anchors.fill: parent
				anchors.margins: 10
				spacing: 10

				TextField {
					id: dbTitle
					text: root.pageManager.getPageTitle(root.page.p_data.id)
					placeholderText: "Database Name"
					Layout.fillWidth: true
					font.pixelSize: 22
					font.bold: true
					color: "white"
					horizontalAlignment: Text.AlignHCenter
					onEditingFinished: root.pageManager.setPageTitle(pageViewRoot.page.p_data.id, text)
					background: Item {}
				}

				RowLayout {
					Layout.fillWidth: true
					spacing: 8

					// View Switcher
					RowLayout {
						spacing: 2
						Button {
							text: "Table"
							implicitHeight: 28
							font.pixelSize: 12
							highlighted: root.activeViewType === "table"
							onClicked: viewLoader.loadSource("DBTableView.qml")
						}
						Button {
							text: "Kanban"
							implicitHeight: 28
							font.pixelSize: 12
							highlighted: root.activeViewType === "kanban"
							onClicked: viewLoader.loadSource("DBKanbanView.qml")
						}
						Button {
							text: "List"
							implicitHeight: 28
							font.pixelSize: 12
							highlighted: root.activeViewType === "list"
							onClicked: viewLoader.loadSource("DBListView.qml")
						}
					}

					Item { Layout.fillWidth: true }

					// Actions
					Button {
						text: "Visibility"
						implicitHeight: 28
						font.pixelSize: 12
						onClicked: visibilityMenu.popup()
						
						Menu {
							id: visibilityMenu
							Repeater {
								model: root.page.dbModel.schema
								MenuItem {
									required property var modelData
									text: modelData.name
									checkable: true
									checked: root.page.dbModel.isPropertyVisible(modelData.id, root.activeViewType)
									onTriggered: root.page.dbModel.setPropertyVisible(modelData.id, root.activeViewType, checked)
								}
							}
						}
					}

					Button {
						text: "+ Add Note"
						implicitHeight: 28
						font.pixelSize: 12
						onClicked: root.page.dbModel.addNote()
					}

					Button {
						id: sortBtn
						text: "Sort"
						implicitHeight: 28
						font.pixelSize: 12
						onClicked: sortMenu.popup()

						Menu {
							id: sortMenu
							title: "Sort by Property"

							Repeater {
								model: root.page.dbModel.schema
								MenuItem {
									required property var modelData
									required property int index
									text: modelData.name + " (Asc)"
									onClicked: if(root.activeProxyModel) root.activeProxyModel.sortByColumn(index + 1, Qt.AscendingOrder)
								}
							}

							MenuSeparator { visible: root.page.dbModel.schema.length > 0 }

							Repeater {
								model: root.page.dbModel.schema
								MenuItem {
									required property var modelData
									required property int index
									text: modelData.name + " (Desc)"
									onClicked: if(root.activeProxyModel) root.activeProxyModel.sortByColumn(index + 1, Qt.DescendingOrder)
								}
							}
						}
					}
				}
			}
		}

		// Content Area
		Component.onCompleted:{
			viewLoader.loadSource("DBTableView.qml")
		}
		Loader {
			id: viewLoader
			Layout.fillWidth: true
			Layout.fillHeight: true

			function loadSource(sourceUrl) {
				setSource(sourceUrl, {
					"dbModel": root.page.dbModel
				})
				// root.activeProxyModel = item.proxyModel
			}

		}
	}
}
