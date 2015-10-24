macro (ursine_source_group_files dir_base files)
	foreach (entry ${files})
        # get the directory name from the file
        get_filename_component(group_name "${entry}" DIRECTORY)

        # skip folders containing ".ignore-group"
        if (NOT EXISTS "${group_name}/.ignore-group")
        	# get the directory relative to the base directory
        	file(RELATIVE_PATH group_name "${dir_base}" "${group_name}")

             # replace forward slashes with double back slashes
            string(REPLACE "/" "\\\\" group_name "${group_name}")

            # make sure the group name isn't empty (root include folder)
            if (NOT group_name STREQUAL "")
                source_group(${group_name} FILES "${entry}")
            endif ()
        endif ()
    endforeach ()
endmacro ()

#-------------------------------------------------------------------------------
# ursine_recursive_includes()
#
# Recursively appends all sub folders of a given folder to the given output
# variable. Folders with the file ".ignore-include" in their root are skipped.
#
# - dir_current: base target include directory (header files)
# - output_var: variable to append all includes to
#-------------------------------------------------------------------------------
macro (ursine_recursive_includes dir_current output_var)
    # /* is the only way to include folders as far as I'm aware
    file(GLOB files "${dir_current}/*")

    # skip folders containing ".ignore-include"
    if (NOT EXISTS "${dir_current}/.ignore-include")
        # append this folder to the output target
        list(APPEND ${output_var} ${dir_current})
    endif ()

    foreach (entry ${files})
        # if the entry is a directory, recursively call this function on it
        if (IS_DIRECTORY ${entry})
            ursine_recursive_includes(${entry} ${output_var})
        endif ()
    endforeach ()
endmacro ()

#-------------------------------------------------------------------------------
# ursine_parse_source_groups()
#
# Emulates the physical folder structure in the source and include directories
# with source groups (Filters in Visual Studio)
#
# - dir_inc: target includes directory (header files)
# - dir_src: target source directory  (source files)
#-------------------------------------------------------------------------------
function (ursine_parse_source_groups dir_inc dir_src)
    # header files
    file(GLOB_RECURSE files_inc ${dir_inc}/*.h ${dir_inc}/*.hpp)

    # source files
    file(GLOB_RECURSE files_src ${dir_src}/*.cpp)

    # by default, add all files to the empty group (otherwise the default
    # "Source Files" and "Header Files" group is created).
    source_group("" FILES ${files_inc} ${files_src})

    ursine_source_group_files("${dir_inc}" "${files_inc}")
    ursine_source_group_files("${dir_src}" "${files_src}")
endfunction ()

#-------------------------------------------------------------------------------
# ursine_set_folder()
#
# Adds the given target to a folder.
#
# - project_name: target project
# - folder_name: target folder name
#-------------------------------------------------------------------------------
function (ursine_set_folder project_name folder_name)
    set_property(TARGET ${project_name} PROPERTY FOLDER ${folder_name})
endfunction ()