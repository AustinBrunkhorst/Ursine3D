cmake_minimum_required(VERSION 3.0)

include(UrsineProject)

ursine_project(Meta
    FOLDER "Tests"
    TYPE EXE
    DEPENDS Boost
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    SUBSYSTEM_RELEASE "CONSOLE"
    BUILD_META
    META_HEADER "Test.h"
)