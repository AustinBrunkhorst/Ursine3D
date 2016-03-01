#include "UrsinePrecompiled.h"

#if defined(URSINE_WITH_EDITOR)

#include "EditorToolResources.h"

#define RESOURCE_GUID(guid) ursine::GUIDStringGenerator( )( guid )

namespace ursine
{
    namespace editor_resources
    {
        const GUID IconDirectionalLight = RESOURCE_GUID( "ccde8fd8-109e-4785-b3b7-b3f258a87dcd" );
        const GUID       IconPointLight = RESOURCE_GUID( "280d307f-bdcc-4576-9da0-bfd4be5cfb81" );
        const GUID        IconSpotLight = RESOURCE_GUID( "24a8c5ef-560f-40e7-855a-807bb692223c" );
        const GUID           IconCamera = RESOURCE_GUID( "83b03acf-4b4e-4200-8503-d125ba6dd609" );
    }
}

#undef RESOURCE_GUID

#endif // !defined(URSINE_WITH_EDITOR)