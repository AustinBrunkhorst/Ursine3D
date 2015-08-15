////////////////////////////////////////////////////////////////////////////////
#ifndef LIFE_SPAN_EFFECT_H
#define LIFE_SPAN_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#include "ParticleEffect.h"
#include "TimeSpan.h"

namespace Ursine
{
    class LifeSpanEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(LifeSpanEffect, _life_span);
        TimeSpan _life_span;

    public:
        LifeSpanEffect(void)
            : BaseParticleEffect()
            , _life_span(1) { }

        void InitializeParticle(ECS::Entity *particle) const override;
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !LIFE_SPAN_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef LIFE_SPAN_REGISTERED
#define LIFE_SPAN_REGISTERED

namespace Ursine
{
    RegisterParticleEffect(LifeSpanEffect);
    RegisterParticleEffectData(LifeSpanEffect, _life_span, "life_span");
}

#endif

#endif
