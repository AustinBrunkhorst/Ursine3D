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
			~Body(void);

			void OnInitialize(void) override;

        private:
            physics::Body m_body;

			void onTransformChange(EVENT_HANDLER(Entity));

        } Meta( Enable, WhiteListMethods );
    }
}
