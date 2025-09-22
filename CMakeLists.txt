cmake_minimum_required(VERSION 3.10)

project(scanner)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest/googletest/include)

add_executable(scanner 
    src/traversal.cpp 
    src/main.cpp
)
target_include_directories(scanner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)