cmake_minimum_required(VERSION 3.0)

project(BearKing CXX)

# ------------------------------------------------------------------------------
# Global Settings
# ------------------------------------------------------------------------------

# directory to Ursine3D
set(ENGINE_DIR "${CMAKE_SOURCE_DIR}/Ursine3D")

# directory to Ursine3D dependencies
set(ENGINE_DEP_DIR "${ENGINE_DIR}/dep")

# directory to Ursine3D resources
set(ENGINE_RESOURCES_DIR "${ENGINE_DIR}/resources")

# module directory
set(CMAKE_MODULE_PATH "${ENGINE_DIR}/cmake")

# modules

include(ParseArguments)

include(Compiler)
include(Precompiled)
include(SourceGroup)
include(DefaultProject)

# add predefined targets to "CMake" folder 
# (ex - in Visual Studio Solution Explorer)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# ------------------------------------------------------------------------------
# Global Dependencies
# ------------------------------------------------------------------------------

set(SDL2_DIR          "${ENGINE_DEP_DIR}/SDL2")
set(CEF_DIR           "${ENGINE_DEP_DIR}/CEF")
set(BENCHMARKS_DIR    "${ENGINE_DEP_DIR}/Benchmarks")
set(CLANG_DIR         "${ENGINE_DEP_DIR}/Clang")
set(BulletPhysics_DIR "${ENGINE_DEP_DIR}/Bullet")
set(WWise_DIR         "${ENGINE_DEP_DIR}/WWise")
set(FBX_DIR           "${ENGINE_DEP_DIR}/FBX")

find_package(SDL2 REQUIRED 
    HINTS ${SDL2_DIR})

find_package(CEF 3.2526 REQUIRED 
    HINTS ${CEF_DIR})

find_package(BENCHMARKS REQUIRED
    HINTS ${BENCHMARKS_DIR})

find_package(Clang REQUIRED
    HINTS ${CLANG_DIR})

find_package(BulletPhysics REQUIRED
    HINTS ${BulletPhysics_DIR})
	
find_package(FBX REQUIRED
    HINTS ${FBX_DIR})

find_package(WWise REQUIRED
    HINTS ${WWise_DIR})

# dependencies that require special attention
include(BoostSetup)

# ------------------------------------------------------------------------------
# Global Compiler Definitions
# ------------------------------------------------------------------------------

if (MSVC)
    add_definitions(
        -D_CRT_SECURE_NO_WARNINGS
        -D_SCL_SECURE_NO_WARNINGS
        # FUCK YOU MICROSOFT
        -DNOMINMAX
    )
    
    ursine_add_compile_flags(
        "/W3" # warning level 3
        "/WX" # warnings as errors
        "/Zm200" # increased compiler heap memory
        "/bigobj" # increased object file size
    )

    # DirectX requires some special attention
    include(DirectXSetup)

    # DirectX graphics backend
    add_definitions(-DURSINE_GRAPHICS_DIRECTX)

    #ursine_remove_compile_flags(
    #    "/EHsc" # disable exceptions
    #)
else ()
    ursine_add_compile_flags(
        "-std=c++11"
    )
endif ()

#add_definitions(
#    # disable exceptions
#    -D_HAS_EXCEPTIONS=0
#)

# ------------------------------------------------------------------------------
# General Installer Settings
# ------------------------------------------------------------------------------

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_VENDOR "DigiPen Institute of Technology")
set(CPACK_RESOURCE_FILE_LICENSE "${ENGINE_RESOURCES_DIR}/Installers/DigiPen_EULA.txt")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")

if (MSVC)
    if (MSVC10)
        set(VS_VERSION 10)
    elseif (MSVC11)
        set(VS_VERSION 11)
    elseif (MSVC12)
        set(VS_VERSION 12)
    elseif (MSVC14)
        set(VS_VERSION 14)
    endif()

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(vc_build x64)
    else ()
        set(vc_build x86)
    endif ()

    set(vcredist_name "vcredist${VS_VERSION}0_${vc_build}.exe")

    # Visual Studio redistributable runtime libraries
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
        ExecWait '\\\"$INSTDIR\\\\tmp\\\\${vcredist_name}\\\" /q /norestart'
        RMDir /r \\\"$INSTDIR\\\\tmp\\\"
    ")

    set(ENGINE_VCREDIST_FILE "${ENGINE_RESOURCES_DIR}/Installers/${vcredist_name}")
endif ()

# ------------------------------------------------------------------------------
# Sub Projects
# ------------------------------------------------------------------------------

# engine

add_subdirectory("Ursine3D")

# all projects to build

set(projects "")

# projects
file(GLOB_RECURSE projects_files ${CMAKE_SOURCE_DIR}/Projects/CMakeLists.txt)
set(projects ${projects} ${projects_files})

# tools
file(GLOB_RECURSE tools_files ${CMAKE_SOURCE_DIR}/Tools/CMakeLists.txt)
set(projects ${projects} ${tools_files})

foreach (project ${projects})
    get_filename_component(project_dir ${project} DIRECTORY)
    
    # ignore directories with ".ignore-project" next to the CMake config
    if (NOT EXISTS "${project_dir}/.ignore-project")
        add_subdirectory(${project_dir})
    endif ()
endforeach ()