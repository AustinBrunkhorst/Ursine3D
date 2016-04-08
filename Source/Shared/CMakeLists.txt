
ursine_project(Shared
    FOLDER "Shared"
    TYPE LIB
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FBX FileWatcher
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    BUILD_META
    META_HEADER "SharedReflection.h"
    META_MODULE_HEADER "SharedReflectionModule.h"
    PCH_NAME "Precompiled"
) 
