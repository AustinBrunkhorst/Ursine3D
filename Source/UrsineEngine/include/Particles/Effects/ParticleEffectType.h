/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectType.h
** - Stores particle effect type information
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ParticleEffectConfig.h"
#include "JsonSerializer.h"

namespace Ursine
{
    template <class Type>
    class ParticleEffectType final
    {
    public:
        // ID of this effect type
        static ParticleEffectTypeID ID;

        // Bit mask for this effect type
        static ParticleEffectTypeMask Mask;

        // Determines if this type has been registered with the effect registrar
        static bool Registered;

        // Registers a effect
        ParticleEffectType(const char *name);

        // Registers a component member
        ParticleEffectType(const char *member_name,
                           ParticleEffectDataAccessor accessor,
                           JsonSerializer::Serializer serializer,
                           JsonSerializer::Deserializer deserializer,
                           const char *display_name = nullptr);

        static ParticleEffect *Create(void);
        static ParticleEffect *Copy(const ParticleEffect *instance);
    };
}

#include "ParticleEffectType.hpp"