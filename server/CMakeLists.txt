cmake_minimum_required(VERSION 3.14)
project(server)

# localizzare il path di Qt nell'attuale sistema operativo
# set(CMAKE_PREFIX_PATH "/home/pioniere/Qt/5.12.7/gcc_64/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)


find_package(SQLite3 REQUIRED)

list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        sqlite3
        )
list(REMOVE_DUPLICATES LIBRARIES)

add_executable(server
        main.cpp
        server.cpp
        server.h
        server.ui
        db_operations.cpp
        db_operations.h
        User.cpp
        User.h
        SymbolPrototype.cpp
        )
target_link_libraries(server ${LIBRARIES})