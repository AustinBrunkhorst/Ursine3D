include(GlobalConfig)

macro (_install_expand_files EXPR)
    file(GLOB_RECURSE EXPR_EXPANDED ${EXPR})

    get_filename_component(BASE_DIR ${EXPR} DIRECTORY)

    foreach (EXPANDED ${EXPR_EXPANDED})
        get_filename_component(EXPANDED_DIR ${EXPANDED} DIRECTORY)

        file(RELATIVE_PATH RELATIVE_DIR "${BASE_DIR}" "${EXPANDED_DIR}")

        install(FILES ${EXPANDED} DESTINATION "bin/${RELATIVE_DIR}" ${ARGN})
    endforeach ()
endmacro ()

macro (ursine_project PROJECT_NAME)
    ursine_parse_arguments(PROJ 
        "FOLDER;TYPE;SOURCE_DIR;INCLUDE_DIR;DEPENDS;PCH_NAME;SYM_LINKS;INSTALLER_VERSION;INSTALLER_SUMMARY;INSTALLER_DISPLAY_NAME;WINDOWS_RESOURCE_FILE;INSTALLER_ICON;INSTALLER_UNINSTALL_ICON;SUBSYSTEM_DEBUG;SUBSYSTEM_RELEASE;META_HEADER;META_FLAGS;META_DEPENDENCIES;COPY_SHADERS" 
        "NO_ENGINE;PARSE_SOURCE_GROUPS;RECURSIVE_INCLUDES;INCLUDE_INSTALLER;BUILD_META" 
        ${ARGN})
    
    project(${PROJECT_NAME} CXX)

    set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

    add_definitions(-D${PROJECT_NAME}_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
    set_property(TARGET Ursine3D APPEND PROPERTY COMPILE_DEFINITIONS ${PROJECT_NAME}_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
    
    file(GLOB_RECURSE FILES_SRC ${PROJ_SOURCE_DIR}/*.cpp)
    file(GLOB_RECURSE FILES_INC ${PROJ_INCLUDE_DIR}/*.h ${PROJ_INCLUDE_DIR}/*.hpp)

    if ("${PROJ_BUILD_META}" STREQUAL "TRUE")
        set(META_GENERATED_HEADER "${BUILD_DIR}/Meta.Generated.h")
        set(META_GENERATED_SRC "${BUILD_DIR}/Meta.Generated.cpp")

        # add to the sources
        list(APPEND FILES_SRC ${META_GENERATED_SRC})
        list(APPEND FILES_INC ${META_GENERATED_HEADER})
    endif ()

    set(FILES_MISC "")

    if ("${PROJ_TYPE}" STREQUAL "EXE")
        set(POST_BUILD_COMMANDS "")

        if (MSVC)
            # if the RESOURCEs file is set, include in the source
            if (NOT "${PROJ_WINDOWS_RESOURCE_FILE}" STREQUAL "")
                list(APPEND FILES_MISC "${PROJ_WINDOWS_RESOURCE_FILE}")
            endif ()
        endif ()

        # executable
        add_executable(${PROJECT_NAME} ${FILES_INC} ${FILES_SRC} ${FILES_MISC})

        if (MSVC)
            # if the RESOURCEs file is set, add to RESOURCEs source group
            if (NOT "${PROJ_WINDOWS_RESOURCE_FILE}" STREQUAL "")
                source_group("RESOURCEs" FILES "${PROJ_WINDOWS_RESOURCE_FILE}")
            endif ()
        endif ()

        # all projects depend on boost
        list(APPEND PROJ_DEPENDS Boost)

        list(REMOVE_DUPLICATES PROJ_DEPENDS)

        foreach (DEPENDENCY ${PROJ_DEPENDS})
            if ("${DEP_${DEPENDENCY}_DIR}" STREQUAL "")
                message(FATAL_ERROR "Unknown dependency \"${DEPENDENCY}\".")
            endif ()

            # copy shared libraries
            foreach (SHARED_LIB ${${DEPENDENCY}_SHARED_LIBS})
                string(REPLACE "/" "\\" SHARED_LIB_ESCAPED "${SHARED_LIB}")
        
                list(APPEND POST_BUILD_COMMANDS
                    COMMAND 
                    xcopy \"${SHARED_LIB_ESCAPED}\" \"$<TARGET_FILE_DIR:${PROJECT_NAME}>\" /Y /d
                )

                # install shared lib if applicable
                if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
                    _install_expand_files(${SHARED_LIB} COMPONENT ${PROJECT_NAME})
                endif ()
            endforeach ()

            # installer specific shared libs if applicable
            if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
                set(LIB_CONFIGS "Debug;Release")

                foreach (CONFIG ${LIB_CONFIGS})
                    foreach (SHARED_LIB ${${DEPENDENCY}_SHARED_LIBS_${CONFIG}})
                        _install_expand_files(${SHARED_LIB} COMPONENT ${PROJECT_NAME} CONFIGURATIONS ${CONFIG})
                    endforeach ()
                endforeach ()
            endif ()

            # copy resources
            foreach (RESOURCE ${${DEPENDENCY}_RESOURCES})
                string(REPLACE "/" "\\" RESOURCE_ESCAPED "${RESOURCE}")
        
                list(APPEND POST_BUILD_COMMANDS
                    COMMAND 
                    xcopy \"${RESOURCE_ESCAPED}\" \"$<TARGET_FILE_DIR:${PROJECT_NAME}>\" /Y /e /d
                )

                # install resource if applicable
                if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
                    get_filename_component(BASE_DIR ${RESOURCE} DIRECTORY)

                    install(DIRECTORY "${BASE_DIR}/" DESTINATION "bin" COMPONENT ${PROJECT_NAME} PATTERN ${RESOURCE})
                endif ()
            endforeach ()

            set(DEPENDENCY_LIBS ${DEPENDENCY}_STATIC_LIBS)

            # static libs if defined
            if (DEFINED ${DEPENDENCY_LIBS})
                target_link_libraries(${PROJECT_NAME}
                    ${${DEPENDENCY_LIBS}}
                )
            endif ()
        endforeach ()

        if (NOT "${PROJ_SYM_LINKS}" STREQUAL "")
            # create symbolic links to folders
            foreach (LINK ${PROJ_SYM_LINKS})
                list(APPEND POST_BUILD_COMMANDS
                    COMMAND 
                    "${CMAKE_MODULE_PATH}/Scripts/SymLink.bat" 
                    \"$<TARGET_FILE_DIR:${PROJECT_NAME}>/${LINK}\" \"${CMAKE_CURRENT_SOURCE_DIR}/${LINK}\"
                )

                # install folder if applicable
                if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
                    install(DIRECTORY ${LINK} DESTINATION "bin" COMPONENT ${PROJECT_NAME})
                endif ()
            endforeach ()
        endif ()

        # handle shaders
        if (NOT "${PROJ_COPY_SHADERS}" STREQUAL "")
            add_dependencies(${PROJECT_NAME} Shaders)

            # create a macro to reference at compile time
            add_definitions(-DURSINE_SHADER_BUILD_DIRECTORY=\"${PROJ_COPY_SHADERS}\")
            
            foreach (SHADER ${URSINE_SHADER_FILES})
                get_filename_component(shader_file "${SHADER}" NAME_WE)

                # copy shaders on post build of the shader project
                list(APPEND POST_BUILD_COMMANDS
                    COMMAND 
                    ${CMAKE_COMMAND} -E copy_if_different
                    \"$<TARGET_FILE_DIR:Shaders>/${shader_file}.cso\" \"$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJ_COPY_SHADERS}${shader_file}.cso\"
                )

                # install shaders if applicable
                if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
                    install(FILES "$<TARGET_FILE_DIR:Shaders>/${shader_file}.cso" DESTINATION "bin/${PROJ_COPY_SHADERS}" COMPONENT ${PROJECT_NAME})
                endif ()
            endforeach ()
        endif ()
        
        # add post build command if it isn't empty
        if (NOT "${POST_BUILD_COMMANDS}" STREQUAL "")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                ${POST_BUILD_COMMANDS}
                COMMENT "Libs, Resources, Symbolic links"
            )
        endif ()

        # installer configuration
        if ("${PROJ_INCLUDE_INSTALLER}" STREQUAL "TRUE")
            install(TARGETS ${PROJECT_NAME} DESTINATION "bin" COMPONENT ${PROJECT_NAME})

            if (MSVC)
                install(PROGRAMS ${ENGINE_VCREDIST_FILE} DESTINATION "tmp" COMPONENT ${PROJECT_NAME})
                install(PROGRAMS ${ENGINE_D3DREDIST_FILE} DESTINATION "tmp" COMPONENT ${PROJECT_NAME})
                install(PROGRAMS ${ENGINE_D3DREDIST_HELPER_DLL} DESTINATION "tmp" COMPONENT ${PROJECT_NAME})
            endif ()

            set(EXPR_DEBUG $<OR:$<CONFIG:debug>,$<CONFIG:relwithdebinfo>>)
            set(EXPR_RELEASE $<NOT:${EXPR_DEBUG}>)

            set(INSTALLER_TARGET "${PROJECT_NAME}-Installer")

            add_custom_target(${INSTALLER_TARGET}
                COMMAND cd ${CMAKE_CURRENT_BINARY_DIR} && cpack -C $<${EXPR_DEBUG}:Debug>$<${EXPR_RELEASE}:Release>
                DEPENDS ${PROJECT_NAME})

            # project folder
            if (NOT "${PROJ_FOLDER}" STREQUAL "")
                ursine_set_folder(${INSTALLER_TARGET} ${PROJ_FOLDER})
            endif ()

            if ("${PROJ_INSTALLER_DISPLAY_NAME}" STREQUAL "")
                set(PROJ_INSTALLER_DISPLAY_NAME ${PROJECT_NAME})
            endif ()

            if ("${PROJ_INSTALLER_VERSION}" STREQUAL "")
                set(START_MENU_NAME "${PROJ_INSTALLER_DISPLAY_NAME}")

                set(PROJ_INSTALLER_VERSION "1.0.0")

                set(INSTALL_DIRECTORY_SUFFIX "")
            else ()
                set(INSTALL_DIRECTORY_SUFFIX "\\\\${PROJ_INSTALLER_VERSION}")

                set(START_MENU_NAME "${PROJ_INSTALLER_DISPLAY_NAME} \(${PROJ_INSTALLER_VERSION}\)")
            endif ()

            set(CPACK_NSIS_MODIFY_PATH "ON")

            set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${START_MENU_NAME}")
            set(CPACK_INSTALL_CMAKE_PROJECTS ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_NAME} ${PROJECT_NAME} /)
            set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
            set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJ_INSTALLER_SUMMARY})
            set(CPACK_NSIS_DISPLAY_NAME ${PROJ_INSTALLER_DISPLAY_NAME})
            set(CPACK_PACKAGE_VERSION ${PROJ_INSTALLER_VERSION})

            # custom start menu define
            set(CPACK_NSIS_DEFINES "${CPACK_NSIS_DEFINES}
                !define MUI_STARTMENUPAGE_DEFAULTFOLDER \\\"DigiPen\\\\${START_MENU_NAME}\\\"
            ")

            set(CPACK_PACKAGE_INSTALL_DIRECTORY "DigiPen\\\\${PROJECT_NAME}${INSTALL_DIRECTORY_SUFFIX}")
            set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${PROJECT_NAME}${PROJ_INSTALLER_VERSION})

            set(CPACK_NSIS_MUI_ICON "${PROJ_INSTALLER_ICON}")
            set(CPACK_NSIS_MUI_UNIICON "${PROJ_INSTALLER_UNINSTALL_ICON}")

            set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/${PROJECT_NAME}.exe")
            set(CPACK_NSIS_MUI_FINISHPAGE_RUN "${PROJECT_NAME}.exe")

            # manually create a desktop link if the option was set
            set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                \\\${If} $INSTALL_DESKTOP == 1
                    CreateShortCut \\\"$DESKTOP\\\\${START_MENU_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
                \\\${EndIf}
            ")

            # manually remove the desktop link during uninstallation
            set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
                Delete '$DESKTOP\\\\${START_MENU_NAME}.lnk'
            ")

            set(CPACK_CREATE_DESKTOP_LINKS "${PROJ_INSTALLER_DISPLAY_NAME}")

            set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")

            set(CPACK_OUTPUT_CONFIG_FILE "CPackConfig.cmake")

            include(CPack)
        endif ()
    elseif ("${PROJ_TYPE}" STREQUAL "LIB")
        # library
        add_library(${PROJECT_NAME} STATIC ${FILES_INC} ${FILES_SRC} ${FILES_MISC})
    else ()
        message(FATAL_ERROR "Project \"${PROJECT_NAME}\" missing option \"TYPE\".")
    endif ()
        
    # dependency includes
    foreach (DEPENDENCY ${PROJ_DEPENDS})
        set(DEPENDENCY_INCLUDES ${DEPENDENCY}_INCLUDE_DIRS)

        # includes if defined
        if (DEFINED ${DEPENDENCY_INCLUDES})
            ursine_add_include_directories(${PROJECT_NAME} 
                ${${DEPENDENCY_INCLUDES}}
            )
        endif ()

        # add the defintion "URSINE_DEPENDENCY_NAME"
        add_definitions(-DURSINE_DEPENDENCY_${DEPENDENCY})

        # add it to the engine explicitly too
        set_property(TARGET Ursine3D APPEND PROPERTY COMPILE_DEFINITIONS URSINE_DEPENDENCY_${DEPENDENCY})
    endforeach ()
       
    # engine includes/lib if not disabled
    if ("${PROJ_NO_ENGINE}" STREQUAL "FALSE")
        ursine_add_include_directories(${PROJECT_NAME} ${URSINE_INCLUDE_DIRS})

        target_link_libraries(${PROJECT_NAME} Ursine3D)
    endif ()

    if (MSVC)
        # treat warnings as errors
        target_compile_options(${PROJECT_NAME} PUBLIC /WX)

        # default to console as subsytem if not defined
        if ("${PROJ_SUBSYSTEM_DEBUG}" STREQUAL "")
            set(PROJ_SUBSYSTEM_DEBUG "CONSOLE")
        endif ()

        # default to window as subsytem if not defined
        if ("${PROJ_SUBSYSTEM_RELEASE}" STREQUAL "")
            set(PROJ_SUBSYSTEM_RELEASE "WINDOWS")
        endif ()

        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG
            "/SUBSYSTEM:${PROJ_SUBSYSTEM_DEBUG}
            /NODEFAULTLIB:msvcrt.lib"
        )
    
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE
            "/SUBSYSTEM:${PROJ_SUBSYSTEM_RELEASE}
            /NODEFAULTLIB:msvcrtd.lib"
        )
    endif ()

    # add reflection parser
    if ("${PROJ_BUILD_META}" STREQUAL "TRUE")
        if ("${PROJ_NO_ENGINE}" STREQUAL "TRUE")
            message(FATAL_ERROR "Building meta requires the project to built with Ursine3D.")
        endif ()

        add_subdirectory("${ENGINE_DIR}/../Tools/ReflectionParser" "${CMAKE_CURRENT_BINARY_DIR}/ReflectionParser")

        # add the generated files to the source generated source group
        source_group("Generated" FILES ${META_GENERATED_HEADER} ${META_GENERATED_SRC})

        get_property(DIRECTORIES TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)

        set(META_FLAGS "${PROJ_META_FLAGS}")

        # build the include directory flags
        foreach (directory ${DIRECTORIES})
            set(META_FLAGS ${META_FLAGS} "\\-I${directory}")
        endforeach ()

        if (MSVC)
            set(META_FLAGS ${META_FLAGS} "\\-IC://Program Files (x86)/Microsoft Visual Studio ${VS_VERSION}.0/VC/include")
        else ()
            message(FATAL_ERROR "System include directories not implemented for this compiler.")
        endif ()

        if ("${PROJ_PCH_NAME}" STREQUAL "")
            set(PCH_SWITCH "")
        else ()
            set(PCH_SWITCH "--pch \"${PROJ_PCH_NAME}.h\"")
        endif ()

        set(META_DEPENDS ${FILES_INC} ${URSINE_HEADER_FILES})

        # add the explicit dependencies if applicable
        if (NOT "${PROJ_META_DEPENDENCIES}" STREQUAL "")
            set(META_DEPENDS ${META_DEPENDS} ${PROJ_META_DEPENDENCIES})
        endif ()

        list(REMOVE_ITEM META_DEPENDS ${META_GENERATED_HEADER})

        # add the command that generates the header and source files
        add_custom_command(
            OUTPUT ${META_GENERATED_HEADER} ${META_GENERATED_SRC}
            DEPENDS ${META_DEPENDS}
            COMMAND call "$<TARGET_FILE:ReflectionParser>"
            --target-name "${PROJECT_NAME}"
            --in-source "${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_SOURCE_DIR}/${PROJ_META_HEADER}"
            --out-header "${META_GENERATED_HEADER}"
            --out-source "${META_GENERATED_SRC}"
            ${PCH_SWITCH}
            --flags ${META_FLAGS}
        )
    endif ()

    # project folder
    if (NOT "${PROJ_FOLDER}" STREQUAL "")
        ursine_set_folder(${PROJECT_NAME} ${PROJ_FOLDER})
    endif ()
    
    # precompiled header
    if (NOT "${PROJ_PCH_NAME}" STREQUAL "")
        add_definitions(-DPRECOMPILED_HEADER_FILE="${PROJ_PCH_NAME}.h")

        ursine_precompiled_header(${PROJECT_NAME}
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
    
    set(ABS_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_INCLUDE_DIR}")
    
    # recursive includes
    if ("${PROJ_RECURSIVE_INCLUDES}" STREQUAL "TRUE")
        ursine_recursive_includes(
            ${ABS_INCLUDE} 
            ALL_INCLUDES
        )
    # just include the include directory
    else ()
        set(ALL_INCLUDES "${ABS_INCLUDE}")
    endif ()
    
    ursine_add_include_directories(${PROJECT_NAME} ${ALL_INCLUDES})
    
    get_directory_property(HAS_PARENT PARENT_DIRECTORY)

    if (HAS_PARENT)
        # export to parent scope
        set(${PROJECT_NAME}_INCLUDE_DIRS ${ALL_INCLUDES} PARENT_SCOPE)
        set(${PROJECT_NAME}_HEADER_FILES ${FILES_INC} PARENT_SCOPE)
    else ()
        set(${PROJECT_NAME}_INCLUDE_DIRS ${ALL_INCLUDES})
        set(${PROJECT_NAME}_HEADER_FILES ${FILES_INC})
    endif ()
endmacro ()