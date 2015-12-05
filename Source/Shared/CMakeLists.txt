cmake_minimum_required(VERSION 3.0)

ursine_default_project(Shared
    FOLDER "Shared"
    TYPE LIB
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FBX
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

# @@@TODO: this can't be an absolute path in the installer
add_definitions(-DWORLD_ARCHETYPE_PATH="${CMAKE_SOURCE_DIR}/Projects/Retrospect/Assets/Archetypes/")
