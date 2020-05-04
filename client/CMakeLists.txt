cmake_minimum_required(VERSION 3.14)
project(client)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Charts REQUIRED)

list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
#        Qt5::Charts
        )
list(REMOVE_DUPLICATES LIBRARIES)

qt5_add_resources(QRC icons.qrc)

add_executable(client
               main.cpp
               client.cpp
               client.h
               User.cpp
               User.h
               MainEditor.cpp
               MainEditor.h
               SaveAsDialog.cpp
               SaveAsDialog.h
               Symbol.cpp
               Symbol.h
               ${QRC} )
target_link_libraries(client ${LIBRARIES})

