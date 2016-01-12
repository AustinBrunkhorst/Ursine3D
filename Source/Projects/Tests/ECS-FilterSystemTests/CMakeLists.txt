cmake_minimum_required(VERSION 3.0)

ursine_default_project(
    ECS-FilterSystemTests
    FOLDER "Tests"
    TYPE EXE
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FBX
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    BUILD_META
    META_HEADER "Reflection.h"
)

add_definitions(-D_HAS_EXCEPTIONS=0)
add_definitions(-SDL_MAIN_HANDLED)
