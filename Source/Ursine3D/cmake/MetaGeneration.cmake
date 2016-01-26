macro(ursine_build_meta TARGET_NAME OUT_GENERATED_HEADERS OUT_GENERATED_SOURCES)
    # add the ReflectionParser target if it doesn't exist
    if (NOT TARGET ReflectionParser)
        add_subdirectory("${ENGINE_DIR}/../Tools/ReflectionParser" "${CMAKE_CURRENT_BINARY_DIR}/ReflectionParser")
    endif ()

    get_property(DIRECTORIES TARGET ${TARGET_NAME} PROPERTY INCLUDE_DIRECTORIES)

    set(META_SOURCE_ROOT "${PROJECT_SOURCE_DIR}/${PROJ_SOURCE_DIR}")
    set(META_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/Generated")
    set(META_FLAGS "${PROJ_META_FLAGS}")

    # build the include directory flags
    foreach (DIRECTORY ${DIRECTORIES})
        list(APPEND META_FLAGS "\\-I${DIRECTORY}")
    endforeach ()

    set(META_GENERATED_FILES "")

    set(META_MODULE_SOURCE "${META_GENERATED_DIR}/Module.${PROJECT_NAME}.cpp")

    list(APPEND META_GENERATED_FILES ${META_MODULE_SOURCE})
    #list(APPEND ${${OUT_GENERATED_SOURCES}} ${META_MODULE_SOURCE})

    source_group("Generated" FILES ${META_MODULE_SOURCE})

    foreach (HEADER ${FILES_INC})
        get_filename_component(DIRECTORY_NAME ${HEADER} DIRECTORY)
        get_filename_component(BASE_NAME ${HEADER} NAME_WE)

        file(RELATIVE_PATH RELATIVE ${META_SOURCE_ROOT} "${DIRECTORY_NAME}/${BASE_NAME}")

        set(GENERATED_HEADER "${META_GENERATED_DIR}/${RELATIVE}.h")
        set(GENERATED_SOURCE "${META_GENERATED_DIR}/${RELATIVE}.cpp")

        list(APPEND META_GENERATED_FILES ${GENERATED_HEADER} ${GENERATED_SOURCE})
        #list(APPEND ${${OUT_GENERATED_HEADERS}} ${GENERATED_HEADER})
        #list(APPEND ${${OUT_GENERATED_SOURCES}} ${GENERATED_SOURCE})

        # add the generated files to the source generated source group
        source_group("Generated" FILES ${GENERATED_HEADER} ${GENERATED_SRC})
    endforeach ()

    if ("${PROJ_PCH_NAME}" STREQUAL "")
        set(PCH_SWITCH "")
    else () 
        set(PCH_SWITCH "--pch \"${PROJ_PCH_NAME}.h\"")
    endif ()

    set(META_DEPENDS ${FILES_INC})

    # add the explicit dependencies if applicable
    if (NOT "${PROJ_META_DEPENDENCIES}" STREQUAL "")
        set(META_DEPENDS ${META_DEPENDS} ${PROJ_META_DEPENDENCIES})
    endif ()

    # add the command that generates the header and source files
    add_custom_command(
        OUTPUT ${META_GENERATED_FILES}
        DEPENDS ${META_DEPENDS}
        COMMAND call "$<TARGET_FILE:ReflectionParser>"
        --target-name "${PROJECT_NAME}"
        --source-root "${META_SOURCE_ROOT}"
        --in-source "${META_SOURCE_ROOT}/${PROJ_META_HEADER}"
        --module-header "${META_SOURCE_ROOT}/${PROJ_META_HEADER}"
        --out-source "${META_MODULE_SOURCE}"
        --out-dir "${META_GENERATED_DIR}"
        ${PCH_SWITCH}
        --flags ${META_FLAGS}
    )
endmacro ()