#pragma once

#include "Component.h"
#include "Body.h"

namespace ursine
{
    namespace ecs
    {
        class Body : public Component
        {
            NATIVE_COMPONENT;

            friend class PhysicsSystem;

        public:
            Body(void);

        private:
            physics::Body m_body;

        } Meta( Enable, WhiteListMethods );
    }
}
