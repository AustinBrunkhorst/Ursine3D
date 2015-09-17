cmake_minimum_required(VERSION 3.0)

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
    ${BulletPhysics_INCLUDE_DIRS}
)
 
set(URSINE_DEPENDENCIES_INCLUDE_DIRS "${URSINE_DEPENDENCIES_INCLUDE_DIRS}" PARENT_SCOPE)

# static libraries
set(URSINE_STATIC_LIBS
    ${SDL2_STATIC_LIBS}
    ${CEF_STATIC_LIBS}
    ${DirectX_STATIC_LIBS}
    ${Boost_STATIC_LIBS}
    ${BulletPhysics_LIBS}
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

file(GLOB_RECURSE URSINE_SRC ${URSINE_DIR_SOURCE}/*.cpp)
file(GLOB_RECURSE URSINE_INC ${URSINE_DIR_INCLUDE}/*.h ${URSINE_DIR_INCLUDE}/*.hpp)

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

set(URSINE_DIR_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${URSINE_DIR_INCLUDE}" PARENT_SCOPE)
set(URSINE_DIR_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/${URSINE_DIR_SOURCE}" PARENT_SCOPE)