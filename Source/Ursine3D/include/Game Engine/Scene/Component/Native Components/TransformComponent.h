#pragma once

#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        class Transform : public Component 
        {
            NATIVE_COMPONENT;

        public:
            Transform(void) 
                : BaseComponent( ) { }
        } Meta(Enable, DisplayName( "Transform" ));
    }
}