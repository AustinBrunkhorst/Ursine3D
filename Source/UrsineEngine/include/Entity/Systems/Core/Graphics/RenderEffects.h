#pragma once

#include "DefaultRenderEffect.h"
#include "BlurEffect.h"
#include "WaveEffect.h"

namespace ursine
{
    namespace ecs
    {
        class RenderEffect;

        enum RenderEffectType
        {
            FX_DEFAULT,
            FX_BLUR,
            FX_WAVE,
            // this is actually used
            FX_COUNT
        };

        RenderEffect *GetRenderEffect(RenderEffectType type);
    }
}