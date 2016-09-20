/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** U3DContentReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "U3DContentReader.h"

namespace ursine
{
    namespace resources
    {
        U3DContentReader::U3DContentReader(void) { }

        ResourceData::Handle UAnimationReader::Read(ResourceReader &input)
        {
            // TODO: [J] input.read(shit);

            return std::make_shared<U3DContentData>( /* shit */ );
        }
    }
}
