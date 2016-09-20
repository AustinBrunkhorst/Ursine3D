/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UModelReader.h"

namespace ursine
{
    namespace resources
    {
        UModelReader::UModelReader(void) { }

        ResourceData::Handle UModelReader::Read(ResourceReader &input)
        {
            // TODO: [J] input.read(shit);

            return std::make_shared<UModelData>( /* shit */ );
        }
    }
}
