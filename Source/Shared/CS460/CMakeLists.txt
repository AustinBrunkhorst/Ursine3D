
ursine_project(CS460-Shared
    FOLDER "CS460-Shared"
    TYPE LIB
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FBX FileWatcher
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    BUILD_META
    META_HEADER "CS460-SharedReflection.h"
    META_MODULE_HEADER "CS460-SharedReflectionModule.h"
    PCH_NAME "Precompiled"
) 
