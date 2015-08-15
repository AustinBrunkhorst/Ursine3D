/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffect.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ParticleEffectType.h"
#include "ParticleEffectRegistrar.h"
#include "Entity.h"

namespace Ursine
{
    namespace ECS
    {
        class ParticleEmitter;
    }

    class ParticleEffect
    {
        friend class ECS::ParticleEmitter;

        ECS::ParticleEmitter *_emitter;

        ParticleEffectTypeID _type_id;

        // (1 << _type_id)
        ParticleEffectTypeMask _type_mask;

    public:
        inline ParticleEffect(ParticleEffectTypeID id);

        ParticleEffect(const ParticleEffect &rhs) = default;
        ParticleEffect &operator=(const ParticleEffect &rhs) = default;

        virtual ~ParticleEffect(void) { }

        // Called when the Particle Effect has been initialized and added to an emitter
        virtual void OnInitialize(void) { }

        // Called when the particle effect has been removed from the emitter
        virtual void OnRemove(void) { }

        // override this function and implement your initialization code
        virtual void InitializeParticle(ECS::Entity *particle) const = 0;

        ////////////////////////////////////
        // Reflection related
        ////////////////////////////////////

        inline ParticleEffectTypeID GetTypeID(void) const;

        inline ParticleEffectTypeMask GetTypeMask(void) const;

        inline const std::string &GetName(void) const;

        inline ECS::ParticleEmitter *GetEmitter(void) const;

        // Determines if this effect is of the specified type
        template <class ParticleEffectType>
        inline bool Is(void) const;
    };
}

#include "ParticleEffect.hpp"
