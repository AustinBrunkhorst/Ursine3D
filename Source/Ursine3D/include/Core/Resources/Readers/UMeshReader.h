/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UMeshReader.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        // TODO: [J] DO this!
        class UMeshReader //: public ResourceTypeReader
        {
            // RESOURCE_TYPE_READER;

            // TODO: [J] Remove this
            friend class UModelReader;

            UMeshData::Handle Read(ResourceReader &input); // override;
        };
    }
}
