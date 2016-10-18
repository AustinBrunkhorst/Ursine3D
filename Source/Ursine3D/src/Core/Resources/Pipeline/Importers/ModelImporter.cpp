/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelImporter.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ModelImporter.h"

#include "ResourceReader.h"
#include "ResourceImportContext.h"

#include "ModelData.h"
#include "ModelReader.h"

namespace ursine
{
    namespace resources
    {
        rp::ModelImporter::ModelImporter(void)
        {
        }

        ResourceData::Handle rp::ModelImporter::Import(ResourceImportContext &context)
        {
            ResourceReader resource( context.resource->GetSourceFileName( ) );
            ModelReader reader;

            return reader.Read( resource );
        }
    }
}
