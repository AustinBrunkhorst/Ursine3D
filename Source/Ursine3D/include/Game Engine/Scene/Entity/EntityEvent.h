/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityEvent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
#pragma once

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
			// Physics
			///////////////////////////////////////////////////////////////////

			// A collision is persisting between this entity and the other entity.
			// NOTE: This event is only sent when one of the two objects has a 
			// non-kinematic (or static) rigidbody attached.
			ENTITY_COLLISION_PERSISTED,

            ///////////////////////////////////////////////////////////////////
            // Particle Systems
            ///////////////////////////////////////////////////////////////////
            // Commands all particle modifiers to operate on the set of 
            // particles
            ENTITY_PARTICLE_UPDATE,

            ///////////////////////////////////////////////////////////////////
            // User Defined
            ///////////////////////////////////////////////////////////////////

            // Starting index for events defined out of engine
            ENTITY_EVENT_USER = 0x8000,
        };
    }
}
