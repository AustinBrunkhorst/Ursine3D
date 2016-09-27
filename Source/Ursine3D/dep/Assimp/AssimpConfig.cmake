include(Platform)

# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})

set(DIR_LIB ${DIR_FILE}/lib/${PLATFORM_NAME})

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

set(Assimp_INCLUDE_DIRS 
    ${DIR_FILE}/include)

# Static Libraries

set(DLL_EXT ${CMAKE_SHARED_LIBRARY_SUFFIX})

set(DEBUG_EXPR $<CONFIG:debug>)
set(RELEASE_EXPR $<NOT:${DEBUG_EXPR}>)

set(Assimp_SHARED_LIBS 
  "${DIR_LIB}/$<${DEBUG_EXPR}:Debug>$<${RELEASE_EXPR}:Release>/*${DLL_EXT}"
)

set(Assimp_SHARED_LIBS_Debug 
  "${DIR_LIB}/Debug/*${DLL_EXT}"
)

set(Assimp_SHARED_LIBS_Release 
  "${DIR_LIB}/Release/*${DLL_EXT}"
)