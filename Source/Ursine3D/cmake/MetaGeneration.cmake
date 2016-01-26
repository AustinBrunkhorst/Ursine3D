macro(ursine_prebuild_meta)
    ursine_parse_arguments(
        PREBUILD_META 
        "SOURCE_ROOT;HEADER_FILES;OUT_GENERATED_DIR;OUT_GENERATED_FILES;OUT_FILES_INC;OUT_FILES_SRC;PCH_NAME" 
        "" 
        ${ARGN}
    )

    set(${PREBUILD_META_OUT_GENERATED_DIR} "${CMAKE_CURRENT_BINARY_DIR}/Generated")
    set(${PREBUILD_META_OUT_MODULE_SOURCE} "${META_GENERATED_DIR}/Module.${PROJECT_NAME}.cpp")

    set(${PREBUILD_META_OUT_GENERATED_FILES} "${${PREBUILD_META_OUT_MODULE_SOURCE}}")

    list(APPEND ${PREBUILD_META_OUT_FILES_SRC} ${${PREBUILD_META_OUT_MODULE_SOURCE}})

    if (NOT "${PREBUILD_META_PCH_NAME}" STREQUAL "")
        set(EMPTY_SOURCE_CONTENTS "#include \"${PREBUILD_META_PCH_NAME}.h\"")
    else ()
        set(EMPTY_SOURCE_CONTENTS "")
    endif ()

    foreach (HEADER ${PREBUILD_META_HEADER_FILES})
        get_filename_component(DIRECTORY_NAME ${HEADER} DIRECTORY)
        get_filename_component(BASE_NAME ${HEADER} NAME_WE)

        file(RELATIVE_PATH RELATIVE ${PREBUILD_META_SOURCE_ROOT} "${DIRECTORY_NAME}/${BASE_NAME}.Generated")

        set(GENERATED_HEADER "${GENERATED_DIR}/${RELATIVE}.h")
        set(GENERATED_SOURCE "${GENERATED_DIR}/${RELATIVE}.cpp")

        # we have to create the files, as they might not be written to
        if (NOT EXISTS ${GENERATED_HEADER})
            file(WRITE ${GENERATED_HEADER} "")
        endif ()

        if (NOT EXISTS ${GENERATED_SOURCE})
            file(WRITE ${GENERATED_SOURCE} ${EMPTY_SOURCE_CONTENTS})
        endif ()
        
        list(APPEND ${PREBUILD_META_OUT_GENERATED_FILES} ${GENERATED_HEADER} ${GENERATED_SOURCE})

        list(APPEND ${PREBUILD_META_OUT_FILES_INC} ${GENERATED_HEADER})
        list(APPEND ${PREBUILD_META_OUT_FILES_SRC} ${GENERATED_SOURCE})
    endforeach ()
endmacro()

macro(ursine_build_meta)
    ursine_parse_arguments(
        BUILD_META 
        "TARGET;FLAGS;SOURCE_ROOT;SOURCE_FILE;MODULE_HEADER;MODULE_SOURCE_FILE;GENERATED_DIR;GENERATED_FILES;HEADER_FILES;PCH_NAME" 
        "" 
        ${ARGN}
    )

    # add the ReflectionParser target if it doesn't exist
    if (NOT TARGET ReflectionParser)
        add_subdirectory("${ENGINE_DIR}/../Tools/ReflectionParser" "${CMAKE_BINARY_DIR}/ReflectionParser")
    endif ()

    get_property(DIRECTORIES TARGET ${BUILD_META_TARGET} PROPERTY INCLUDE_DIRECTORIES)

    set(FLAGS ${BUILD_META_FLAGS})

    # build the include directory flags
    foreach (DIRECTORY ${DIRECTORIES})
        list(APPEND FLAGS "\\-I${DIRECTORY}")
    endforeach ()

    source_group("Generated" FILES ${BUILD_META_GENERATED_FILES})

    if ("${BUILD_META_PCH_NAME}" STREQUAL "")
        set(PCH_SWITCH "")
    else () 
        set(PCH_SWITCH "--pch \"${BUILD_META_PCH_NAME}.h\"")
    endif ()

    # add the command that generates the header and source files
    add_custom_command(
        OUTPUT ${BUILD_META_GENERATED_FILES}
        DEPENDS ${BUILD_META_HEADER_FILES}
        COMMAND call "$<TARGET_FILE:ReflectionParser>"
        --target-name "${BUILD_META_TARGET}"
        --source-root "${BUILD_META_SOURCE_ROOT}"
        --in-source "${BUILD_META_SOURCE_ROOT}/${BUILD_META_SOURCE_FILE}"
        --module-header "${BUILD_META_SOURCE_ROOT}/${BUILD_META_MODULE_HEADER}"
        --out-source "${BUILD_META_MODULE_SOURCE_FILE}"
        --out-dir "${BUILD_META_GENERATED_DIR}"
        ${PCH_SWITCH}
        --flags ${FLAGS}
    )
endmacro ()