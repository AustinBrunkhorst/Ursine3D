cmake_minimum_required(VERSION 3.0)

ursine_default_project(
    Audio-Basic
    FOLDER "Tests"
    TYPE EXE
    DEPENDS WWise
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
)