cmake_minimum_required(VERSION 3.0)

ursine_default_project(ReflectionParser
    FOLDER "Tools/ReflectionParser"
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