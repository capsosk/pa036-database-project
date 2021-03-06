cmake_minimum_required(VERSION 3.19)
project(db_project)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # clang/appleclang/gcc
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -pthread -Wall -Wextra -pedantic -Wold-style-cast")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    # Microsoft compiler
    set(CMAKE_CXX_FLAGS "/permissive- /W4 /EHsc")
    add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

set(SOURCES main.cpp src/ChronoWrapper.cpp src/MongoDatabase.cpp src/PostgresqlDatabase.cpp src/FileParser.cpp)

include_directories("/usr/local/lib/include/mongocxx/v_noabi")
include_directories("/usr/local/lib/include/bsoncxx/v_noabi")

find_package(jsoncpp CONFIG REQUIRED)

add_executable(db_project ${SOURCES})

set(PQ_LIBS pqxx pq)
set(MONGO_LIBS mongoc-1.0 mongocxx-static)
set(BSON_LIBS bson-1.0 bson-static-1.0 bsoncxx-static)
set(JSON jsoncpp)

target_link_libraries(db_project ${PQ_LIBS} ${MONGO_LIBS} ${BSON_LIBS} ${JSON})
