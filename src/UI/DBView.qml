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
	property var activeProxyModel: viewLoader.item.pModel ??null

	ColumnLayout {
		anchors.fill: parent
		spacing: 0

		// Header
		Rectangle {
			Layout.fillWidth: true
			Layout.preferredHeight: 50
			color: "#1a1a1a"

			RowLayout {
				anchors.fill: parent
				anchors.leftMargin: 10
				anchors.rightMargin: 10
				spacing: 10

				// View Switcher
				RowLayout {
					spacing: 2
					Button {
						text: "Table"
						highlighted: viewLoader.source.toString().includes("DBTableView.qml")
						onClicked: viewLoader.loadSource("DBTableView.qml")
					}
					Button {
						text: "Kanban"
						highlighted: viewLoader.source.toString().includes("DBKanbanView.qml")
						onClicked: viewLoader.loadSource("DBKanbanView.qml")
					}
					Button {
						text: "List"
						highlighted: viewLoader.source.toString().includes("DBListView.qml")
						onClicked: viewLoader.loadSource("DBListView.qml")
					}
				}

				Item { Layout.fillWidth: true }

				// Actions
				Button {
					text: "+ Add Note"
					onClicked: root.page.dbModel.addNote()
				}

				Button {
					id: sortBtn
					text: "Sort"
					onClicked: sortMenu.popup()

					Menu {
						id: sortMenu
						title: "Sort by Property"

						Repeater {
							model: root.page.dbModel.schema
							MenuItem {
								required property var model
								text: model.name + " (Asc)"
								onClicked: if(root.activeProxyModel) root.activeProxyModel.sortByColumn(model.index + 1, Qt.AscendingOrder)
							}
						}

						MenuSeparator { visible: root.page.dbModel.schema.length > 0 }

						Repeater {
							model: root.page.dbModel.schema
							MenuItem {
								required property var model
								text: model.name + " (Desc)"
								onClicked: if(root.activeProxyModel) root.activeProxyModel.sortByColumn(model.index + 1, Qt.DescendingOrder)
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
