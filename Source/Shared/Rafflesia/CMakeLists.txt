
ursine_project(Rafflesia-Shared
    FOLDER "Rafflesia-Shared"
    TYPE LIB
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FBX FileWatcher
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    BUILD_META
    META_HEADER "Rafflesia-SharedReflection.h"
    META_MODULE_HEADER "Rafflesia-SharedReflectionModule.h"
    PCH_NAME "Precompiled"
) 
