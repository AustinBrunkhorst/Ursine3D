cmake_minimum_required(VERSION 3.0)

ursine_default_project(ReflectionParser
    FOLDER "Tools/ReflectionParser"
    TYPE EXE
    DEPENDS Clang
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PCH_NAME "Precompiled"
    SYM_LINKS "Templates" 
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
)