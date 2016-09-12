/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EmptyColliderComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "EmptyCollider.h"

namespace ursine
{
    namespace ecs
    {
        class EmptyCollider : public Component
        {
            NATIVE_COMPONENT;

            friend class PhysicsSystem;

        public:
            EmptyCollider(void);

        private:
            physics::EmptyCollider m_emptyCollider;
        
        } Meta(
            Enable, 
            DisableSerialization
        ) EditorMeta(
            HiddenInInspector
        );
    }
}
