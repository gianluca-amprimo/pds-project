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
find_package(Qt5PrintSupport REQUIRED)

qt5_add_resources(QRC icons.qrc)

list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::Charts
        Qt5::PrintSupport
        )
list(REMOVE_DUPLICATES LIBRARIES)

add_executable(editor
        main.cpp
        MainEditor.cpp MainEditor.h
        MyTextArea.cpp MyTextArea.h
        FracPosition.cpp FracPosition.h
        client.cpp client.h
        User.cpp User.h
        Symbol.cpp Symbol.h
        ${QRC})

target_link_libraries(editor ${LIBRARIES})
