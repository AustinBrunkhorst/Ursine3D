#include "UrsinePrecompiled.h"

#include "AudioConfigComponent.h"

#include "AudioSystem.h"
#include "WorldEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( AudioConfig );

        AudioConfig::AudioConfig(void)
            : BaseComponent( )
        {
            
        }
    }
}
