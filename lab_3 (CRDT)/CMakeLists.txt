cmake_minimum_required(VERSION 3.14)
project(Es3_CRDT)

set(CMAKE_CXX_STANDARD 17)

add_executable(Es3_CRDT main.cpp NetworkServer.cpp NetworkServer.h Symbol.cpp Symbol.h SharedEditor.cpp SharedEditor.h Message.cpp Message.h)