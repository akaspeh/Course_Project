cmake_minimum_required(VERSION 3.23)
project(Course_client)

set(CMAKE_CXX_STANDARD 23)

add_executable(Course_client main.cpp Client.cpp Client.h)

target_link_libraries(Course_client PRIVATE ws2_32 dbghelp stdc++fs)