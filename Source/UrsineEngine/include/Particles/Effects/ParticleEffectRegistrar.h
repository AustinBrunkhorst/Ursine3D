/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectRegistrar.h
** - Register particle effects
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineLogTools.h"

#include "ParticleEffectConfig.h"
#include "ParticleEffectOperator.h"
#include "ParticleEffectReflection.h"

#include "JsonSerializer.h"

#include <unordered_map>

namespace Ursine
{
    namespace ECS
    {
        class ParticleEmitter;
    }

    class ParticleEffectRegistrar final
    {
        typedef std::unordered_map<ParticleEffectTypeID, ParticleEffectReflection> TypeMap;
        typedef std::unordered_map<std::string, std::pair<ParticleEffectTypeID, ParticleEffectOperator>> OperatorMap;

        static TypeMap &getTypeMap(void);
        static OperatorMap &getOperatorMap(void);

        static ParticleEffectTypeID &getNextID(void);

    public:
        template <class Type>
        static void Register(const char *name);

        template <class Type>
        static void RegisterMember(const char *member_name,
                                   ParticleEffectDataAccessor accessor,
                                   JsonSerializer::Serializer serializer,
                                   JsonSerializer::Deserializer deserializer);

        // Gets the unique ID for this particle effect type
        template<class Type>
        static inline ParticleEffectTypeID GetID(void);

        // Gets the unique type mask for this particle effect type
        template<class Type>
        static inline ParticleEffectTypeMask GetMask(void);

        // Gets the unique particle effect type mask with this name
        // returns false if the particle effect type doesn't exist
        static inline bool GetMask(const std::string &name, ParticleEffectTypeMask &mask_out);

        template<class Type>
        static inline bool IsRegistered(void);

        // Gets particle effect reflection data for this particle effect type
        static inline const ParticleEffectReflection &Reflect(ParticleEffectTypeID id);

        // Instantiates a particle effect based on its string name
        static ParticleEffect *Instantiate(const std::string &name);

        // Creates a copy of the particle effect instance
        static ParticleEffect *Copy(const ParticleEffect *instance);
    };
}

#include "ParticleEffectRegistrar.hpp"
