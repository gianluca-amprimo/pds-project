cmake_minimum_required(VERSION 3.14)
project(provaQT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)
find_package(Qt5Network REQUIRED)


list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        )
list(REMOVE_DUPLICATES LIBRARIES)


add_executable(provaQT
        provaQT.cpp FileSaver.cpp FileSaver.h)

target_link_libraries(provaQT ${LIBRARIES})
