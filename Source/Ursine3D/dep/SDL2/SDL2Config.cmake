# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})

set(DIR_LIB ${DIR_FILE}/lib)

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

set(SDL2_INCLUDE_DIRS ${DIR_FILE}/include)

# Static Libraries

file(GLOB_RECURSE LIB_STATIC ${DIR_LIB}/*${CMAKE_STATIC_LIBRARY_SUFFIX})

set(SDL2_STATIC_LIBS ${LIB_STATIC})

# Shared Libraries

set(SDL2_SHARED_LIBS ${DIR_LIB}/*${CMAKE_SHARED_LIBRARY_SUFFIX})