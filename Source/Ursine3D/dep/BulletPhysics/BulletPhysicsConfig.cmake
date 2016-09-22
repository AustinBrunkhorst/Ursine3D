include(Platform)

# ------------------------------------------------------------------------------
# Version Configuration
# ------------------------------------------------------------------------------

set(BulletPhysics_VERSION 2.83)

# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})

set(DIR_LIB ${DIR_FILE}/lib/${PLATFORM_NAME})

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

set(BulletPhysics_INCLUDE_DIRS
    ${DIR_FILE}/include)

# Static Libraries

set(LIB_EXT ${CMAKE_STATIC_LIBRARY_SUFFIX})

set(BulletPhysics_STATIC_LIBS 
    debug ${DIR_LIB}/Debug/BulletCollision${LIB_EXT}
    debug ${DIR_LIB}/Debug/BulletDynamics${LIB_EXT}
    debug ${DIR_LIB}/Debug/BulletSoftBody${LIB_EXT}
    debug ${DIR_LIB}/Debug/LinearMath${LIB_EXT}
    debug ${DIR_LIB}/Debug/BulletWorldImporter${LIB_EXT}
    debug ${DIR_LIB}/Debug/BulletFileLoader${LIB_EXT}
    debug ${DIR_LIB}/Debug/HACD${LIB_EXT}
    optimized ${DIR_LIB}/Release/BulletCollision${LIB_EXT}
    optimized ${DIR_LIB}/Release/BulletDynamics${LIB_EXT}
    optimized ${DIR_LIB}/Release/BulletSoftBody${LIB_EXT}
    optimized ${DIR_LIB}/Release/LinearMath${LIB_EXT}
    optimized ${DIR_LIB}/Release/BulletWorldImporter${LIB_EXT}
    optimized ${DIR_LIB}/Release/BulletFileLoader${LIB_EXT}
    optimized ${DIR_LIB}/Release/HACD${LIB_EXT}
)
