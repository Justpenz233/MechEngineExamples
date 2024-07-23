cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(CPPReflection)
add_subdirectory(BuildTool)

## build test
IF (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(Example)
ENDIF (PROJECT_IS_TOP_LEVEL)