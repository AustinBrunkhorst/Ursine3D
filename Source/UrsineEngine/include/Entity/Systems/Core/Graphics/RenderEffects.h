#pragma once

#include "DefaultRenderEffect.h"
#include "BlurEffect.h"
#include "WaveEffect.h"

namespace Ursine
{
    namespace ECS
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