find_package(FBX)

message("lib: ${FBX_LIBRARY}")
message("lib debug: ${FBX_LIBRARY_DEBUG}")
message("inc: ${FBX_INCLUDE_DIR}")

if (NOT FBX_FOUND)
    message(FATAL_ERROR "Couldn't find FBX SDK dependency.")
endif ()

set(FBX_INCLUDE_DIRS ${FBX_INCLUDE_DIR})

set(FBX_STATIC_LIBS 
	debug ${FBX_LIBRARY_DEBUG}
	optimized ${FBX_LIBRARY}
)