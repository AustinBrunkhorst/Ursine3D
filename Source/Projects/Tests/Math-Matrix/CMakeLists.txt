cmake_minimum_required(VERSION 3.0)

ursine_default_project(
    Math-MatrixTests
    FOLDER "Tests"
    TYPE EXE
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
)

add_definitions(-D_HAS_EXCEPTIONS=0)
add_definitions(-SDL_MAIN_HANDLED)
