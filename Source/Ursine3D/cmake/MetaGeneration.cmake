function(ursine_prebuild_meta)
    ursine_parse_arguments(
        PREBUILD_META 
        "TARGET;GENERATED_DIR;SOURCE_ROOT;HEADER_FILES;MODULE_HEADER;OUT_MODULE_SOURCE;OUT_GENERATED_FILES;OUT_INC;OUT_SRC;PCH_NAME" 
        "" 
        ${ARGN}
    )

    # source file for this reflection module
    set(MODULE_SOURCE "${PREBUILD_META_GENERATED_DIR}/Module.${PREBUILD_META_TARGET}.Generated.cpp")
    
    # output the source file
    set(${PREBUILD_META_OUT_MODULE_SOURCE} ${MODULE_SOURCE} PARENT_SCOPE)

    set(GENERATED_FILES "${MODULE_SOURCE}")
    set(GENERATED_HEADERS "")
    set(GENERATED_SOURCES "${MODULE_SOURCE}")

    # output it to the output source files
    set(${PREBUILD_META_OUT_SRC} ${${PREBUILD_META_OUT_SRC}} "${MODULE_SOURCE}" PARENT_SCOPE)

    # exclude the module header from the included headers
    list(REMOVE_ITEM PREBUILD_META_HEADER_FILES "${PREBUILD_META_SOURCE_ROOT}/${PREBUILD_META_MODULE_HEADER}")

    foreach (HEADER ${PREBUILD_META_HEADER_FILES})
        get_filename_component(DIRECTORY_NAME ${HEADER} DIRECTORY)
        get_filename_component(BASE_NAME ${HEADER} NAME_WE)
        get_filename_component(EXTENSION ${HEADER} EXT)

        # skip hpp files
        if (NOT "${EXTENSION}" STREQUAL ".hpp")
            file(RELATIVE_PATH RELATIVE 
                ${PREBUILD_META_SOURCE_ROOT} 
                "${DIRECTORY_NAME}/${BASE_NAME}.Generated"
            )

            set(GENERATED_HEADER "${PREBUILD_META_GENERATED_DIR}/${RELATIVE}.h")
            set(GENERATED_SOURCE "${PREBUILD_META_GENERATED_DIR}/${RELATIVE}.cpp")

            list(APPEND GENERATED_FILES ${GENERATED_HEADER} ${GENERATED_SOURCE})
            list(APPEND GENERATED_HEADERS ${GENERATED_HEADER})
            list(APPEND GENERATED_SOURCES ${GENERATED_SOURCE})
        endif ()
    endforeach ()

    source_group(".Generated" FILES ${GENERATED_FILES})

    set(${PREBUILD_META_OUT_GENERATED_FILES} "${GENERATED_FILES}" PARENT_SCOPE)
    set(${PREBUILD_META_OUT_INC} ${${PREBUILD_META_OUT_INC}} ${GENERATED_HEADERS} PARENT_SCOPE)
    set(${PREBUILD_META_OUT_SRC} ${${PREBUILD_META_OUT_SRC}} ${GENERATED_SOURCES} PARENT_SCOPE)
endfunction ()

function(ursine_build_meta)
    ursine_parse_arguments(
        BUILD_META 
        "TARGET;DEFINES;INCLUDES;SOURCE_ROOT;SOURCE_FILE;MODULE_HEADER;MODULE_SOURCE_FILE;GENERATED_DIR;GENERATED_FILES;HEADER_FILES;PCH_NAME" 
        "" 
        ${ARGN}
    )

    get_property(DIRECTORIES TARGET ${BUILD_META_TARGET} PROPERTY INCLUDE_DIRECTORIES)

    set(RAW_INCLUDES ${GLOBAL_META_INCLUDES} ${DIRECTORIES} ${BUILD_META_INCLUDES})

    set(INCLUDES "")

    foreach (INC ${RAW_INCLUDES})
        set(INCLUDES "${INCLUDES}${INC}\n")
    endforeach ()

    set(INCLUDES_FILE "${BUILD_META_GENERATED_DIR}/Module.${BUILD_META_TARGET}.Includes.txt")

    file(WRITE ${INCLUDES_FILE} ${INCLUDES})
	
    set(DEFINES ${GLOBAL_META_DEFINES} ${BUILD_META_DEFINES})

    string(REPLACE " " "" DEFINES_TRIMMED "${DEFINES}")

    if ("${DEFINES_TRIMMED}" STREQUAL "")
    	set(DEFINES_SWITCH )
    else ()
    	set(DEFINES_SWITCH --defines "${DEFINES}")
    endif ()

    # empty source files need to include the precompiled header
    if (NOT "${BUILD_META_PCH_NAME}" STREQUAL "")
        set(EMPTY_SOURCE_CONTENTS "#include \"${BUILD_META_PCH_NAME}.h\"")
        set(PCH_SWITCH --pch \"${BUILD_META_PCH_NAME}.h\")
    else ()
        set(EMPTY_SOURCE_CONTENTS "")
        set(PCH_SWITCH )
    endif ()

    list(REMOVE_ITEM BUILD_META_GENERATED_FILES "${BUILD_META_SOURCE_ROOT}/${BUILD_META_MODULE_HEADER}")

    foreach (GENERATED_FILE ${BUILD_META_GENERATED_FILES})
        get_filename_component(EXTENSION ${GENERATED_FILE} EXT)

        set_source_files_properties(${GENERATED_FILE} PROPERTIES GENERATED TRUE)

        # we have to create the files, as they might not be written to
        if (NOT EXISTS ${GENERATED_FILE})
            if ("${EXTENSION}" STREQUAL ".Generated.h")
                file(WRITE ${GENERATED_FILE} "")
            else ()
                file(WRITE ${GENERATED_FILE} ${EMPTY_SOURCE_CONTENTS})
            endif ()
        endif ()
    endforeach ()

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
        --includes "${INCLUDES_FILE}"
        ${DEFINES_SWITCH}
    )

    set(REBUILD_TARGET "${BUILD_META_TARGET}-RebuildMeta")

    add_custom_target(
        ${REBUILD_TARGET}
        COMMAND call "$<TARGET_FILE:ReflectionParser>"
        --target-name "${BUILD_META_TARGET}"
        --source-root "${BUILD_META_SOURCE_ROOT}"
        --in-source "${BUILD_META_SOURCE_ROOT}/${BUILD_META_SOURCE_FILE}"
        --module-header "${BUILD_META_SOURCE_ROOT}/${BUILD_META_MODULE_HEADER}"
        --out-source "${BUILD_META_MODULE_SOURCE_FILE}"
        --out-dir "${BUILD_META_GENERATED_DIR}"
        ${PCH_SWITCH}
        --force-rebuild
        --display-diagnostics
        --includes "${INCLUDES_FILE}"
        ${DEFINES_SWITCH}
    )

    ursine_set_folder(${REBUILD_TARGET} ".Utility/Meta")
endfunction ()