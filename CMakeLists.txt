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

add_library(scanner_lib SHARED ${LIB_SRC})

add_executable(scanner src/main.cpp)

target_include_directories(scanner_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_compile_definitions(scanner_lib PRIVATE BUILD_MYLIB)

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
set(OPENSSL_INCLUDE_DIR "C:/msys64/mingw64/include")
set(OPENSSL_CRYPTO_LIBRARY "C:/msys64/mingw64/lib/libcrypto.dll.a")
set(OPENSSL_SSL_LIBRARY "C:/msys64/mingw64/lib/libssl.dll.a")

find_package(OpenSSL REQUIRED)
target_link_libraries(scanner PRIVATE scanner_lib OpenSSL::Crypto)
target_link_libraries(scanner_lib PRIVATE OpenSSL::Crypto)
# target_link_libraries(scanner_lib PRIVATE crypto)