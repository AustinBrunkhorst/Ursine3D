include(Platform)

# ------------------------------------------------------------------------------
# Version Configuration
# ------------------------------------------------------------------------------

set(CEF_CHROMIUM_VERSION 33.0.1750.170)

# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})

set(DIR_LIB ${DIR_FILE}/lib/${PLATFORM_NAME})
set(DIR_RESOURCES ${DIR_FILE}/resources)

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

set(CEF_INCLUDE_DIRS
    ${DIR_FILE} # CEF is weird, and uses include/file
    ${DIR_FILE}/include)

# Static Libraries

set(LIB_EXT ${CMAKE_STATIC_LIBRARY_SUFFIX})

set(CEF_STATIC_LIBS 
	debug ${DIR_LIB}/Debug/libcef${LIB_EXT}
	debug ${DIR_LIB}/Debug/libcef_dll_wrapper${LIB_EXT}
	optimized ${DIR_LIB}/Release/libcef${LIB_EXT}
	optimized ${DIR_LIB}/Release/libcef_dll_wrapper${LIB_EXT})

# Shared Libraries

set(DEBUG_EXPR $<CONFIG:debug>)
set(RELEASE_EXPR $<NOT:${DEBUG_EXPR}>)

set(CEF_SHARED_LIBS "${DIR_LIB}/$<${DEBUG_EXPR}:Debug>$<${RELEASE_EXPR}:Release>/*${CMAKE_SHARED_LIBRARY_SUFFIX}")

set(CEF_SHARED_LIBS_Debug "${DIR_LIB}/Debug/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
set(CEF_SHARED_LIBS_Release "${DIR_LIB}/Release/*${CMAKE_SHARED_LIBRARY_SUFFIX}")

# Resources

set(CEF_RESOURCES ${DIR_RESOURCES}/*)