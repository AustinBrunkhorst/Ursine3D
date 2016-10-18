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

set(GAME_LOGIC_TARGET "CS460" CACHE STRING "Name of the target used for game specific code.")
set(GAME_LOGIC_PATH "../../Shared/CS460" CACHE PATH "Source directory of the game specific code.")

add_subdirectory(${GAME_LOGIC_PATH} "${CMAKE_CURRENT_BINARY_DIR}/${GAME_LOGIC_TARGET}")
 
include_directories(${${GAME_LOGIC_TARGET}_INCLUDE_DIRS})

ursine_project(Editor
    FOLDER "Engine"
    TYPE EXE
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FileWatcher Assimp 
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
    META_MODULE_DEPENDS Ursine3D ${GAME_LOGIC_TARGET}
    PCH_NAME "Precompiled"
)

target_link_libraries(Editor ${GAME_LOGIC_TARGET})

add_custom_target(SyncAddedFiles
    COMMAND ${CMAKE_COMMAND} -E touch \"${Editor_LIST_FILE}\"
    COMMAND ${CMAKE_COMMAND} -E touch \"${${GAME_LOGIC_TARGET}_LIST_FILE}\"
    COMMAND ${CMAKE_COMMAND} -E touch \"${URSINE_LIST_FILE}\"
)

add_custom_target(MagicRebuild
    DEPENDS SyncAddedFiles
    DEPENDS Editor-RebuildMeta
    DEPENDS ${GAME_LOGIC_TARGET}-RebuildMeta
    DEPENDS Ursine3D-RebuildMeta
)

ursine_set_folder(SyncAddedFiles ".Utility")
ursine_set_folder(MagicRebuild ".Utility")
