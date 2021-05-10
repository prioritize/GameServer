cmake_minimum_required(VERSION 3.0.0)
project(AsioGameServer VERSION 0.1.0)

include(CTest)
enable_testing()

set(This AsioGameServer)
set(Simp SimpleClient)
add_subdirectory(include/googletest)

set(Sources
    src/ConnectionHandler.cc
    )
set(SimpleExec
    src/SimpleClient.cpp)
set(Headers 
    include/net_common.hpp
    include/net_message.hpp
    include/ConnectionHandler.hpp
    )
find_library(Boost QUIET components log system required)
add_executable(${Simp} ${SimpleExec} ${Headers})
add_executable()
set(CMAKE_CXX_FLAGS
"-g -Wall -DBOOST_LOG_DYN_LINK -pthread")
add_library(${This} STATIC ${Sources} ${Headers})
add_subdirectory(test)