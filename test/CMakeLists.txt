cmake_minimum_required(VERSION 3.19.0)
set(This Tests)
set(Sources ConnectionHandlerTest.cc)
set(BOOST_ROOT "/home/jared/boost")
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost QUIET COMPONENTS log system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${This} ${Sources})
    target_link_libraries(${This} PUBLIC
        gtest_main
        AsioGameServer
        ${Boost_LIBRARIES}
    )
else()
    message("Boost was not found")
endif()

add_test(
    NAME ${This}
    COMMAND ${This}
)