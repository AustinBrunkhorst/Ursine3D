ursine_project(Shared
    FOLDER "Shared"
    TYPE LIB
    DEPENDS Boost SDL2 CEF DirectX BulletPhysics WWise FBX
    SOURCE_DIR "Source"
    INCLUDE_DIR "Source"
    PARSE_SOURCE_GROUPS
    RECURSIVE_INCLUDES
    META_HEADER "Reflection.h"
    PCH_NAME "Precompiled"
)
