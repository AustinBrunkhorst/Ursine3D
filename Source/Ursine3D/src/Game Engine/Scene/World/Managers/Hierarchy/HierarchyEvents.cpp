#include "UrsinePrecompiled.h"

#include "HierarchyEvents.h"
#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        ParentChangedArgs::ParentChangedArgs(Entity* newParent)
            : newParent(newParent ? newParent->GetID() : -1) { }
    }
}