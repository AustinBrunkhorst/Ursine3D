set(_boost_root "${ENGINE_DEP_DIR}/Boost")
set(BOOST_INCLUDEDIR "${_boost_root}/include")
set(BOOST_LIBRARYDIR "${_boost_root}/lib")

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_USE_MULTITHREADED ON)
set(BOOST_USE_STATIC_RUNTIME ON)
set(BOOST_NO_SYSTEM_PATHS ON)
set(BOOST_ALL_NO_LIB ON)

# disable auto-linking
add_definitions(-DBOOST_ALL_NO_LIB)

find_package(Boost 1.59.0 REQUIRED 
	COMPONENTS 
	system
	filesystem
	program_options
	regex
)

# use the variable format for ursine_project( ) dependency info
set(Boost_STATIC_LIBS ${Boost_LIBRARIES})
