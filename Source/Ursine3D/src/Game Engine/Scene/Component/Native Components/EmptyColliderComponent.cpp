#include "UrsinePrecompiled.h"

#include "EmptyColliderComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(EmptyCollider);

        EmptyCollider::EmptyCollider(void)
            : BaseComponent( ) { }
    }
}
