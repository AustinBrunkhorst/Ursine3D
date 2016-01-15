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