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
			~Rigidbody(void);

			void OnInitialize(void) override;

        private:
            physics::Rigidbody m_rigidbody;

			void onTransformChange(EVENT_HANDLER(Entity));

        } Meta( Enable, WhiteListMethods, DisplayName( "Rigidbody" ) );
    }
}
