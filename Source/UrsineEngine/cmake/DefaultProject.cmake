macro (_install_expand_files expr)
    file(GLOB_RECURSE expr_expanded ${expr})

    get_filename_component(base_dir ${expr} DIRECTORY)

    foreach (expanded ${expr_expanded})
        get_filename_component(expanded_dir ${expanded} DIRECTORY)

        file(RELATIVE_PATH relative_dir "${base_dir}" "${expanded_dir}")

        install(FILES ${expanded} DESTINATION "bin/${relative_dir}" ${ARGN})
    endforeach ()
endmacro ()

macro (ursine_default_project project_name)
    ursine_parse_arguments(PROJ 
        "FOLDER;TYPE;SOURCE_DIR;INCLUDE_DIR;DEPENDS;PCH_NAME;SYM_LINKS;INSTALLER_VERSION;INSTALLER_SUMMARY;INSTALLER_DISPLAY_NAME;WINDOWS_RESOURCE_FILE;INSTALLER_ICON;INSTALLER_UNINSTALL_ICON;SUBSYSTEM_DEBUG;SUBSYSTEM_RELEASE;META_HEADER" 
        "NO_ENGINE;PARSE_SOURCE_GROUPS;RECURSIVE_INCLUDES;INCLUDE_INSTALLER;BUILD_META" 
        ${ARGN})
    
    project(${project_name} CXX)

    set(build_dir "${CMAKE_CURRENT_BINARY_DIR}")
    
    file(GLOB_RECURSE files_src ${PROJ_SOURCE_DIR}/*.cpp)
    file(GLOB_RECURSE files_inc ${PROJ_INCLUDE_DIR}/*.h ${PROJ_INCLUDE_DIR}/*.hpp)

    if ("${PROJ_BUILD_META}" STREQUAL "TRUE")
        set(meta_generated_header "${build_dir}/Meta.Generated.h")
        set(meta_generated_src "${build_dir}/Meta.Generated.cpp")

        # add to the sources
        list(APPEND files_src ${meta_generated_src})
        list(APPEND files_inc ${meta_generated_header})

        # create temporary empty files to suppress compilation errors
        file(WRITE ${meta_generated_header} "")
        file(WRITE ${meta_generated_src} "")
    endif ()

    set(files_misc "")

    if ("${PROJ_TYPE}" STREQUAL "EXE")
        set(post_build_commands "")

        if (MSVC)
            # if the resources file is set, include in the source
            if (NOT "${PROJ_WINDOWS_RESOURCE_FILE}" STREQUAL "")
                list(APPEND files_misc "${PROJ_WINDOWS_RESOURCE_FILE}")
            endif ()
        endif ()

        # executable
        add_executable(${project_name} ${files_inc} ${files_src} ${files_misc})

        if (MSVC)
            # if the resources file is set, add to resources source group
            if (NOT "${PROJ_WINDOWS_RESOURCE_FILE}" STREQUAL "")
                source_group("Resources" FILES "${PROJ_WINDOWS_RESOURCE_FILE}")
            endif ()
        endif ()

        foreach (dependency ${PROJ_DEPENDS})
            # copy shared libraries
            foreach (shared_lib ${${dependency}_SHARED_LIBS})
                string(REPLACE "/" "\\" shared_lib_escaped "${shared_lib}")
        
                list(APPEND post_build_commands
                    COMMAND 
                    xcopy \"${shared_lib_escaped}\" \"$<TARGET_FILE_DIR:${project_name}>\" /Y /d
                )

                # install shared lib if applicable
                if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
                    _install_expand_files(${shared_lib} COMPONENT ${project_name})
                endif ()
            endforeach ()

            # installer specific shared libs if applicable
            if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
                set(lib_configs "Debug;Release")

                foreach (config ${lib_configs})
                    foreach (shared_lib ${${dependency}_SHARED_LIBS_${config}})
                        _install_expand_files(${shared_lib} COMPONENT ${project_name} CONFIGURATIONS ${config})
                    endforeach ()
                endforeach ()
            endif ()

            # copy resources
            foreach (resource ${${dependency}_RESOURCES})
                string(REPLACE "/" "\\" resource_escaped "${resource}")
        
                list(APPEND post_build_commands
                    COMMAND 
                    xcopy \"${resource_escaped}\" \"$<TARGET_FILE_DIR:${project_name}>\" /Y /e /d
                )

                # install resource if applicable
                if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
                    get_filename_component(base_dir ${resource} DIRECTORY)

                    install(DIRECTORY "${base_dir}/" DESTINATION "bin" COMPONENT ${project_name} PATTERN ${resource})
                endif ()
            endforeach ()

            set(dependency_libs ${dependency}_STATIC_LIBS)

            # static libs if defined
            if (DEFINED ${dependency_libs})
                target_link_libraries(${project_name}
                    ${${dependency_libs}}
                )
            endif ()
        endforeach ()

        if (NOT "${PROJ_SYM_LINKS}" STREQUAL "")
            # create symbolic links to folders
            foreach (link ${PROJ_SYM_LINKS})
                list(APPEND post_build_commands
                    COMMAND 
                    "${CMAKE_MODULE_PATH}/Scripts/SymLink.bat" 
                    \"$<TARGET_FILE_DIR:${project_name}>/${link}\" \"${CMAKE_CURRENT_SOURCE_DIR}/${link}\"
                )

                # install folder if applicable
                if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
                    install(DIRECTORY ${link} DESTINATION "bin" COMPONENT ${project_name})
                endif ()
            endforeach ()
        endif ()
        
        # add post build command if it isn't empty
        if (NOT "${post_build_commands}" STREQUAL "")
            add_custom_command(TARGET ${project_name} POST_BUILD
                ${post_build_commands}
                COMMENT "Libs, Resources, Symbolic links"
            )
        endif ()

        # installer configuration
        if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
            install(TARGETS ${project_name} DESTINATION "bin" COMPONENT ${project_name})

            if (MSVC)
                install(PROGRAMS ${ENGINE_VCREDIST_FILE} DESTINATION "tmp" COMPONENT ${project_name})
            endif ()

            set(expr_debug $<OR:$<CONFIG:debug>,$<CONFIG:relwithdebinfo>>)
            set(expr_release $<NOT:${expr_debug}>)

            set(installer_target "${project_name}-Installer")

            add_custom_target(${installer_target}
                COMMAND cd ${CMAKE_CURRENT_BINARY_DIR} && cpack -C $<${expr_debug}:Debug>$<${expr_release}:Release>
                DEPENDS ${project_name})

            # project folder
            if (NOT "${PROJ_FOLDER}" STREQUAL "")
                ursine_set_folder(${installer_target} ${PROJ_FOLDER})
            endif ()

            if ("${PROJ_INSTALLER_DISPLAY_NAME}" STREQUAL "")
                set(PROJ_INSTALLER_DISPLAY_NAME ${project_name})
            endif ()

            if ("${PROJ_INSTALLER_VERSION}" STREQUAL "")
                set(start_menu_name "${PROJ_INSTALLER_DISPLAY_NAME}")

                set(PROJ_INSTALLER_VERSION "1.0.0")

                set(install_directory_suffix "")
            else ()
                set(install_directory_suffix "\\\\${PROJ_INSTALLER_VERSION}")

                set(start_menu_name "${PROJ_INSTALLER_DISPLAY_NAME} \(${PROJ_INSTALLER_VERSION}\)")
            endif ()

            set(CPACK_NSIS_MODIFY_PATH "ON")

            set(CPACK_PACKAGE_EXECUTABLES "${project_name}" "${start_menu_name}")
            set(CPACK_INSTALL_CMAKE_PROJECTS ${CMAKE_CURRENT_BINARY_DIR} ${project_name} ${project_name} /)
            set(CPACK_PACKAGE_NAME ${project_name})
            set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJ_INSTALLER_SUMMARY})
            set(CPACK_NSIS_DISPLAY_NAME ${PROJ_INSTALLER_DISPLAY_NAME})
            set(CPACK_PACKAGE_VERSION ${PROJ_INSTALLER_VERSION})

            # custom start menu define
            set(CPACK_NSIS_DEFINES "${CPACK_NSIS_DEFINES}
                !define MUI_STARTMENUPAGE_DEFAULTFOLDER \\\"DigiPen\\\\${start_menu_name}\\\"
            ")

            set(CPACK_PACKAGE_INSTALL_DIRECTORY "DigiPen\\\\${project_name}${install_directory_suffix}")
            set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${project_name}${PROJ_INSTALLER_VERSION})

            set(CPACK_NSIS_MUI_ICON "${PROJ_INSTALLER_ICON}")
            set(CPACK_NSIS_MUI_UNIICON "${PROJ_INSTALLER_UNINSTALL_ICON}")

            set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/${project_name}.exe")
            set(CPACK_NSIS_MUI_FINISHPAGE_RUN "${project_name}.exe")

            # manually create a desktop link if the option was set
            set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                \\\${If} $INSTALL_DESKTOP == 1
                    CreateShortCut \\\"$DESKTOP\\\\${start_menu_name}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${project_name}.exe\\\"
                \\\${EndIf}
            ")

            # manually remove the desktop link during uninstallation
            set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
                Delete '$DESKTOP\\\\${start_menu_name}.lnk'
            ")

            set(CPACK_CREATE_DESKTOP_LINKS "${PROJ_INSTALLER_DISPLAY_NAME}")

            set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")

            set(CPACK_OUTPUT_CONFIG_FILE "CPackConfig.cmake")

            include(CPack)
        endif ()
    elseif ("${PROJ_TYPE}" STREQUAL "LIB")
        # library
        add_library(${project_name} STATIC ${files_inc} ${files_src} ${files_misc})
    else ()
        message(FATAL_ERROR "Default project \"${project_name}\" missing option 'TYPE'.")
    endif ()
        
    # engine includes/lib if not disabled
    if ("${PROJ_NO_ENGINE}" STREQUAL "FALSE")
        ursine_add_include_directories(${project_name} ${URSINE_INCLUDE_DIRS})

        target_link_libraries(${project_name} UrsineEngine)
    endif ()

    # dependency includes
    foreach (dependency ${PROJ_DEPENDS})
        set(dependency_includes ${dependency}_INCLUDE_DIRS)

        # includes if defined
        if (DEFINED ${dependency_includes})
            ursine_add_include_directories(${project_name} 
                ${${dependency_includes}}
            )
        endif ()

        # add the defintion "URSINE_DEPENDENCY_NAME"
        add_definitions(-DURSINE_DEPENDENCY_${dependency})
    endforeach ()

    if (MSVC)
        # default to console as subsytem if not defined
        if ("${PROJ_SUBSYSTEM_DEBUG}" STREQUAL "")
            set(PROJ_SUBSYSTEM_DEBUG "CONSOLE")
        endif ()

        # default to window as subsytem if not defined
        if ("${PROJ_SUBSYSTEM_RELEASE}" STREQUAL "")
            set(PROJ_SUBSYSTEM_RELEASE "WINDOWS")
        endif ()

        set_target_properties(${project_name} PROPERTIES LINK_FLAGS_DEBUG
            "/SUBSYSTEM:${PROJ_SUBSYSTEM_DEBUG}
            /NODEFAULTLIB:msvcrt.lib"
        )
    
        set_target_properties(${project_name} PROPERTIES LINK_FLAGS_RELEASE
            "/SUBSYSTEM:${PROJ_SUBSYSTEM_RELEASE}
            /NODEFAULTLIB:msvcrtd.lib"
        )
    endif ()

    # add reflection parser
    if ("${PROJ_BUILD_META}" STREQUAL "TRUE")
        # add the generated files to the source generated source group
        source_group("Generated" FILES ${meta_generated_header} ${meta_generated_src})

        get_property(directories TARGET ${project_name} PROPERTY INCLUDE_DIRECTORIES)

        set(meta_flags "")

        # build the include directory flags
        foreach (directory ${directories})
            set(meta_flags ${meta_flags} "\\-I${directory}")
        endforeach ()

        if ("${PROJ_PCH_NAME}" STREQUAL "")
            set(pch_switch "")
        else ()
            set(pch_switch "--pch \"${PROJ_PCH_NAME}.h\"")
        endif ()

        add_custom_command(
            TARGET ${project_name}
            PRE_BUILD
            COMMAND call "$<TARGET_FILE:ReflectionParser>"
            --target-name "${project_name}"
            --in-source "${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_SOURCE_DIR}/${PROJ_META_HEADER}"
            --out-header "${meta_generated_header}"
            --out-source "${meta_generated_src}"
            ${pch_switch}
            --flags ${meta_flags}
        )
    endif ()
    
    # project folder
    if (NOT "${PROJ_FOLDER}" STREQUAL "")
        ursine_set_folder(${project_name} ${PROJ_FOLDER})
    endif ()
    
    # precompiled header
    if (NOT "${PROJ_PCH_NAME}" STREQUAL "")
        add_definitions(-DPRECOMPILED_HEADER_FILE="${PROJ_PCH_NAME}.h")

        ursine_precompiled_header(${project_name}
            ${PROJ_PCH_NAME}.h
            ${PROJ_SOURCE_DIR}/${PROJ_PCH_NAME}.cpp
        )
    endif ()
    
    # parse source groups (VS Filters)
    if ("${PROJ_PARSE_SOURCE_GROUPS}" STREQUAL "TRUE")
        ursine_parse_source_groups(
            ${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_INCLUDE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_SOURCE_DIR}
        )
    endif ()
    
    set(abs_include "${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_INCLUDE_DIR}")
    
    # recursive includes
    if ("${PROJ_RECURSIVE_INCLUDES}" STREQUAL "TRUE")
        ursine_recursive_includes(
            ${abs_include} 
            f_includes
        )
    # just include the include directory
    else ()
        set(f_includes "${abs_include}")
    endif ()
    
    ursine_add_include_directories(${project_name} ${f_includes})
    
    set(${project_name}_INCLUDE_DIRS ${f_includes} PARENT_SCOPE)
endmacro ()