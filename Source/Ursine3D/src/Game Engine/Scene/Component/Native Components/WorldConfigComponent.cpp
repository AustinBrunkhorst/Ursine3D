#include "UrsinePrecompiled.h"

#include "WorldConfigComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( WorldConfig );

        WorldConfig::WorldConfig(void)
            : BaseComponent( )
        {
            
        }

        WorldConfig::~WorldConfig(void)
        {
            
        }

        void WorldConfig::OnInitialize(void)
        {
            Component::OnInitialize( );
        }
    }
}