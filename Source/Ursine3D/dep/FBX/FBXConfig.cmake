include(Platform)

# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})

set(DIR_LIB ${DIR_FILE}/lib/${PLATFORM_NAME})

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

set(FBX_INCLUDE_DIRS 
    ${DIR_FILE}/include)

# Static Libraries

set(LIB_EXT ${CMAKE_STATIC_LIBRARY_SUFFIX})
set(DLL_EXT ${CMAKE_SHARED_LIBRARY_SUFFIX})

set(FBX_STATIC_LIBS
	debug ${DIR_LIB}/Debug/libfbxsdk-md${LIB_EXT}
	optimized ${DIR_LIB}/Release/libfbxsdk-md${LIB_EXT}
)

# Shared Libraries

set(DEBUG_EXPR $<CONFIG:debug>)
set(RELEASE_EXPR $<NOT:${DEBUG_EXPR}>)
