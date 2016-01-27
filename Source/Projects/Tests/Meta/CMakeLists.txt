cmake_minimum_required(VERSION 3.0)

include(GlobalConfig)

add_definitions(-D_HAS_EXCEPTIONS=0)

ursine_project(MetaTest
    FOLDER "Tests"
    TYPE EXE
    DEPENDS Boost BulletPhysics WWise
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    SUBSYSTEM_RELEASE "CONSOLE"
    BUILD_META
    META_HEADER "Test.h"
    META_MODULE_HEADER "MetaTestReflectionModule.h"
    META_MODULE_DEPENDS Ursine3D
)