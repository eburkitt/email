cmake_minimum_required(VERSION 3.2)
project(email)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.62.0 REQUIRED COMPONENTS log thread system)
include_directories(message ${Boost_INCLUDE_DIRS})

set(MESSAGE_SOURCE_FILES message/message.hpp message/message_test.cpp)
set(SMTP_SOURCE_FILES smtp_multi_xmpl/smtp_multi_xmpl.cpp)
add_executable(message_test ${MESSAGE_SOURCE_FILES})
add_executable(smtp_client ${SMTP_SOURCE_FILES})

target_link_libraries(message_test ${Boost_LIBRARIES})
target_link_libraries(smtp_client curl)

