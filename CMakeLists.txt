cmake_minimum_required(VERSION 3.0.0)
project(AsioGameServer VERSION 0.1.0)

include(CTest)
enable_testing()

set(This AsioGameServer)
add_subdirectory(include/googletest)

set(Sources
    src/ConnectionHandler.cc
    )
set(CMAKE_CXX_FLAGS
"-g -Wall -DBOOST_LOG_DYN_LINK")
add_library(${This} STATIC ${Sources} ${Headers})
add_subdirectory(test)