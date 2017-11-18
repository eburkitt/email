cmake_minimum_required(VERSION 3.2)
project(email)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.62.0 COMPONENTS log thread system)

if(Boost_FOUND)
    include_directories(message ${Boost_INCLUDE_DIRS})
    set(SOURCE_FILES message/message.hpp message/message_test.cpp)
    add_executable(message_test ${SOURCE_FILES})
    target_link_libraries(message_test ${Boost_LIBRARIES})
endif()
