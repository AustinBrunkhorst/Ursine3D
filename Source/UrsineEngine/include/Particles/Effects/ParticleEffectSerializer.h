/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectSerializer.h
** - Serialize and deserialize particle effects
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Json.h"

namespace Ursine
{
    class ParticleEffect;

    class ParticleEffectSerializer final
    {
    public:
        static Json Serialize(const ParticleEffect *effect);
        static ParticleEffect *Deserialize(const Json &obj);
    };
}
