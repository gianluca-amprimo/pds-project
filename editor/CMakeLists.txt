cmake_minimum_required(VERSION 3.14)
project(editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)
find_package(Qt5Network REQUIRED)

qt5_add_resources(QRC icons.qrc)

list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        )
list(REMOVE_DUPLICATES LIBRARIES)


add_executable(editor
        main.cpp SaveAsDialog.cpp SaveAsDialog.h MainEditor.cpp MainEditor.h ${QRC})

target_link_libraries(editor ${LIBRARIES})
