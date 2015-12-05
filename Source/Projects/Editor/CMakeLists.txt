cmake_minimum_required(VERSION 3.0)

include_directories(${Shared_INCLUDE_DIRS})

# external files that will trigger regeneration of meta data
set(meta_dependencies ${Shared_HEADER_FILES})

ursine_default_project(Editor
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
    META_DEPENDENCIES ${meta_dependencies}
    PCH_NAME "Precompiled"
)
 
# disable exceptions
add_definitions(-D_HAS_EXCEPTIONS=0)

# add WITH_EDITOR definition to editor and engine targets
set_target_properties(Editor PROPERTIES COMPILE_DEFINITIONS URSINE_WITH_EDITOR=1)
set_target_properties(Ursine3D PROPERTIES COMPILE_DEFINITIONS URSINE_WITH_EDITOR=1)

target_link_libraries(Editor Shared)
