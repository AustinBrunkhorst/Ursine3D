/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectReflection.h
** - Reflect particle effect data
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ParticleEffectConfig.h"

namespace Ursine
{
    struct ParticleEffectReflection
    {
        std::string name;
        ParticleEffectTypeMask mask;
        ParticleEffecDataMap members;

        ParticleEffectReflection(void) = default;
    };
}
