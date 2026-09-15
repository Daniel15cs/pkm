add_library(core STATIC
	PageModel.cc PageModel.h
	filemodel.h filemodel.cc
	PageManager.cc PageManager.h
	DatabaseModel.cc DatabaseModel.h
	DatabaseSortProxyModel.cc DatabaseSortProxyModel.h
)
add_subdirectory(BlockSystem)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core PUBLIC Qt6::Quick Qt6::Core Qt6::Sql)

qt_add_executable(appPKM_03_qml main.cc)
qt_add_qml_module(appPKM_03_qml
    URI PKM_03_qml
    VERSION 1.0
    QML_FILES
				UI/Main.qml
				UI/TextListItem.qml
				UI/CheckboxListItem.qml
				UI/BulletListItem.qml
				UI/PageView.qml
				UI/PageListItem.qml
				UI/ToggleItem.qml
				UI/ListViewDelegate.qml
				UI/DBView.qml
				UI/DBTableView.qml
				UI/DBKanbanView.qml
				UI/DBListView.qml
				UI/KanbanColumn.qml
				UI/CalendarDialog.qml
				UI/PropertyEditorDialog.qml
		SOURCES
				QML_FILES UI/ToggleItem.qml
)

set_target_properties(appPKM_03_qml PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
target_link_libraries(appPKM_03_qml PRIVATE core)

include(GNUInstallDirs)
install(TARGETS appPKM_03_qml
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
