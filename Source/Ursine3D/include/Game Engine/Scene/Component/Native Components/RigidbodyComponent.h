#pragma once

#include "Component.h"
#include "Rigidbody.h"

namespace ursine
{
    namespace ecs
    {
        class Rigidbody : public Component
        {
            NATIVE_COMPONENT;

            friend class PhysicsSystem;

        public:
            Rigidbody(void);

        private:
            physics::Rigidbody m_rigidbody;

        } Meta( Enable, WhiteListMethods, DisplayName( "Rigidbody" ) );
    }
}
