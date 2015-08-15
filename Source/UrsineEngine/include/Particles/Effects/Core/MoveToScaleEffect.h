////////////////////////////////////////////////////////////////////////////////
#ifndef MOVE_TO_SCALE_EFFECT_H
#define MOVE_TO_SCALE_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#include "ParticleEffect.h"
#include "Randomizer.h"
#include "TimeSpan.h"

namespace Ursine
{
    class MoveToScaleEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(MoveToScaleEffect, _uniform_scaling);
        bool _uniform_scaling;

        ParticleEffectSerializerAllow(MoveToScaleEffect, _start);
        ParticleEffectSerializerAllow(MoveToScaleEffect, _middle);
        ParticleEffectSerializerAllow(MoveToScaleEffect, _end);
        Randomizer _start, _middle, _end;

        ParticleEffectSerializerAllow(MoveToScaleEffect, _first_delay);
        ParticleEffectSerializerAllow(MoveToScaleEffect, _first_duration);
        ParticleEffectSerializerAllow(MoveToScaleEffect, _second_delay);
        ParticleEffectSerializerAllow(MoveToScaleEffect, _second_duration);
        TimeSpan _first_delay, _first_duration, 
                 _second_delay, _second_duration;

    public:
        MoveToScaleEffect(void)
            : BaseParticleEffect()
            , _first_delay(1)
            , _first_duration(1)
            , _second_delay(1)
            , _second_duration(1) { }

        void InitializeParticle(ECS::Entity *particle) const override;
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !MOVE_TO_SCALE_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef MOVE_TO_SCALE_REGISTERED
#define MOVE_TO_SCALE_REGISTERED

namespace Ursine
{
    RegisterParticleEffect(MoveToScaleEffect);
    RegisterParticleEffectData(MoveToScaleEffect, _uniform_scaling, "uniform_scaling");
    RegisterParticleEffectData(MoveToScaleEffect, _start,           "start");
    RegisterParticleEffectData(MoveToScaleEffect, _middle,          "middle");
    RegisterParticleEffectData(MoveToScaleEffect, _end,             "end");
    RegisterParticleEffectData(MoveToScaleEffect, _first_delay,     "first_delay");
    RegisterParticleEffectData(MoveToScaleEffect, _first_duration,  "first_duration");
    RegisterParticleEffectData(MoveToScaleEffect, _second_delay,    "second_delay");
    RegisterParticleEffectData(MoveToScaleEffect, _second_duration, "second_duration");
}

#endif

#endif
