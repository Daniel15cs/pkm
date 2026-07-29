cmake_minimum_required(VERSION 3.16)

project(PKM_03_qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Sql)

qt_standard_project_setup(REQUIRES 6.8)
add_library(core STATIC
	src/PageModel.cc src/PageModel.h
	src/filemodel.h src/filemodel.cc
	src/PageManager.cc src/PageManager.h
	src/DatabaseModel.cc src/DatabaseModel.h
	src/DatabaseSortProxyModel.cc src/DatabaseSortProxyModel.h
	src/BlockSystem/block.h 
	src/BlockSystem/textBlock.h src/BlockSystem/textBlock.cc
	src/BlockSystem/checkboxBlock.h src/BlockSystem/checkboxBlock.cc
	src/BlockSystem/pageBlock.h src/BlockSystem/pageBlock.cc
	src/BlockSystem/toggleBlock.cc  src/BlockSystem/toggleBlock.h
	src/BlockSystem/bulletBlock.h   src/BlockSystem/bulletBlock.cc
)
target_include_directories(core PUBLIC src)
target_link_libraries(core PUBLIC Qt6::Quick Qt6::Core Qt6::Sql)
set(CMAKE_AUTOMOC ON)  # увімкнути moc для всіх файлів
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
# target_link_libraries(appPKM_03_qml PUBLIC Qt6::Core Qt6::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.

# target_link_libraries(appPKM_03_qml PRIVATE Qt6::Quick)

# include(GNUInstallDirs)
# install(TARGETS appPKM_03_qml
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
