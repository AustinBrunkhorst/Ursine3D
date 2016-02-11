# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})
set(DIR_LIB ${DIR_FILE}/lib)

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

set(WWise_INCLUDE_DIRS ${DIR_FILE}/include)

set(LIB_EXT ${CMAKE_STATIC_LIBRARY_SUFFIX})

set(WWise_STATIC_LIBS 
    debug ${DIR_LIB}/Debug/AkMemoryMgr${LIB_EXT}
    debug ${DIR_LIB}/Debug/AkMusicEngine${LIB_EXT}
    debug ${DIR_LIB}/Debug/AkSoundEngine${LIB_EXT}
    debug ${DIR_LIB}/Debug/AkStreamMgr${LIB_EXT}
    debug ${DIR_LIB}/Debug/CommunicationCentral${LIB_EXT}
    optimized ${DIR_LIB}/Release/AkMemoryMgr${LIB_EXT}
    optimized ${DIR_LIB}/Release/AkMusicEngine${LIB_EXT}
    optimized ${DIR_LIB}/Release/AkSoundEngine${LIB_EXT}
    optimized ${DIR_LIB}/Release/AkStreamMgr${LIB_EXT})

set(WWise_SHARED_LIBS "")