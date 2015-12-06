cmake_minimum_required(VERSION 3.0)

set(resources_dir "${CMAKE_CURRENT_LIST_DIR}/Resources")

set(windows_resource_file "${resources_dir}/Resources.rc")
set(install_icon "${resources_dir}/Icon.ico")

include_directories(${Shared_INCLUDE_DIRS})

# external files that will trigger regeneration of meta data
set(meta_dependencies ${Shared_HEADER_FILES})

ursine_default_project(Retrospect
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
    INSTALLER_ICON ${install_icon}
    INSTALLER_UNINSTALL_ICON ${install_icon}
    WINDOWS_RESOURCE_FILE ${windows_resource_file}
    META_HEADER "Reflection.h"
    META_DEPENDENCIES ${meta_dependencies}
    PCH_NAME "Precompiled"
)

# disable exceptions
add_definitions(-D_HAS_EXCEPTIONS=0)

# add WITH_EDITOR definition to editor and engine targets
set_target_properties(Retrospect PROPERTIES COMPILE_DEFINITIONS URSINE_WITH_EDITOR=1)

target_link_libraries(Retrospect Shared)
