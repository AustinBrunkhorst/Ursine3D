/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorToolResources.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#if defined(URSINE_WITH_EDITOR)

namespace ursine
{
    namespace editor_resources
    {
        const extern GUID IconDirectionalLight;
        const extern GUID IconPointLight;
        const extern GUID IconSpotLight;
        const extern GUID IconCamera;
        const extern GUID IconParticleSystem;

        const extern GUID ArchetypeTranslateTool;
        const extern GUID ArchetypeScaleTool;
        const extern GUID ArchetypeRotateTool;
        const extern GUID ArchetypeDuplicateTool;

        const extern GUID ModelTorus;
    }
}

#endif // defined(URSINE_WITH_EDITOR)