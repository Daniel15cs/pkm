import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound
Item {
	id: root
	required property var pageManager
	required property var model
	required property int index
	required property var listView

	property var logicobject: root.model.getLogic(root.index)
	property var pageModel: logicobject.p_model
	property var page: pageModel.p_pageData

	property int _width:500
	readonly property int headerHeight: 25
	width: _width
	height: headerHeight + (expanded ? panel.implicitHeight : 0)
	property bool expanded: false

	property alias _compText:compText
	property string _text: logicobject?.text_p
	readonly property string blockType:"toggleBlock"

	onHeightChanged:{
		var item = root.listView.itemAtIndex(index);
		if(item !=null)
		item.height = root.height;
	} 

	function onCurrent(){
		compText.forceActiveFocus(Qt.TabFocusReason)
		// btn.highlighted = !btn.highlighted
	}

	Component.onCompleted:{
		console.log("lo: "+ root.logicobject)
		console.log("pM: "+ root.pageModel)
	}


	Column {
		anchors.fill: parent
		spacing: 4
		Row{
			// spacing: 4
			Button {
				id:btn
				width: root.headerHeight
				height: root.headerHeight
				text: root.expanded ? "v" : ">"
				onClicked:{
					root.listView.currentIndex = root.index
					root.onCurrent()
					root.listView.currentItem.height = root.height
					root.expanded = !root.expanded
				} 
				background: Rectangle {
					opacity: enabled ? 1 : 0.3
					color:"black" 
				}
			}
			TextArea{
				id: compText
				height:root.headerHeight
				width:root.width-50
				verticalAlignment:TextEdit.AlignVCenter
				persistentSelection: true
				text: root._text
				placeholderText: compText.activeFocus==true ? "Enter text" :""
				implicitHeight: contentHeight
				// wrapMode: TextArea.Wrap
				// focus:true

				background: Item{
					// anchors.fill: parent
					// Rectangle{ anchors.fill:parent; color: "Grey"}
				}
				onPressed:{
					root.listView.currentIndex = root.index
					if(compText.activeFocus==false)
					root.onCurrent()
				}

				onTextChanged:{
					root.logicobject.setText(text)
					root.listView.currentItem.height = root.height
				}

				Keys.onPressed:function(event){
					if(event.key ===Qt.Key_Return || event.key === Qt.Key_Enter){
						if(event.modifiers & Qt.ShiftModifier){
							event.accepted = false
						}if(event.modifiers & Qt.ControlModifier){
							event.accepted = true
							root.expanded = ! root.expanded
						}
						else{
							event.accepted = true
							if(root.index==root.pageModel.rowCount()){
								root.pageModel.append(root.blockType)
								root.listView.incrementCurrentIndex()
							}
							else if(root.index<root.pageModel.rowCount()){
								root.pageModel.insert(root.blockType,root.index+1)
								root.listView.incrementCurrentIndex()
							}
						}
					}else if(compText.text ==="" && event.key===Qt.Key_Backspace){
						root.pageModel.removeRow(root.index)
						root.listView.decrementCurrentIndex()
					}
				}
			}

			Button{
				id:compBtn
				text: "x"
				height:25
				width:25
				// visible:hover.hovered

				background: Rectangle {
					opacity: enabled ? 1 : 0.3
					color:"black" 
				}
				onClicked:{
					if (root.index>=0)
					root.pageModel.removeRow(root.index);
				}
			}
		}

		Rectangle {
			id: panel
			width: parent.width
			color: "black"
			// clip: true
			implicitHeight: content.implicitHeight + 10
			height: root.expanded ? implicitHeight : 0

			Behavior on height {
				NumberAnimation { duration: 50}
			}

			Item {
				id: content
				width: parent.width

				implicitHeight: column.implicitHeight

				Column {
					id: column
					width: parent.width
					spacing: 4

					TextArea {
						id:tta
						width: parent.width
						text: "test 1"
						wrapMode: TextEdit.Wrap
					}

					TextArea {
						width: parent.width
						text: "test 2"
						wrapMode: TextEdit.Wrap
					}
					Item{// internal ListView
					width: parent.width
					height: 100
					Rectangle{
						anchors.fill:parent
						color:"grey"
							border.color: "red"
							border.width: 5
					}
					Column{
						width: parent.width
						spacing: 10
						Button{
							// HoverHandler{ id:btnHover}
							id:addBtn
							text:"+"
							// anchors.topMargin:50
							// anchors.fill:parent
							// anchors.top:parent.top
							width:parent.width

							// visible:hover.hovered | btnHover.hovered
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
								color: "blue"
							}
							Menu{
								id:itemMenu
								Menu{
									title: "Add block"
									Action{
										text :"Text"
										onTriggered:{
											//TODO: model is null
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
								}
							}
						}//button
						Repeater{
							id:listGrid
							// anchors.fill:parent
							model: root.pageModel
							// model: ListModel{
							// 	ListElement{
							// 	}
							// 	ListElement{
							// 	}
							// }
							// delegate:
							// Item {
							// 	id:de
							// 	Rectangle{
							// 		anchors.fill:parent
							// 		color:"pink"
							// 	}
							// }
							// spacing:2
							// reuseItems: false
							// implicitHeight: contentHeight

							// onCurrentIndexChanged:{
							// 	// console.log("currind: "+currentIndex)
							// 	if(currentItem){
							// 		currentItem._loader.item.onCurrent()
							// 	}
							// }
							Component.onCompleted:{
								console.log("listgrid height: "+listGrid.height)
								console.log("listgrid imheight: "+listGrid.implicitHeight)
							}
							// Keys.onUpPressed: decrementCurrentIndex()
							// Keys.onDownPressed: incrementCurrentIndex()
								delegate: Item{
									id:delItem
									width: root.width
									//TODO:
									// implicitHeight: loader.item ? loader.item.height : 24
									height:24
									required property int index
									property alias _loader:loader

									Loader{ id:loader 
									Component.onCompleted:{
										var _model = listGrid.model
										var type = _model.getLogic(delItem.index).typeName()
										if(type === "textBlock"){
											loader.sourceComponent = textDel
										} else if(type === "checkboxBlock"){
											loader.sourceComponent = checkboxDel
										} else if(type === "pageBlock"){
											loader.sourceComponent = pageListItemDel
										}
										//TODO: inline toggleblock
										// else if(type==="toggleBlock"){
										// 	loader.sourceComponent = toggleDel
										// }
										else{
											loader.sourceComponent = textDel
										}
										// listGrid.forceLayout()
									}
								}

								Component{
									id:textDel
									TextListItem{ 
										model:listGrid.model
										_width: root.width
										index: delItem.index
										listView: listGrid
									}
								}
								Component{
									id:checkboxDel
									CheckboxListItem{
										model:listGrid.model
										_width: root.width
										index: delItem.index
										listView: listGrid
									}
								}
								Component{
									id:pageListItemDel
									PageListItem{
										model:listGrid.model
										_width: root.width
										index: delItem.index
										pageManager: root.pageManager
									}
								}
								// Component{
								// 	id:toggleDel
								// 	ToggleItem{
								// 		model:listGrid.model
								// 		_width: root.width
								// 		index: delItem.index
								// 		listView: listGrid
								// 		pageManager: root.pageManager
								// 	}
								// }
							}

							// HoverHandler{ 
							// 	id:hover
							// }
							// header:Item{
							// 	width:parent.width
							// 	height: 100
							// 	TextField{
							// 		id: pageTitle
							// 		// TODO:
							// 		text: root.page.id
							// 		placeholderText: "Unnamed"
							// 		width:parent.width
							// 		font.pixelSize: 22
							// 		font.bold: true
							// 		horizontalAlignment: Text.AlignHCenter
							// 		background: Item{}
							// 		onEditingFinished:{
							// 			// TODO:
							// 			// root.pageModel.setPageName(text)
							// 		}
							//
							// 	}
							// }
							// footer: Item{ 
							// 	width:parent.width
							// 	anchors.top:parent.bottom
							// 	height: 50
							// 	Rectangle{
							// 		anchors.fill:parent
							// 		color:"blue"
							// 	}
							//
							// }//footer

						}//listview
					}//col

				}//item
			}//column
		}//content
	}//panel
}//main column
}//root item
