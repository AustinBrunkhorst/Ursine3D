#include "UrsinePrecompiled.h"

#if defined(URSINE_WITH_EDITOR)

#include "EditorToolResources.h"

#define RESOURCE_GUID(guid) ursine::GUIDStringGenerator( )( guid )

namespace ursine
{
    namespace editor_resources
    {
        const GUID IconDirectionalLight = RESOURCE_GUID( "26b0a80a-c829-4e8e-bce9-54d2c1c23369" );
        const GUID       IconPointLight = RESOURCE_GUID( "f3901624-b038-44fe-9905-0ce6b7a741d7" );
        const GUID        IconSpotLight = RESOURCE_GUID( "da205a6b-5619-4ef4-aa64-f1cb875ce87e" );
        const GUID           IconCamera = RESOURCE_GUID( "c27e1f32-3890-4c6b-afd4-7a1574dda439" );

        const GUID ArchetypeTranslateTool = RESOURCE_GUID( "str" );
        const GUID ArchetypeScaleTool     = RESOURCE_GUID( "str" );
        const GUID ArchetypeRotateTool    = RESOURCE_GUID( "str" );
        const GUID ArchetypeDuplicateTool = RESOURCE_GUID( "str" );
    }
}

#undef RESOURCE_GUID

#endif // !defined(URSINE_WITH_EDITOR)