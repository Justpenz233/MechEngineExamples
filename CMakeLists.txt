cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(ExampleMain)

set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Global definitions based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DME_BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DME_BUILD_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DME_BUILD_RELWITHDEBINFO)
endif()

add_subdirectory(Examples)
add_subdirectory(Engine)


add_executable(ExampleMain main.cpp)
target_link_libraries(ExampleMain PUBLIC MechEngineEditor)
target_compile_definitions(ExampleMain PRIVATE "PROJECT_DIR=\"${USER_PROJECT_ROOT_DIR}\"")
