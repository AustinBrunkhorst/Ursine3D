cmake_minimum_required(VERSION 3.0)

ursine_default_project(Graphics
    FOLDER "Tests"
    TYPE EXE
    DEPENDS SDL2 CEF DirectX
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    SYM_LINKS "Assets" "Models" "SHADER_BINARY" "Textures"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    BUILD_META
    META_HEADER "Reflection.h"
    PCH_NAME "Precompiled"
)

# disable exceptions
add_definitions(-D_HAS_EXCEPTIONS=0)