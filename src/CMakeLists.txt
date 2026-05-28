file(GLOB CONFIGURE_DEPENDS SOURCES "*")
# target_sources(appPKM_03_qml PRIVATE 
# 	main.cc
# 	uiModel.cc uiModel.h
# 	PageModel.cc PageModel.h
# 	filemodel.h filemodel.cc
# 	rootModelController.h rootModelController.cc
# 	PageManager.cc PageManager.h
# 	# ${SOURCES}
# )
qt_add_executable(appPKM_03_qml
    main.cc
		uiModel.cc uiModel.h
		PageModel.cc PageModel.h
		filemodel.h filemodel.cc
		rootModelController.h rootModelController.cc
		PageManager.cc PageManager.h
)
qt_add_qml_module(appPKM_03_qml
    URI PKM_03_qml
    VERSION 1.0
    QML_FILES
				UI/Main.qml
				UI/TextListItem.qml
				UI/CheckboxListItem.qml
				UI/PageView.qml
				UI/PageListItem.qml
		SOURCES
				QML_FILES UI/toggleItem.qml
)
set_target_properties(appPKM_03_qml PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPKM_03_qml
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
add_subdirectory(BlockSystem)
target_link_libraries(appPKM_03_qml
    PRIVATE Qt6::Quick
)
target_link_libraries(appPKM_03_qml PRIVATE Qt6::Core Qt6::Sql)
include(GNUInstallDirs)
install(TARGETS appPKM_03_qml
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
