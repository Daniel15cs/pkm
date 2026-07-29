import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
// import QtQuick.Dialogs
// pragma ComponentBehavior: Bound
Item{
	id: pageViewRoot
	required property var pageManager
	// property var page: Qt.binding(function() { return pageManager.p_currentPage })
	//  property var pageModel: Qt.binding(function() { return root.page ? root.page.model : null })
	property var page: pageManager.p_currentPage
	property var pageModel: pageViewRoot.page ? pageViewRoot.page.model : null
	//WARNING: pageModel is null after  setRootpage	

	property var parentPage: pageManager.getPageById(page.p_data.parentId)
	property var parentDBModel: parentPage && parentPage.p_data.id !== -1 && parentPage.p_data.type === "DataBase" ? parentPage.dbModel : null

	onPageChanged: {
		mainLoader.sourceComponent = pageViewRoot.page.p_data.type === "DataBase" ? dbViewComp : standardViewComp
	}
	
	Loader {
		id: mainLoader
		anchors.fill: parent
		sourceComponent: pageViewRoot.page.p_data.type === "DataBase" ? dbViewComp : standardViewComp
	}

	Component {
		id: dbViewComp
		DBView {
			pageManager: pageViewRoot.pageManager
			page: pageViewRoot.page
		}
	}

    CalendarDialog {
        id: calDialog
        property int targetPropId
        function onDateSelected(formatted) {
            pageViewRoot.parentDBModel.setPropertyByPageId(pageViewRoot.page.p_data.id, targetPropId, formatted)
        }
    }

    PropertyEditorDialog {
        id: propEditDialog
    }

	Component {
		id: standardViewComp
		Flickable{
			id: flick
			anchors.fill:parent

			ListView{
				id:listGrid
				anchors.fill:parent

				TapHandler {
					onTapped: {
						if (typeof pageTitle !== "undefined" && pageTitle.activeFocus) return;
						var model = pageViewRoot.pageModel
						if (!model) return
						var count = model.rowCount()
						if (count === 0) {
							model.append("textBlock")
							listGrid.currentIndex = 0
						} else {
							var lastIndex = count - 1
							var lastBlock = model.getLogic(lastIndex)
							if (lastBlock.typeName() === "textBlock") {
								if (lastBlock.text_p && lastBlock.text_p.length > 0) {
									model.append("textBlock")
									listGrid.currentIndex = count
								} else {
									listGrid.currentIndex = lastIndex
									if (listGrid.currentItem) {
										listGrid.currentItem.onCurrent()
									}
								}
							} else {
								model.append("textBlock")
								listGrid.currentIndex = count
							}
						}
					}
				}

				model: pageViewRoot.pageModel
				spacing:2
				reuseItems: false
				implicitHeight: contentHeight

				onCurrentIndexChanged:{
					if(currentItem){
						currentItem._loader.item.onCurrent()
					}
				}

				Keys.onUpPressed: decrementCurrentIndex()
				Keys.onDownPressed: incrementCurrentIndex()
				delegate: ListViewDelegate{
					width: flick.width
					flickable: flick
					pageManager: pageViewRoot.pageManager
					pageModel: pageViewRoot.pageModel
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
						text: pageViewRoot.pageManager.getPageTitle(pageViewRoot.page.p_data.id)
						placeholderText: "Unnamed"
						width:parent.width
						font.pixelSize: 22
						font.bold: true
						color: "white"
						horizontalAlignment: Text.AlignHCenter
						background: Item{}
						onEditingFinished: pageViewRoot.pageManager.setPageTitle(pageViewRoot.page.p_data.id, text)
						onAccepted: {
							pageViewRoot.pageModel.insert("textBlock", 0)
							listGrid.currentIndex = 0
							focus = false
						}
                        onActiveFocusChanged: pageViewRoot.isTitleFocused = activeFocus
					}

					// Properties Editor
					Column {
						width: parent.width
						visible: pageViewRoot.parentDBModel !== null
						spacing: 12

						Rectangle {
							width: parent.width
							height: 1
							color: "#444"
						}

						Repeater {
							model: pageViewRoot.parentDBModel ? pageViewRoot.parentDBModel.schema : []
							delegate: RowLayout {
								id: propDelegate
								Layout.fillWidth: true
								spacing: 10

								property int propId: modelData.id || 0
								property string propName: modelData.name || ""
								property string propType: modelData.type || ""
								property var propValues: modelData.values || []
								
								property var currentVal: pageViewRoot.parentDBModel ? pageViewRoot.parentDBModel.getPropertyByPageId(pageViewRoot.page.p_data.id, propId) : ""

								Connections {
									target: pageViewRoot.parentDBModel
									function onPropertyChanged(pageId, propertyId, value) {
										if (pageId === pageViewRoot.page.p_data.id && propertyId === propId) {
											propDelegate.currentVal = value;
										}
									}
								}

								Text {
									text: (propName || "Property") + ":"
									color: "lightgrey"
									font.pixelSize: 12
									font.bold: true
									Layout.preferredWidth: 100
									elide: Text.ElideRight
								}

								Loader {
									Layout.fillWidth: true
									sourceComponent: {
										switch(propType) {
											case "status": return statusEditor;
											case "select": return selectEditor;
											case "multi-select": return selectEditor; // Use same for now
											case "date": return dateEditor;
											default: return textEditor;
										}
									}

									Component {
										id: textEditor
										TextField {
											Layout.fillWidth: true
											implicitHeight: 30
											font.pixelSize: 12
											text: propDelegate.currentVal || ""
											color: "white"
											leftPadding: 8
											rightPadding: 8
											readOnly: propType === "creation_date"
											horizontalAlignment: Text.AlignLeft
											background: Rectangle {
												color: "#222"
												border.color: parent.activeFocus ? "cyan" : "#444"
												radius: 4
											}
											onEditingFinished: if(propType !== "creation_date") pageViewRoot.parentDBModel.setPropertyByPageId(pageViewRoot.page.p_data.id, propId, text)
										}
									}

									Component {
										id: statusEditor
										ComboBox {
											Layout.fillWidth: true
											height: 24
											font.pixelSize: 12
											model: propValues
											currentIndex: Math.max(0, model.indexOf(propDelegate.currentVal || ""))
											onActivated: pageViewRoot.parentDBModel.setPropertyByPageId(pageViewRoot.page.p_data.id, propId, currentText)
										}
									}

									Component {
										id: selectEditor
										RowLayout {
											Layout.fillWidth: true
											spacing: 5
											
											Flow {
												id: selectedValuesFlow
												Layout.fillWidth: true
												spacing: 4
												Repeater {
													model: {
														let val = propDelegate.currentVal;
														let arr = [];
														if (Array.isArray(val)) arr = val;
														else if (val && typeof val === 'object' && val.length !== undefined) arr = Array.from(val);
														else if (val) arr = [val];
														return arr.slice(0, 5);
													}
													delegate: Rectangle {
														color: "#333"
														radius: 3
														height: 20
														width: label.width + 10
														Text {
															id: label
															anchors.centerIn: parent
															text: modelData
															color: "white"
															font.pixelSize: 10
														}
													}
												}
												Text {
													visible: {
														let val = propDelegate.currentVal;
														let arr = [];
														if (Array.isArray(val)) arr = val;
														else if (val && typeof val === 'object' && val.length !== undefined) arr = Array.from(val);
														else if (val) arr = [val];
														return arr.length > 5;
													}
													text: "..."
													color: "#888"
													font.pixelSize: 10
													Layout.alignment: Qt.AlignVCenter
												}
											}

											Button {
												text: "Select..."
												implicitHeight: 24
												implicitWidth: 70
												onClicked: {
													propEditDialog.propId = propId;
													propEditDialog.propType = propType;
													propEditDialog.dbModel = pageViewRoot.parentDBModel;
													propEditDialog.pageId = pageViewRoot.page.p_data.id;
													propEditDialog.propValues = propValues;
													let curr = propDelegate.currentVal;
													propEditDialog.currentValues = Array.isArray(curr) ? curr : (curr ? [curr] : []);
													propEditDialog.open();
												}
											}
										}
									}

									Component {
										id: dateEditor
										RowLayout {
											spacing: 2
											TextField {
												id: dateField
												Layout.fillWidth: true
												height: 24
												font.pixelSize: 12
												text: propDelegate.currentVal || ""
												placeholderText: "dd.MM.yyyy HH:mm"
												color: "white"
												background: Rectangle { color: "#222"; border.color: "#444"; radius: 4 }
												onEditingFinished: pageViewRoot.parentDBModel.setPropertyByPageId(pageViewRoot.page.p_data.id, propId, text)
											}
											Button {
												text: "📅"
												implicitWidth: 30
												implicitHeight: 24
												onClicked: {
													calDialog.targetPropId = propId;
                                                    let d = new Date();
                                                    if (dateField.text) {
                                                        // Simple parsing for dd.MM.yyyy
                                                        let parts = dateField.text.split(".");
                                                        if (parts.length >= 3) d = new Date(parts[2].split(" ")[0], parts[1]-1, parts[0]);
                                                    }
                                                    calDialog.selectedDate = d;
													calDialog.open();
												}
											}
										}
									}
								}

								Row {
									spacing: 4
									visible: propId !== 0
									Button {
										text: "✎"
										implicitWidth: 24
										implicitHeight: 24
										font.pixelSize: 10
										onClicked: renameDialog.openRename(propId, propName)
									}
									Button {
										text: "✕"
										implicitWidth: 24
										implicitHeight: 24
										font.pixelSize: 10
										onClicked: pageViewRoot.parentDBModel.removeProperty(propId)
									}
								}
							}
						}

						Button {
							text: "+ Add Parameter"
							implicitHeight: 26
							font.pixelSize: 12
							onClicked: addParamMenu.popup()
							Menu {
								id: addParamMenu
								MenuItem { text: "Status"; onClicked: pageViewRoot.parentDBModel.addProperty("Status", "status") }
								MenuItem { text: "Select"; onClicked: pageViewRoot.parentDBModel.addProperty("Select", "select") }
								MenuItem { text: "Multi-select"; onClicked: pageViewRoot.parentDBModel.addProperty("Multi-select", "multi-select") }
								MenuItem { text: "Date"; onClicked: pageViewRoot.parentDBModel.addProperty("Date", "date") }
								MenuItem { text: "Text"; onClicked: pageViewRoot.parentDBModel.addProperty("Text", "text") }
							}
						}

						Rectangle {
							width: parent.width
							height: 1
							color: "#444"
						}
					}
				}

				Dialog {
					id: renameDialog
					title: "Rename Parameter"
					standardButtons: Dialog.Ok | Dialog.Cancel
					property int targetId
					Column {
						spacing: 10
						TextField {
							id: renameField
							placeholderText: "New Name"
							onAccepted: renameDialog.accept()
						}
					}
					function openRename(id, oldName) {
						targetId = id
						renameField.text = oldName
						open()
					}
					onAccepted: pageViewRoot.parentDBModel.renameProperty(targetId, renameField.text)
				}

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
			}
		}
	}
}
