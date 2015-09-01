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

namespace ursine
{
    ParticleEffect::ParticleEffect(ParticleEffectTypeID type_id)
        : _type_id(type_id)
        , _type_mask(1ull << type_id) { }

    ParticleEffectTypeID ParticleEffect::GetTypeID(void) const
    {
        return _type_id;
    }

    ParticleEffectTypeMask ParticleEffect::GetTypeMask(void) const
    {
        return _type_mask;
    }

    ecs::ParticleEmitter *ParticleEffect::GetEmitter(void) const
    {
        return _emitter;
    }

    const std::string &ParticleEffect::GetName(void) const
    {
        return ParticleEffectRegistrar::Reflect(_type_id).name;
    }

    template<class ParticleEffectType>
    bool ParticleEffect::Is(void) const
    {
        static_assert(std::is_base_of<ParticleEffect, ParticleEffectType>::value,
            "The specified type is not a component!");

        static const auto id = GetParticleEffectID(ParticleEffectType);

        return _type_id == id;
    }
}
