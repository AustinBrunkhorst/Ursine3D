#pragma once

#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        class Rigidbody : public Component
        {
            NATIVE_COMPONENT;

        public:
            Rigidbody(void);

        private:
            // private tings

        } Meta(Enable, WhiteListMethods, DisplayName( "Rigidbody" ));
    }
}
