cmake_minimum_required(VERSION 3.0)

# disable exceptions
add_definitions(-D_HAS_EXCEPTIONS=0)

include(GlobalConfig)

set(RESOURCES_DIR "${CMAKE_CURRENT_LIST_DIR}/Resources")
set(WINDOWS_RESOURCE_FILE "${RESOURCES_DIR}/Resources.rc")
set(INSTALL_ICON "${RESOURCES_DIR}/Icon.ico")

add_subdirectory(../../Shared "${CMAKE_CURRENT_BINARY_DIR}/Shared")

include_directories(${Shared_INCLUDE_DIRS})

# external files that will trigger regeneration of meta data
set(META_DEPENDENCIES ${Shared_HEADER_FILES})

ursine_project(Retrospect
    FOLDER "Projects"
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
    INSTALLER_VERSION 1.0s1
    INSTALLER_ICON ${INSTALL_ICON}
    INSTALLER_UNINSTALL_ICON ${INSTALL_ICON}
    WINDOWS_RESOURCE_FILE ${WINDOWS_RESOURCE_FILE}
    META_HEADER "Reflection.h"
    META_DEPENDENCIES ${META_DEPENDENCIES}
    PCH_NAME "Precompiled"
)

target_link_libraries(Retrospect Shared)