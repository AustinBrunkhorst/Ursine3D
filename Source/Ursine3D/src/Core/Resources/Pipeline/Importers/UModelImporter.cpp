/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelImporter.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UModelImporter.h"

#include "ResourceReader.h"
#include "ResourceImportContext.h"

#include "UModelData.h"
#include "UModelReader.h"

namespace ursine
{
    namespace resources
    {
        rp::UModelImporter::UModelImporter(void)
        {
        }

        ResourceData::Handle rp::UModelImporter::Import(ResourceImportContext &context)
        {
            ResourceReader resource( context.resource->GetSourceFileName( ) );
            UModelReader reader;

            return reader.Read( resource );
        }
    }
}
