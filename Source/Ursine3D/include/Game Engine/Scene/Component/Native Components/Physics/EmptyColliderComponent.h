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
        
        } Meta(Enable, HiddenInInspector);
    }
}
