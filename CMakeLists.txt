cmake_minimum_required(VERSION 3.16)

project(PKM_03_qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Sql)
find_package(Qt6 REQUIRED COMPONENTS Core Quick Sql)

qt_standard_project_setup(REQUIRES 6.8)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
