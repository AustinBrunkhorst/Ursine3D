#include "UrsinePrecompiled.h"

#include "EditorToolResources.h"

#define RESOURCE_GUID(guid) ursine::GUIDStringGenerator( )( guid )

namespace ursine
{
    namespace editor_resources
    {
        const extern GUID IconDirectionalLight = RESOURCE_GUID( "ccde8fd8-109e-4785-b3b7-b3f258a87dcd" );
        const extern GUID IconPointLight       = RESOURCE_GUID( "280d307f-bdcc-4576-9da0-bfd4be5cfb81" );
        const extern GUID IconSpotLight        = RESOURCE_GUID( "24a8c5ef-560f-40e7-855a-807bb692223c" );
        const extern GUID IconCamera           = RESOURCE_GUID( "6cd72102-b89c-450c-8728-6d3adeb50bac" );
    }
}

#undef RESOURCE_GUID