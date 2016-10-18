/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationImporter.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimationImporter.h"

#include "ResourceReader.h"
#include "ResourceImportContext.h"

#include "AnimationData.h"
#include "AnimationReader.h"

namespace ursine
{
    namespace resources
    {
        rp::AnimationImporter::AnimationImporter(void)
        {
        }

        ResourceData::Handle rp::AnimationImporter::Import(ResourceImportContext &context)
        {
            ResourceReader resource( context.resource->GetSourceFileName( ) );
            AnimationReader reader;

            return reader.Read( resource );
        }
    }
}
