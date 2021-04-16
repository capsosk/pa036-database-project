cmake_minimum_required(VERSION 3.19)
project(db_project)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # clang/appleclang/gcc
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -Wall -Wextra -pedantic -Wold-style-cast")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    # Microsoft compiler
    set(CMAKE_CXX_FLAGS "/permissive- /W4 /EHsc")
    add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()


set(SOURCES main.cpp src/ChronoWrapper.cpp src/MongoDatabase.cpp src/PostgresqlDatabase.cpp)

add_executable(db_project ${SOURCES})