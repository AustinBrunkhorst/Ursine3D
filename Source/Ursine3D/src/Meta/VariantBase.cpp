/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VariantBase.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "VariantBase.h"

#include "ArrayWrapper.h"

namespace ursine
{
    namespace meta
    {
        bool VariantBase::IsArray(void) const
        {
            return false;
        }

        ArrayWrapper VariantBase::GetArray(void) const
        {
            // invalid wrapper
            return { };
        }
    }
}