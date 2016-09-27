/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UAnimationData.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UAnimationData.h"

#include "UAnimationReader.h"

namespace ursine
{
    namespace resources
    {
        void UAnimationData::Write(pipeline::ResourceWriter &output)
        {
            
        }

        meta::Type UAnimationData::GetReaderType(void)
        {
            return typeof(UAnimationReader);
        }
    }
}
