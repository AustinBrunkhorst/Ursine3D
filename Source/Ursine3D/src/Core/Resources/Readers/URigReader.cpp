/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** URigReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "URigReader.h"

#include "URigData.h"

namespace ursine
{
    namespace resources
    {
        URigReader::URigReader(void) { }

        ResourceData::Handle URigReader::Read(ResourceReader &input)
        {
            // TODO: [J] input.read(shit);

            return std::make_shared<URigData>( /* shit */ );
        }
    }
}
