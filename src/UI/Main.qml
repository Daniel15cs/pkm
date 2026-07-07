import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
// import myPageBlock
pragma ComponentBehavior: Bound
ApplicationWindow {
	id:root
	width: 900
	height: 500
	visible: true
	title: "My_own_PKM"
	color: "black"

	property bool sidebarVisible: true
	onSidebarVisibleChanged:{
			if(root.sidebarVisible == true){ 
				sidebarToggle.anchors.left = undefined
				sidebarToggle.anchors.right = menuRect.right
				checkWidthForSidebar()
			}
			else if (root.sidebarVisible == false){
				menuRect.anchors.fill = undefined
				sidebarToggle.anchors.right = undefined
				sidebarToggle.anchors.left = menuRect.right
			}
	}
	function setFocusToEditor(){
		if(root.width<=400 && root.sidebarVisible){
			menuRect.anchors.fill = undefined
			sidebarVisible = false
		}
	}
	function checkWidthForSidebar(){
		if(root.width<=400){
			if(root.sidebarVisible){
				menuRect.anchors.fill = menuRect.parent
			}	
		} else{
			menuRect.width = root.sidebarVisible ? Math.max(200, root.width/4) : 0
			menuRect.anchors.fill = undefined
		}
	}

	Shortcut {
		sequence: "Ctrl+R"
		onActivated: {
			searchPanel.visible = true
			searchField.forceActiveFocus()
		}
	}
	Shortcut {
		sequence: "Ctrl+E"
		onActivated: {
			root.sidebarVisible =! root.sidebarVisible
			sidebarToggle.reanchor()
		}
	}
	onWidthChanged:{
		checkWidthForSidebar()
	}

	MessageDialog{
		id:clearConfirm
		title: "Confirm"
		informativeText:"Are you sure to clean current page?"
		buttons: MessageDialog.Ok | MessageDialog.Cancel
		onAccepted:{p_pageManager.uploadList()}
	}
	Rectangle{
		id: menuRect
		color:"#222"
		width: root.sidebarVisible ? Math.max(200, root.width/4) : 0
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		clip: true

		Behavior on width {
			NumberAnimation { duration: 200; easing.type: Easing.InOutQuad }
		}

		ColumnLayout {
			anchors.fill: parent
			anchors.margins: 10
			spacing: 10
			visible: root.sidebarVisible

			Text{
				id: pathText
				text: "Explorer"
				color: "white"
				font.bold: true
				Layout.fillWidth: true
			}

			Button{
				text:"Search"
				Layout.fillWidth: true
				onClicked: {
					searchPanel.visible = true
					searchField.forceActiveFocus()
				}
					background: Rectangle {
						color: parent.hovered ? "#444" : "transparent"
						radius: 4
					}
			}

			RowLayout {
				Layout.fillWidth: true
				Text {
					text: "Notes"
					color: "#aaa"
					font.pixelSize: 12
					font.bold: true
					Layout.fillWidth: true
				}
				Button {
					text: "+"
					implicitWidth: 24
					implicitHeight: 24
					onClicked: {
						var newId = p_pageManager.appendPageToList(0)
						p_pageManager.setCurrentPage(newId)
					}
					background: Rectangle {
						color: parent.hovered ? "#444" : "transparent"
						radius: 4
					}
				}
			}

			ListView {
				id: rootPagesList
				Layout.fillWidth: true
				Layout.fillHeight: true
				model: p_pageManager.rootPages
				clip: true
				delegate: ItemDelegate {
					required property var modelData
					width: rootPagesList.width
					onClicked: {
						p_pageManager.setCurrentPage(modelData.id)
						root.setFocusToEditor()
					}
					contentItem: Text {
						text: modelData.title
						color: "white"
						elide: Text.ElideRight
						verticalAlignment: Text.AlignVCenter
					}
					background: Rectangle {
						color: highlighted ? "#444" : "transparent"
					}
				}
			}

			Button{
				text:"Upload"
				Layout.fillWidth: true
					background: Rectangle {
						color: parent.hovered ? "#444" : "transparent"
						radius: 4
					}
				onClicked: {
					if(p_pageManager.p_currentPage.model.rowCount()>0)
					clearConfirm.open()
					else p_pageManager.uploadList()
				}
			}
			Button{
				id: saveBtn
				text: "Save"
				Layout.fillWidth: true
					background: Rectangle {
						color: parent.hovered ? "#444" : "transparent"
						radius: 4
					}
				onClicked:{
					p_pageManager.savePagesList()
				}
			}
			Button{
				text:"Go back"
				Layout.fillWidth: true
					background: Rectangle {
						color: parent.hovered ? "#444" : "transparent"
						radius: 4
					}
				onPressed:{
					p_pageManager.getToLastPage()
				}
			}
		}
	}

	Button {
		id: sidebarToggle
		text: root.sidebarVisible ? "◀" : "▶"
		z: 100
		width: 24
		height: 24
		anchors.top: parent.top
		anchors.topMargin: 5
		anchors.right: menuRect.right
		function reanchor(){
		}
		anchors.leftMargin: 5
		onClicked: {
			root.sidebarVisible = !root.sidebarVisible
			reanchor()
		}
		background: Rectangle {
			color: "#333"
			radius: 4
			opacity: 0.8
		}
	}

	Loader{
		id:pageLoader
		sourceComponent: pageItemdel
		anchors.right: parent.right
		anchors.left: sidebarToggle.right
		anchors.leftMargin: 5
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		onLoaded:{
			// pageLoader.item.ctrl = controller
			// pageLoader.item.pageModel = controller.p_rootPage
			var currPage = p_pageManager.p_currentPage;
			var model = currPage.model;
			// console.log("curpage:" +currPage)
			// console.log("pdata: " +currPage.p_data)
			// console.log("id:" +currPage.p_data.id)
			// console.log("model: " +model);
		}
	}
	Component{
		id:pageItemdel
		PageView{ 
			id:pageItem
			pageManager: p_pageManager
			pageModel: p_pageManager.p_currentPage.model
		}
	}

	Rectangle {
		id: searchPanel
		anchors.fill: parent
		color: "#AA000000"
		visible: false
		z: 1000

		MouseArea {
			anchors.fill: parent
			onClicked: searchPanel.visible = false
		}

		Rectangle {
			width: parent.width * 0.6
			height: parent.height * 0.6
			color: "#333"
			radius: 8
			anchors.centerIn: parent
			clip: true

			Shortcut {
				enabled: searchPanel.visible
				sequence: "Esc"
				onActivated: searchPanel.visible = false
			}

			ColumnLayout {
				anchors.fill: parent
				anchors.margins: 15
				spacing: 10

				TextField {
					id: searchField
					placeholderText: "Search content..."
					Layout.fillWidth: true
					color: "white"
					placeholderTextColor: "#888"
					background: Rectangle {
						color: "#444"
						radius: 4
					}
					onTextChanged: {
						if (text.length > 2) {
							searchResults.model = p_pageManager.search(text)
						} else {
							searchResults.model = []
						}
					}
				}

				ListView {
					id: searchResults
					Layout.fillWidth: true
					Layout.fillHeight: true
					clip: true
					delegate: ItemDelegate {
						required property var modelData
						width: parent.width
						onClicked: {
							p_pageManager.setCurrentPage(modelData.id)
							searchPanel.visible = false
						}
						contentItem: Column {
							Text {
								text: modelData.title
								color: "white"
								font.bold: true
							}
						}
						background: Rectangle {
							color: highlighted ? "#555" : "transparent"
						}
					}
				}
			}
		}
	}

}
