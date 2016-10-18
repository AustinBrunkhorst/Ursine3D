# ------------------------------------------------------------------------------
# Global Settings
# ------------------------------------------------------------------------------

# directory to Ursine3D
set(ENGINE_DIR "${CMAKE_MODULE_PATH}/..")

# directory to Ursine3D dependencies
set(ENGINE_DEP_DIR "${ENGINE_DIR}/dep")

# directory to Ursine3D resources
set(ENGINE_RESOURCES_DIR "${ENGINE_DIR}/resources")

# modules

include(ParseArguments)

include(Compiler)
include(Precompiled)
include(SourceGroup)
include(UrsineProject)

# add predefined targets to "CMake" folder 
# (ex - in Visual Studio Solution Explorer)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER ".Utility/CMake")

# ------------------------------------------------------------------------------
# Global Dependencies
# ------------------------------------------------------------------------------

macro (declare_dependency DEPENDENCY)
    set(DEP_${DEPENDENCY}_DIR "${ENGINE_DEP_DIR}/${DEPENDENCY}")

    find_package(${DEPENDENCY} REQUIRED HINTS ${DEP_${DEPENDENCY}_DIR})
endmacro ()

declare_dependency(Boost)
declare_dependency(SDL2)
declare_dependency(CEF)
declare_dependency(Benchmarks)
declare_dependency(Clang)
declare_dependency(BulletPhysics)
declare_dependency(WWise)
declare_dependency(DirectX)
declare_dependency(FileWatcher)
declare_dependency(Assimp)

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
        "/Zm500" # increased compiler heap memory
        "/bigobj" # increased object file size
    )

    #ursine_remove_compile_flags(
    #    "/EHsc" # disable exceptions
    #)
else ()
    ursine_add_compile_flags(
        "-std=c++11"
    )
endif ()

add_definitions(
    # disable exceptions
    #-D_HAS_EXCEPTIONS=0
    -DURSINE_PROJECTS_DIRECTORY="${CMAKE_MODULE_PATH}/../../Projects/"
)

# ------------------------------------------------------------------------------
# General Installer Settings
# ------------------------------------------------------------------------------

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_VENDOR "DigiPen Institute of Technology")
set(CPACK_RESOURCE_FILE_LICENSE "${ENGINE_RESOURCES_DIR}/Installers/DigiPen_EULA.txt")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")

if (MSVC)
    if (MSVC10)
        set(VS_VERSION "10")
    elseif (MSVC11)
        set(VS_VERSION "11")
    elseif (MSVC12)
        set(VS_VERSION "12")
    elseif (MSVC14)
        set(VS_VERSION "14")
    endif()

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(VC_BUILD "x64")
    else ()
        set(VC_BUILD "x86")
    endif ()

    set(vcredist_name "vcredist${VS_VERSION}0_${VC_BUILD}.exe")
    set(d3dredist_name "d3dredist_${VC_BUILD}.exe")

    # Visual Studio and DirectX redistributable runtime library installation
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
        ExecWait '\\\"$INSTDIR\\\\tmp\\\\${vcredist_name}\\\" /q /norestart'
        ExecWait '\\\"$INSTDIR\\\\tmp\\\\${d3dredist_name}\\\"'
        RMDir /r \\\"$INSTDIR\\\\tmp\\\"
    ")

    set(ENGINE_VCREDIST_FILE "${ENGINE_RESOURCES_DIR}/Installers/${vcredist_name}")
    set(ENGINE_D3DREDIST_FILE "${ENGINE_RESOURCES_DIR}/Installers/${d3dredist_name}")
    set(ENGINE_D3DREDIST_HELPER_DLL "${ENGINE_RESOURCES_DIR}/Installers/D3D11InstallHelper.dll")
endif ()

# add engine
add_subdirectory(${ENGINE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/Ursine3D")