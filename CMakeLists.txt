cmake_minimum_required(VERSION 3.16)

project(scanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/googletest) 

set(LIB_SRC
    src/traversal.cpp
    src/md5.cpp
    src/logger.cpp
    src/csv_parser.cpp
    src/utility.cpp
    src/timer.cpp
    src/thread_pool.cpp
)


set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
set(OPENSSL_INCLUDE_DIR "C:/msys64/mingw64/include")
set(OPENSSL_CRYPTO_LIBRARY "C:/msys64/mingw64/lib/libcrypto.dll.a")
set(OPENSSL_SSL_LIBRARY "C:/msys64/mingw64/lib/libssl.dll.a")
find_package(OpenSSL REQUIRED)

add_library(scanner_lib SHARED ${LIB_SRC})
target_include_directories(scanner_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(scanner_lib PUBLIC OpenSSL::Crypto)

add_library(tests_lib STATIC ${LIB_SRC})
target_include_directories(tests_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(tests_lib PRIVATE OpenSSL::Crypto)

add_executable(scanner src/main.cpp)
target_link_libraries(scanner PRIVATE scanner_lib)

option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

