ursine_project(Shared
    FOLDER "Shared"
    TYPE LIB
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    BUILD_META
    META_HEADER "SharedReflection.h"
    META_MODULE_HEADER "SharedReflectionModule.h"
    PCH_NAME "Precompiled"
)
