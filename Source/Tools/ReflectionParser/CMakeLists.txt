# this project needs exceptions
add_definitions(-D_HAS_EXCEPTIONS=1)

ursine_project(ReflectionParser
    FOLDER "Tools/Meta"
    TYPE EXE
    DEPENDS Clang Boost
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PCH_NAME "Precompiled"
    SYM_LINKS "Templates" 
    SUBSYSTEM_DEBUG "CONSOLE"
    SUBSYSTEM_RELEASE "CONSOLE"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
)

if (MSVC)
    # cmake is gross sometimes
    set(_PROGRAM_FILES_x86 "ProgramFiles(x86)")
    set(PROGRAM_FILES_x86 "$ENV{${_PROGRAM_FILES_x86}}")
    set(SYSTEM_INCLUDES "${PROGRAM_FILES_x86}/Microsoft Visual Studio ${VS_VERSION}.0/VC/include")

    # normalize slashes
    string(REPLACE "\\" "/" SYSTEM_INCLUDES "${SYSTEM_INCLUDES}")

    # visual studio seems to have issues with escape characters in post build commands
    set_property(TARGET ReflectionParser APPEND PROPERTY COMPILE_DEFINITIONS SYSTEM_INCLUDE_DIRECTORY="${SYSTEM_INCLUDES}")
else ()
    message(FATAL_ERROR "System include directories not implemented for this compiler.")
endif ()