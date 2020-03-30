cmake_minimum_required(VERSION 3.15)
project(Editor)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

list(APPEND LIBRARIES
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        Qt5::PrintSupport)
list(REMOVE_DUPLICATES LIBRARIES)

add_executable(Editor main.cpp LoginWindow.cpp LoginWindow.h TextEdit.cpp TextEdit.h RegistrationWindow.cpp RegistrationWindow.h Server.cpp Server.h)
target_link_libraries(Editor ${LIBRARIES})