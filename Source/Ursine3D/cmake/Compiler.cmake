function (ursine_add_compile_flags)
    foreach (flag ${ARGN})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
    endforeach ()
endfunction ()

function (ursine_remove_compile_flags)
    foreach (flag ${ARGN})
        string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endforeach ()
    
    # update in parent scope
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
endfunction ()

function (ursine_add_linker_flags)
    foreach (flag ${ARGN})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${flag}" PARENT_SCOPE)
    endforeach ()
endfunction ()

function (ursine_add_include_directories target_name)
    get_target_property(inc_current ${target_name} INCLUDE_DIRECTORIES)
    
    # clear if not found
    if (inc_current STREQUAL "inc_current-NOTFOUND")
    	set(inc_current "")
    endif ()

    # append semi-colon
    set(inc_current "${inc_current};")
    
    set_target_properties(${target_name} PROPERTIES INCLUDE_DIRECTORIES "${inc_current}${ARGN}")
endfunction ()