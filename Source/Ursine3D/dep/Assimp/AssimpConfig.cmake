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

set(LIB_EXT ${CMAKE_STATIC_LIBRARY_SUFFIX})

set(Assimp_STATIC_LIBS
  debug ${DIR_LIB}/Debug/assimp-vc140-mt${LIB_EXT}
  debug ${DIR_LIB}/Debug/zlibstaticicd${LIB_EXT}
  optimized ${DIR_LIB}/Release/assimp-vc140-mt${LIB_EXT}
  optimized ${DIR_LIB}/Release/zlibstatic${LIB_EXT})