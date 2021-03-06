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

            // When this entities hierarchy has finished serializing
            ENTITY_HIERARCHY_SERIALIZED,

            // Before an entity is removed from the world
            ENTITY_REMOVED,

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
            // Animation
            ///////////////////////////////////////////////////////////////////

            // This event is sent when a non-looping animation finishes
            ENTITY_ANIMATION_FINISH,

            // This is sent when a animation state's event is triggered
            ENTITY_ANIMATION_STATE_EVENT,

            // This event is sent when animation bone manipulation is allowed
            // through editing the bone's transform component
            ENTITY_ANIMATION_BONE_MANIPULATION_VALID,

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
        } Meta(Enable);
    }
}
