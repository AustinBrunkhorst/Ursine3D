/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** RigImporter.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RigImporter.h"

#include "ResourceReader.h"
#include "ResourceImportContext.h"

#include "RigData.h"
#include "RigReader.h"

namespace ursine
{
    namespace resources
    {
        rp::RigImporter::RigImporter(void)
        {
        }

        ResourceData::Handle rp::RigImporter::Import(ResourceImportContext &context)
        {
            ResourceReader resource( context.resource->GetSourceFileName( ) );
            RigReader reader;

            return reader.Read( resource );
        }
    }
}
