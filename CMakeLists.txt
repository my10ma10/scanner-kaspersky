cmake_minimum_required(VERSION 3.10)

project(scanner)

set(LIB_SRC
    src/traversal.cpp
    src/md5.cpp
    src/logger.cpp
    src/csv_parser.cpp
    src/utility.cpp
    src/timer.cpp
    src/thread_pool.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(scanner_lib SHARED 
    ${LIB_SRC}
)

add_executable(scanner 
    src/main.cpp
)
target_include_directories(scanner_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

find_package(OpenSSL REQUIRED)
target_link_libraries(scanner PRIVATE scanner_lib OpenSSL::Crypto)