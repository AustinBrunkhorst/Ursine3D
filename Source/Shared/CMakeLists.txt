cmake_minimum_required(VERSION 3.0)

ursine_default_project(Shared
    FOLDER "Shared"
    TYPE LIB
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    META_HEADER "Reflection.h"
    PCH_NAME "Precompiled"
)

# disable exceptions
add_definitions(-D_HAS_EXCEPTIONS=0)

# @@@TODO: this should be removed with the shared project, only temporary
add_definitions(-DURSINE_WITH_EDITOR=1)
