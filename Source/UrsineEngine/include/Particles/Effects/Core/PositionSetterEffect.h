////////////////////////////////////////////////////////////////////////////////
#ifndef POSITION_SETTER_EFFECT_H
#define POSITION_SETTER_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#include "ParticleEffect.h"
#include "Randomizer.h"

namespace Ursine
{
    class PositionSetterEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(PositionSetterEffect, _randomizer);
        Randomizer _randomizer;

    public:
        PositionSetterEffect(void)
            : BaseParticleEffect() { }

        void InitializeParticle(ECS::Entity *particle) const override;
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !POSITION_SETTER_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef POSITION_SETTER_REGISTERED
#define POSITION_SETTER_REGISTERED

namespace Ursine
{
    RegisterParticleEffect(PositionSetterEffect);
    RegisterParticleEffectData(PositionSetterEffect, _randomizer, "randomizer");
}

#endif

#endif
