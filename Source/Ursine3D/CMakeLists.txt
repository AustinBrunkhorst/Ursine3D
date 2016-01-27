cmake_minimum_required(VERSION 3.0)

include(MetaGeneration)

project(Ursine3D CXX)

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

set(URSINE_DIR_SOURCE src)
set(URSINE_DIR_INCLUDE include)

set(URSINE_DEPENDENCIES_INCLUDE_DIRS 
    ${SDL2_INCLUDE_DIRS}  
    ${CEF_INCLUDE_DIRS}
    ${DirectX_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${WWise_INCLUDE_DIRS}  
    ${BulletPhysics_INCLUDE_DIRS} 
    ${FBX_INCLUDE_DIRS}
)

set(URSINE_DEPENDENCIES_INCLUDE_DIRS "${URSINE_DEPENDENCIES_INCLUDE_DIRS}" PARENT_SCOPE)

# static libraries
set(URSINE_STATIC_LIBS
    ${SDL2_STATIC_LIBS}
    ${CEF_STATIC_LIBS}
    ${DirectX_STATIC_LIBS}
    ${Boost_STATIC_LIBS}
    ${BulletPhysics_LIBS}
    ${WWise_STATIC_LIBS}
    ${FBX_STATIC_LIBS}
)

set(URSINE_STATIC_LIBS "${URSINE_STATIC_LIBS}" PARENT_SCOPE)

# shared libraries
set(URSINE_SHARED_LIBS
    ${SDL2_SHARED_LIBS}
    ${CEF_SHARED_LIBS}
    ${BulletPhysics_LIBS}
)

set(URSINE_SHARED_LIBS "${URSINE_SHARED_LIBS}" PARENT_SCOPE)

# resources
set(URSINE_RESOURCES
    ${CEF_RESOURCES}
)

set(URSINE_RESOURCES "${URSINE_RESOURCES}" PARENT_SCOPE)

# ------------------------------------------------------------------------------
# Project Properties
# ------------------------------------------------------------------------------

file(GLOB_RECURSE URSINE_INC ${URSINE_DIR_INCLUDE}/*.h ${URSINE_DIR_INCLUDE}/*.hpp)
file(GLOB_RECURSE URSINE_SRC ${URSINE_DIR_SOURCE}/*.cpp)

set(URSINE_INC_NO_GENERATED ${URSINE_INC})

set(META_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/Generated")
set(META_SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/${URSINE_DIR_INCLUDE}")

ursine_prebuild_meta(
    TARGET Ursine3D
    GENERATED_DIR ${META_GENERATED_DIR}
    SOURCE_ROOT ${META_SOURCE_ROOT}
    HEADER_FILES ${URSINE_INC}
    MODULE_HEADER "UrsineReflectionModule.h"
    OUT_MODULE_SOURCE META_MODULE_SOURCE
    OUT_GENERATED_FILES META_GENERATED_FILES
    OUT_INC URSINE_INC
    OUT_SRC URSINE_SRC
    PCH_NAME "UrsinePrecompiled"
)

# disable exceptions for this project
add_definitions(-D_HAS_EXCEPTIONS=0)

add_library(Ursine3D STATIC ${URSINE_SRC} ${URSINE_INC})

ursine_recursive_includes( 
    ${CMAKE_CURRENT_SOURCE_DIR}/${URSINE_DIR_INCLUDE} 
    URSINE_INCLUDE_DIRS
)

set(URSINE_INCLUDE_DIRS "${URSINE_INCLUDE_DIRS}" PARENT_SCOPE)

ursine_add_include_directories(Ursine3D
    ${URSINE_DEPENDENCIES_INCLUDE_DIRS}
    ${URSINE_INCLUDE_DIRS}
)

ursine_set_folder(Ursine3D "Engine")

ursine_precompiled_header(Ursine3D
    UrsinePrecompiled.h
    ${URSINE_DIR_SOURCE}/UrsinePrecompiled.cpp
)

ursine_parse_source_groups(
    ${CMAKE_CURRENT_SOURCE_DIR}/${URSINE_DIR_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR}/${URSINE_DIR_SOURCE}
)

# ------------------------------------------------------------------------------
# Graphics Configuration
# ------------------------------------------------------------------------------

# DirectX
if (MSVC)
    # treat warnings as errors
    target_compile_options(Ursine3D PUBLIC /WX)

    # multi process compilation
    target_compile_options(Ursine3D PUBLIC /MP)

    # ignore linker warnings for no symbols or pdbs
    set_property(TARGET Ursine3D APPEND PROPERTY STATIC_LIBRARY_FLAGS "/ignore:4099 /ignore:4221")
    
    set(URSINE_SHADER_DIR "${ENGINE_RESOURCES_DIR}/Shaders")

    file(GLOB_RECURSE URSINE_SHADER_FILES "${URSINE_SHADER_DIR}/*.hlsl")

    foreach (shader ${URSINE_SHADER_FILES})
        get_filename_component(filename "${shader}" NAME_WE)

        string(LENGTH ${filename} filename_length)
        math(EXPR filename_length "${filename_length} - 2")

        # filename suffix
        string(SUBSTRING ${filename} ${filename_length} -1 shader_type)

        # type of shader to compile (vs, ps, gs, etc.)
        string(TOLOWER ${shader_type} shader_type)

        # default to vertex shader
        set(vs_shader_type "Vertex")

        # map to correct Visual Studio FXCompiler shader type

        if (shader_type STREQUAL "ps")
            set(vs_shader_type "Pixel")
        elseif (shader_type STREQUAL "gs")
            set(vs_shader_type "Geometry")
        elseif (shader_type STREQUAL "hs")
            set(vs_shader_type "Hull")
        elseif (shader_type STREQUAL "ds")
            set(vs_shader_type "Domain")
        elseif (shader_type STREQUAL "cs")
            set(vs_shader_type "Compute")
        endif ()

        # set the properties in Visual Studio
        set_source_files_properties(
            TARGET Shaders
            "${shader}"
            PROPERTIES
            VS_SHADER_TYPE ${vs_shader_type}
            VS_SHADER_MODEL 5.0
            VS_SHADER_ENTRYPOINT main
        )
    endforeach ()

    # build the source group folders for the shader files
    ursine_source_group_files(
        "${URSINE_SHADER_DIR}" 
        "${URSINE_SHADER_FILES}"
    )

    set(shader_out_dir "${CMAKE_CURRENT_BINARY_DIR}/Shaders/")

    # create an empty dummy file
    set(shader_dummy_file "${shader_out_dir}/Shaders.cpp")

    file(WRITE "${shader_dummy_file}" "")

    # exclude it from the build
    set_source_files_properties(
        TARGET Shaders
        "${shader_dummy_file}"
        PROPERTIES
        HEADER_FILE_ONLY TRUE
    )

    source_group("" FILES "${shader_dummy_file}")

    # create a target with the shader files (and dummy file)
    add_library(Shaders
        ${shader_dummy_file}
        ${URSINE_SHADER_FILES}
    )

    # give it a better output directory
    set_target_properties(Shaders 
        PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY "${shader_out_dir}"
    )

    ursine_set_folder(Shaders "Engine")

    # export to parent scope
    set(URSINE_SHADER_FILES "${URSINE_SHADER_FILES}" PARENT_SCOPE)
endif ()

# ------------------------------------------------------------------------------
# Exported Properties
# ------------------------------------------------------------------------------

set(URSINE_DIR_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${URSINE_DIR_INCLUDE}" PARENT_SCOPE)
set(URSINE_DIR_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/${URSINE_DIR_SOURCE}" PARENT_SCOPE)
set(URSINE_HEADER_FILES "${URSINE_INC}" PARENT_SCOPE)
set(URSINE_HEADER_FILES_NO_GENERATED "${URSINE_INC_NO_GENERATED}" PARENT_SCOPE)

set(URSINE_META_GENERATED_DIR "${META_GENERATED_DIR}" PARENT_SCOPE)
set(URSINE_META_SOURCE_ROOT "${META_SOURCE_ROOT}" PARENT_SCOPE)
set(URSINE_META_MODULE_SOURCE "${META_MODULE_SOURCE}" PARENT_SCOPE)
set(URSINE_META_GENERATED_FILES "${META_GENERATED_FILES}" PARENT_SCOPE)

# add reflection parser
add_subdirectory("${ENGINE_DIR}/../Tools/ReflectionParser" "${CMAKE_BINARY_DIR}/ReflectionParser")

# build reflection for the engine
ursine_build_meta(
    TARGET Ursine3D
    SOURCE_ROOT ${META_SOURCE_ROOT}
    GENERATED_DIR ${META_GENERATED_DIR}
    GENERATED_FILES ${META_GENERATED_FILES}
    SOURCE_FILE "UrsineReflection.h"
    MODULE_HEADER "UrsineReflectionModule.h"
    MODULE_SOURCE_FILE ${META_MODULE_SOURCE}
    HEADER_FILES ${URSINE_INC_NO_GENERATED}
    PCH_NAME "UrsinePrecompiled"
)