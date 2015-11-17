#include "EventArgs.h"

namespace ursine
{
    namespace ecs
    {
        enum EntityEventType
        {
            ///////////////////////////////////////////////////////////////////
            // Lifetime
            ///////////////////////////////////////////////////////////////////

            // Before an entity is removed from the world
            ENTITY_REMOVED,

            ///////////////////////////////////////////////////////////////////
            // Components
            ///////////////////////////////////////////////////////////////////

            // A new component was added
            ENTITY_COMPONENT_ADDED,

            // An existing component was removed
            ENTITY_COMPONENT_REMOVED,

            ///////////////////////////////////////////////////////////////////
            // Transform
            ///////////////////////////////////////////////////////////////////

            // A transform was invalidated
            ENTITY_TRANSFORM_DIRTY,

            // An entities parent has changed
            ENTITY_PARENT_CHANGED,

            ///////////////////////////////////////////////////////////////////
            // User Defined
            ///////////////////////////////////////////////////////////////////

            // Starting index for events defined out of engine
            ENTITY_EVENT_USER = 0x8000,
        };
    }
}
