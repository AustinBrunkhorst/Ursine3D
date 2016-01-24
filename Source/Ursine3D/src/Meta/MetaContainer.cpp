/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MetaContainer.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "MetaContainer.h"
#include "MetaManager.h"

namespace ursine
{
    namespace meta
    {
        const MetaManager &MetaContainer::GetMeta(void) const
        {
            return m_meta;
        }
    }
}