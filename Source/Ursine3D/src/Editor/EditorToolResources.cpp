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
        const GUID           IconCamera = RESOURCE_GUID( "8a06df59-c9f4-4cca-9eac-972fec8965ce" );

        const GUID ArchetypeTranslateTool = RESOURCE_GUID( "25b0a698-7e3d-44a5-a79d-c244d7017f9d" );
        const GUID ArchetypeScaleTool     = RESOURCE_GUID( "b4879bb4-f351-4570-939f-100ff6349f98" );
        const GUID ArchetypeRotateTool    = RESOURCE_GUID( "46cbbc00-a136-47f7-a5b1-fe22e2b7f0f3" );
        const GUID ArchetypeDuplicateTool = RESOURCE_GUID( "4c62a439-36c3-44ac-a876-f0bdd81a2dfc" );

        const GUID ModelTorus = RESOURCE_GUID( "eca0044d-0cbe-4b07-8f16-823e67713b83" );
    }
}

#undef RESOURCE_GUID

#endif // !defined(URSINE_WITH_EDITOR)