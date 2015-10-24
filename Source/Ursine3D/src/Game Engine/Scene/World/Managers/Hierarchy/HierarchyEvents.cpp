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