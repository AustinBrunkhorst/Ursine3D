/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UAnimationImporter.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UAnimationImporter.h"

#include "ResourceReader.h"
#include "ResourceImportContext.h"

#include "UAnimationData.h"
#include "UAnimationReader.h"

namespace ursine
{
    namespace resources
    {
        rp::UAnimationImporter::UAnimationImporter(void)
        {
        }

        ResourceData::Handle rp::UAnimationImporter::Import(ResourceImportContext &context)
        {
            ResourceReader resource( context.resource->GetSourceFileName( ) );
            UAnimationReader reader;

            return reader.Read( resource );
        }
    }
}
