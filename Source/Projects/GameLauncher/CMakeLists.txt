cmake_minimum_required(VERSION 3.0)

# disable exceptions
add_definitions(-D_HAS_EXCEPTIONS=0)

include(GlobalConfig)

add_subdirectory(../../Shared "${CMAKE_CURRENT_BINARY_DIR}/Shared")

include_directories(${Shared_INCLUDE_DIRS})

ursine_project(GameLauncher
    FOLDER "Projects"
    TYPE EXE
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FBX FileWatcher
    COPY_SHADERS "Resources/Shaders/"
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    SYM_LINKS "Resources"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    WINDOWS_RESOURCE_FILE "BuildResources/Resources.rc"
    BUILD_META
    META_HEADER "Reflection.h"
    META_MODULE_HEADER "GameLauncherReflectionModule.h"
    META_MODULE_DEPENDS Ursine3D Shared
    PCH_NAME "Precompiled"
)

target_link_libraries(GameLauncher Shared)

add_custom_target(SyncAddedFiles
    COMMAND ${CMAKE_COMMAND} -E touch \"${GameLauncher_LIST_FILE}\"
    COMMAND ${CMAKE_COMMAND} -E touch \"${Shared_LIST_FILE}\"
    COMMAND ${CMAKE_COMMAND} -E touch \"${URSINE_LIST_FILE}\"
)

add_custom_target(MagicRebuild
    DEPENDS SyncAddedFiles
    DEPENDS GameLauncher-RebuildMeta
    DEPENDS Shared-RebuildMeta
    DEPENDS Ursine3D-RebuildMeta
)

ursine_set_folder(SyncAddedFiles ".Utility")
ursine_set_folder(MagicRebuild ".Utility")

set_target_properties(SyncAddedFiles PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
set_target_properties(MagicRebuild PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
