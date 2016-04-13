cmake_minimum_required(VERSION 3.0)

set(EDITOR_VERSION "0.1alpha")

add_definitions(
    # building with editor
    -DURSINE_WITH_EDITOR
    # always output warnings
    -DURSINE_OUTPUT_WARNINGS=1
    # always output to console
    -DURSINE_OUTPUT_CONSOLE=1
    -DEDITOR_VERSION="${EDITOR_VERSION}"
    # disable exceptions
    -D_HAS_EXCEPTIONS=0
)

# make sure we're building with editor
set(GLOBAL_META_DEFINES "URSINE_WITH_EDITOR")

include(GlobalConfig)

add_subdirectory(../../Shared "${CMAKE_CURRENT_BINARY_DIR}/Shared")
 
include_directories(${Shared_INCLUDE_DIRS})

ursine_project(Editor
    FOLDER "Engine"
    TYPE EXE
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FBX FileWatcher
    COPY_SHADERS "Assets/Shaders/"
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    SYM_LINKS "Assets" "Resources" "Tools"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    BUILD_META
    INCLUDE_INSTALLER
    INSTALLER_VERSION ${EDITOR_VERSION}
    META_HEADER "EditorReflection.h"
    META_MODULE_HEADER "EditorReflectionModule.h"
    META_MODULE_DEPENDS Ursine3D Shared
    PCH_NAME "Precompiled"
)

target_link_libraries(Editor Shared)

add_custom_target(SyncAddedFiles
    COMMAND ${CMAKE_COMMAND} -E touch \"${Editor_LIST_FILE}\"
    COMMAND ${CMAKE_COMMAND} -E touch \"${Shared_LIST_FILE}\"
    COMMAND ${CMAKE_COMMAND} -E touch \"${URSINE_LIST_FILE}\"
)

add_custom_target(MagicRebuild
    DEPENDS SyncAddedFiles
    DEPENDS Editor-RebuildMeta
    DEPENDS Shared-RebuildMeta
    DEPENDS Ursine3D-RebuildMeta
)

ursine_set_folder(SyncAddedFiles ".Utility")
ursine_set_folder(MagicRebuild ".Utility")

set_target_properties(SyncAddedFiles PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)
set_target_properties(MagicRebuild PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)
