/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** URigImporter.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "URigImporter.h"

#include "ResourceReader.h"
#include "ResourceImportContext.h"

#include "URigData.h"
#include "URigReader.h"

namespace ursine
{
    namespace resources
    {
        rp::URigImporter::URigImporter(void)
        {
        }

        ResourceData::Handle rp::URigImporter::Import(ResourceImportContext &context)
        {
            ResourceReader resource( context.resource->GetSourceFileName( ) );
            URigReader reader;

            return reader.Read( resource );
        }
    }
}
