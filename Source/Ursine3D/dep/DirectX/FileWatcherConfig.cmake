include(Platform)

# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})

set(DIR_LIB ${DIR_FILE}/lib/${PLATFORM_NAME})

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

set(FileWatcher_INCLUDE_DIRS
    ${DIR_FILE}/include)

# Static Libraries

set(LIB_EXT ${CMAKE_STATIC_LIBRARY_SUFFIX})

set(FileWatcher_STATIC_LIBS 
    debug ${DIR_LIB}/Debug/efsw${LIB_EXT}
    optimized ${DIR_LIB}/Release/efsw${LIB_EXT})
