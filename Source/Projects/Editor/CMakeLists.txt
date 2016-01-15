cmake_minimum_required(VERSION 3.0)

add_definitions(
    # building with editor
    -DURSINE_WITH_EDITOR
    # disable exceptions
    -D_HAS_EXCEPTIONS=0
)

include(UrsineProject)

ursine_project(Editor
    FOLDER "Engine"
    TYPE EXE
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FBX
    COPY_SHADERS "Assets/Shaders/"
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    SYM_LINKS "Assets"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    BUILD_META
    INCLUDE_INSTALLER
    INSTALLER_VERSION "0.1submission"
    META_HEADER "Reflection.h"
    META_DEPENDENCIES ${META_DEPENDENCIES}
    PCH_NAME "Precompiled"
)

add_subdirectory(../../Shared "${CMAKE_CURRENT_BINARY_DIR}/Shared")

include_directories(${Shared_INCLUDE_DIRS})

# external files that will trigger regeneration of meta data
set(META_DEPENDENCIES ${Shared_HEADER_FILES})

target_link_libraries(Editor Shared)