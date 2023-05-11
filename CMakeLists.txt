cmake_minimum_required(VERSION 3.13)

project(hash_server)
set(CMAKE_CXX_STANDARD 20)

add_library(hash_server_lib STATIC src/HashServer.cpp src/ServerContextPool.cpp src/HashConnection.cpp src/MessageParser.cpp)
target_include_directories(hash_server_lib PUBLIC inc)

add_executable(hash_server src/main.cpp)
target_include_directories(hash_server PRIVATE inc)
target_link_libraries(hash_server hash_server_lib crypto pthread)

add_executable(hash_tests test/hash_tests.cpp)
target_include_directories(hash_tests PRIVATE inc)
target_link_libraries(hash_tests hash_server_lib gtest crypto pthread)
