/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** HierarchyEvents.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EventArgs.h"

namespace ursine
{
    namespace ecs
    {
        struct ParentChangedArgs : EventArgs
        {
            EntityHandle newParent;
            EntityHandle oldParent;
      
            ParentChangedArgs(const EntityHandle &newParent, const EntityHandle &oldParent);
        };
    }
}
