include(Platform)

# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})

set(DIR_LIB ${DIR_FILE}/lib/${PLATFORM_NAME})

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

set(GLEW_INCLUDE_DIRS ${DIR_FILE}/include)

# Static Libraries

set(GLEW_STATIC_LIBS 
	# TODO: 32 and 64 bit
	opengl32${CMAKE_STATIC_LIBRARY_SUFFIX}
	${DIR_LIB}/glew32${CMAKE_STATIC_LIBRARY_SUFFIX})

# Shared Libraries

set(GLEW_SHARED_LIBS ${DIR_LIB}/*${CMAKE_SHARED_LIBRARY_SUFFIX})