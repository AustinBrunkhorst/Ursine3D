/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HierarchyEvents.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "HierarchyEvents.h"
#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        ParentChangedArgs::ParentChangedArgs(Entity* newParent, Entity* oldParent)
            : newParent( newParent ? newParent->GetID( ) : -1 )
		    , oldParent( oldParent ? oldParent->GetID( ) : -1 ) { }
    }
}