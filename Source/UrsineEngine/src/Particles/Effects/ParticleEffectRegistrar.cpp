/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectRegistrar.cpp
** - Register particle effects
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ParticleEffectRegistrar.h"
#include "ParticleEffect.h"

namespace ursine
{
    ParticleEffect *ParticleEffectRegistrar::Instantiate(const std::string &name)
    {
        auto map = getOperatorMap();

        // this puppy doesn't exist
        if (map.find(name) == map.end())
            return nullptr;

        return map[name].second.Create();
    }

    ParticleEffect *ParticleEffectRegistrar::Copy(const ParticleEffect *instance)
    {
        auto map = getOperatorMap();
        auto &name = instance->GetName();

        // this puppy doesn't exist
        if (map.find(name) == map.end())
            return nullptr;

        return map[name].second.Copy(instance);
    }
}