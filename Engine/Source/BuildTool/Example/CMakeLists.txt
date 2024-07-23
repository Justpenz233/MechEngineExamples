cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "" FORCE)
set(GENERATED_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

project(Example)

# link all source files
file(GLOB_RECURSE ENGINE_SOURCE_FILES *.cpp *.ipp)
list(REMOVE_ITEM ENGINE_SOURCE_FILES "main.cpp")
file(GLOB_RECURSE ENGINE_HEADER_FILES *.h)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ENGINE_HEADER_FILES} ${ENGINE_SOURCE_FILES})

# create library
add_library(TestLib SHARED ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})

target_include_directories(TestLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(TestLib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

# --------- Reflection setup ------------

# IMPORTANT: Set PARSE_HEADERS property to precompiler to read
set_target_properties(TestLib PROPERTIES PARSE_HEADERS "${ENGINE_HEADER_FILES}")

# Add library to precompile
# IMPORTANT: Set GeneratedPath to this dir
PreCompile("${CMAKE_CURRENT_SOURCE_DIR}" TestLib)

# ---------------------------------------

# create executable
add_executable(TestExe main.cpp)
target_link_libraries(TestExe PUBLIC TestLib)