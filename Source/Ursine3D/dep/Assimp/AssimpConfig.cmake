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

set(Assimp_SHARED_LIBS
	  debug ${DIR_LIB}/Debug/assimp-vc140-mt${DLL_EXT}
	  optimized ${DIR_LIB}/Release/assimp-vc140-mt${DLL_EXT})
