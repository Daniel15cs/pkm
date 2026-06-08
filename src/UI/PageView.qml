import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
// import QtQuick.Dialogs
// pragma ComponentBehavior: Bound
Item{
	id: root
	required property var pageManager
	// property var page: Qt.binding(function() { return pageManager.p_currentPage })
	//  property var pageModel: Qt.binding(function() { return root.page ? root.page.model : null })
	property var page: pageManager.p_currentPage
	property var pageModel: root.page ? root.page.model : null
	//WARNING: pageModel is null after  setRootpage	

	property var parentPage: pageManager.getPageById(page.p_data.parentId)
	property var parentDBModel: parentPage && parentPage.p_data.id !== -1 && parentPage.p_data.type === "DataBase" ? parentPage.dbModel : null

	onPageChanged: {
		mainLoader.sourceComponent = root.page.p_data.type === "DataBase" ? dbViewComp : standardViewComp
	}
	// property var pageModel: ctrl.p_rootPage
	// property var listModel: ctrl.p_rootPage.pageModel
	// required property var loader
	Component.onCompleted:{
		// console.log("pageView ctrl:"+ctrl)
		// console.log("pageView pagemodel:"+pageModel)
		// console.log("pageView rootpage:" +root.ctrl.p_rootPage)
		// console.log("pageView pageModel:" +root.ctrl.p_rootPage.pageModel)
	}
	Loader {
		id: mainLoader
		anchors.fill: parent
		sourceComponent: root.page.p_data.type === "DataBase" ? dbViewComp : standardViewComp
	}

	Component {
		id: dbViewComp
		DBView {
			pageManager: root.pageManager
			page: root.page
		}
	}

	Component {
		id: standardViewComp
		Flickable{
			id: flick
			anchors.fill:parent
			ListView{
				id:listGrid
				anchors.fill:parent
				model: root.pageModel
				spacing:2
				reuseItems: false
				implicitHeight: contentHeight

				onCurrentIndexChanged:{
					// console.log("currind: "+currentIndex)
					if(currentItem){
						currentItem._loader.item.onCurrent()
					}
				}

				Keys.onUpPressed: decrementCurrentIndex()
				Keys.onDownPressed: incrementCurrentIndex()
				delegate: ListViewDelegate{
					width: flick.width
					flickable: flick
					pageManager: root.pageManager
					pageModel: root.pageModel
					listView: listGrid

				}
				HoverHandler{ 
					id:hover
				}
				header: Column {
					width: parent.width
					spacing: 15
					bottomPadding: 20
					leftPadding: 20
					rightPadding: 20

					TextField{
						id: pageTitle
						text: "Note #" + root.page.p_data.id
						placeholderText: "Unnamed"
						width:parent.width
						font.pixelSize: 22
						font.bold: true
						color: "white"
						horizontalAlignment: Text.AlignHCenter
						background: Item{}
					}

					// Properties Editor
					Column {
						width: parent.width
						visible: root.parentDBModel !== null
						spacing: 12

						Rectangle {
							width: parent.width
							height: 1
							color: "#444"
						}

						Repeater {
							model: root.parentDBModel ? root.parentDBModel.schema : []
							delegate: RowLayout {
								Layout.fillWidth: true
								spacing: 10

								property int propId: modelData.id || 0
								property string propName: modelData.name || ""
								property string propType: modelData.type || ""
								property var propValues: modelData.values || []

								Text {
									text: (propName || "Property") + ":"
									color: "lightgrey"
									font.pixelSize: 14
									font.bold: true
									Layout.preferredWidth: 80
									elide: Text.ElideRight
								}

								Loader {
									Layout.fillWidth: true
									sourceComponent: (propType === "status" || propType === "enum") ? comboEditor : textEditor

									Component {
										id: textEditor
										TextField {
											Layout.fillWidth: true
											text: root.parentDBModel ? root.parentDBModel.getPropertyByPageId(root.page.p_data.id, propId) || "" : ""
											color: "white"
											background: Rectangle {
												color: "#222"
												border.color: parent.activeFocus ? "cyan" : "#444"
												radius: 4
											}
											onEditingFinished: root.parentDBModel.setPropertyByPageId(root.page.p_data.id, propId, text)
										}
									}

									Component {
										id: comboEditor
										ComboBox {
											Layout.fillWidth: true
											model: propValues
											currentIndex: Math.max(0, model.indexOf(root.parentDBModel ? root.parentDBModel.getPropertyByPageId(root.page.p_data.id, propId) : ""))
											onActivated: root.parentDBModel.setPropertyByPageId(root.page.p_data.id, propId, currentText)
										}
									}
								}
							}
						}

						Rectangle {
							width: parent.width
							height: 1
							color: "#444"
						}
					}
				}
				// footer: Item{ 
				// width:parent.width
				// anchors.top:parent.bottom
				// height:50
				// Rectangle{
				// 	anchors.fill:parent
				// 	color:"Grey"
				// }

				Button{
					HoverHandler{ id:btnHover}
					id:addBtn
					text:"+"
					anchors.topMargin:50
					// anchors.fill:parent
					anchors.bottom:parent.bottom
					width:parent.width

					visible:hover.hovered | btnHover.hovered
					Behavior on visible{
						NumberAnimation{
							duration: 100
						}
					}
					onClicked:{
						itemMenu.popup()
					}
					background: Rectangle {
						opacity: enabled ? 1 : 0.3
						color: "black"
					}
					Menu{
						id:itemMenu
						Menu{
							title: "Add block"
							Action{ 
								text :"Text"
								onTriggered:{
									listGrid.model.append("textBlock")
									listGrid.currentIndex = listGrid.model.rowCount()-1
								} 
							}
							Action{ 
								text :"Checkbox"
								onTriggered: {
									listGrid.model.append("checkboxBlock")
									listGrid.currentIndex = listGrid.model.rowCount()-1
								}
							}
							Action{ 
								text :"Bullet"
								onTriggered: {
									listGrid.model.append("bulletBlock")
									listGrid.currentIndex = listGrid.model.rowCount()-1
								}
							}
							Action{ 
								text :"Page"
								onTriggered:{
									// TODO: proper page append AND block to this pageModel
									// root.pageManager.appendPageToList(root.pageModel.getPageData.id)
									listGrid.model.append("pageBlock")
									listGrid.currentIndex = listGrid.model.rowCount()-1
								}
							}
							Action{ 
								text:"Toggle"
								onTriggered:{
									listGrid.model.append("toggleBlock")
									listGrid.currentIndex = listGrid.model.rowCount()-1
								}
							}
							Action{ 
								text:"Database"
								onTriggered:{
									listGrid.model.append("databaseBlock")
									listGrid.currentIndex = listGrid.model.rowCount()-1
								}
							}
						}
					}
				}
				// } //footer
			}
		}
	}
}
