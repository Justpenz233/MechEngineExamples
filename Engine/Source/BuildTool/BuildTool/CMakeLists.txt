set(BUILD_TOOL_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "BUILD TOOL DIR")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_subdirectory(meta_parser)


function(get_all_include_dirs target result_var)
    # Get direct include directories
    get_target_property(directories ${target} INCLUDE_DIRECTORIES)
    if (directories)
        list(APPEND cleaned_directories ${directories})
    endif ()
    get_target_property(interface_dirs ${target} INTERFACE_INCLUDE_DIRECTORIES)
    if(interface_dirs)
        # Basic parsing: remove generator expressions for demonstration
        string(REPLACE "$<BUILD_INTERFACE:" "" cleaned_directories ${interface_dirs})
        string(REPLACE "$<INSTALL_INTERFACE:include>" "" cleaned_directories ${cleaned_directories})
        string(REPLACE "$<INSTALL_INTERFACE:" "" cleaned_directories ${cleaned_directories})
        string(REPLACE ">" ";" cleaned_directories ${cleaned_directories})
        list(APPEND all_directories ${cleaned_directories})
    endif()

    get_target_property(linked_libraries ${target} INTERFACE_LINK_LIBRARIES)

    foreach(lib ${linked_libraries})
        # Skip if it's not a target
        if(TARGET ${lib})
            # Recursive call
            get_all_include_dirs(${lib} recursive_result)
            list(APPEND all_directories ${recursive_result})
        endif()
    endforeach()

    # Remove duplicate directories
    list(REMOVE_DUPLICATES all_directories)

    # Set the result variable
    set(${result_var} "${all_directories}" PARENT_SCOPE)
endfunction()


## Precompile and generate header files
## should set path where header should place and target lists
function(PreCompile GeneratedPath Targets)
    set(BuildHeaders)
    set(INCLUDE_DIRS)
    set(GENERATED_DIR ${GeneratedPath})

    foreach(Target IN LISTS Targets)
        get_target_property(TargetHeader ${Target} PARSE_HEADERS)
        list(APPEND BuildHeaders ${TargetHeader})

        get_target_property(TargetIncludeDir ${Target} INCLUDE_DIRECTORIES)
        get_target_property(TargetLocationDir ${Target} SOURCE_DIR)

        if(TargetIncludeDir)
            list(APPEND INCLUDE_DIRS_LIST ${TargetIncludeDir})
        endif()
        if(TargetLocationDir)
            list(APPEND INCLUDE_DIRS_LIST ${TargetLocationDir})
        endif ()

        # Add all linked libraries include directories recursively
        get_all_include_dirs(${Target} recursive_include_list)
        list(APPEND INCLUDE_DIRS_LIST ${recursive_include_list})
    endforeach()
    list(REMOVE_DUPLICATES INCLUDE_DIRS_LIST)
    string(REPLACE ";" "," INCLUDE_DIRS "${INCLUDE_DIRS_LIST}")

    include(${BUILD_TOOL_DIR}/precompile/precompile.cmake)

    add_dependencies(PiccoloPreCompile PiccoloParser)
    foreach(Target ${Targets})
        add_dependencies(${Target} PiccoloPreCompile)
    endforeach()

endfunction()