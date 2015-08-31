# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

set(Clang_INCLUDE_DIRS ${DIR_FILE}/include)
set(Clang_STATIC_LIBS ${DIR_FILE}/lib/libclang${CMAKE_STATIC_LIBRARY_SUFFIX})
set(Clang_SHARED_LIBS ${DIR_FILE}/bin/libclang${CMAKE_SHARED_LIBRARY_SUFFIX})