cmake_minimum_required(VERSION 3.16)

project(PKM_03_qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Sql)

qt_standard_project_setup(REQUIRES 6.8)
qt_add_executable(appPKM_03_qml
    main.cc
		uiModel.cc uiModel.h
		PageModel.cc PageModel.h
    filemodel.h filemodel.cc
		rootModelController.h rootModelController.cc
		PageManager.cc PageManager.h
)

add_subdirectory(BlockSystem)
qt_add_qml_module(appPKM_03_qml
    URI PKM_03_qml
    VERSION 1.0
    QML_FILES
        Main.qml
				qml/TextListItem.qml
				qml/CheckboxListItem.qml
				qml/PageView.qml
				qml/PageListItem.qml
		SOURCES
				main.cc
				uiModel.cc uiModel.h
				PageModel.cc PageModel.h
				filemodel.h filemodel.cc
				rootModelController.h rootModelController.cc
				PageManager.cc PageManager.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPKM_03_qml PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPKM_03_qml
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

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
set(CMAKE_AUTOMOC ON)  # увімкнути moc для всіх файлів
