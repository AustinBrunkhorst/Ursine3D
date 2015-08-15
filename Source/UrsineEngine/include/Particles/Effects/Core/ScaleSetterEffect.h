////////////////////////////////////////////////////////////////////////////////
#ifndef SCALE_SETTER_EFFECT_H
#define SCALE_SETTER_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#include "ParticleEffect.h"
#include "Randomizer.h"

namespace Ursine
{
    class ScaleSetterEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(ScaleSetterEffect, _randomizer);
        Randomizer _randomizer;

    public:
        ScaleSetterEffect(void) 
            : BaseParticleEffect() { }

        void InitializeParticle(ECS::Entity *particle) const override;
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !SCALE_SETTER_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef SCALE_SETTER_REGISTERED
#define SCALE_SETTER_REGISTERED

namespace Ursine
{
    RegisterParticleEffect(ScaleSetterEffect);
    RegisterParticleEffectData(ScaleSetterEffect, _randomizer, "randomizer");
}

#endif

#endif
