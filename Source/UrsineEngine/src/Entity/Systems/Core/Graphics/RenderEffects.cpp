#include "UrsinePrecompiled.h"

#include "RenderEffects.h"

namespace Ursine
{
    namespace ECS
    {
        RenderEffect *GetRenderEffect(RenderEffectType type)
        {
            static RenderEffect *map [] = {
                // FX_DEFAULT
                new DefaultRenderEffect( ),
                // FX_BLUR
                new BlurEffect( ),
                // FX_WAVE
                new WaveEffect( )
            };

            return map[ type ];
        }
    }
}