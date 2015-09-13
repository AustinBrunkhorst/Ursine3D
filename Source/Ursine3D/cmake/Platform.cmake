if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(arch_suffix "64")
else ()
	set(arch_suffix "32")
endif ()

if (WIN32)
	set(PLATFORM_NAME "Win${arch_suffix}")
endif ()