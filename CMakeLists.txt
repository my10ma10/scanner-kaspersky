cmake_minimum_required(VERSION 3.10)

project(scanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest/googletest/include)

add_executable(scanner 
    src/traversal.cpp 
    src/main.cpp
    src/md5.cpp
    src/logger.cpp
)
target_include_directories(scanner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

find_package(OpenSSL REQUIRED)
target_link_libraries(scanner PRIVATE OpenSSL::Crypto)