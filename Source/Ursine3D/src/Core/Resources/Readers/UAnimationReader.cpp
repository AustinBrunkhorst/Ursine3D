/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UAnimationReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UAnimationReader.h"

namespace ursine
{
    namespace resources
    {
        UAnimationReader::UAnimationReader(void) { }

        ResourceData::Handle UAnimationReader::Read(ResourceReader &input)
        {
            // TODO: [J] input.read(shit);

            return std::make_shared<UAnimationData>( /* shit */ );
        }
    }
}
