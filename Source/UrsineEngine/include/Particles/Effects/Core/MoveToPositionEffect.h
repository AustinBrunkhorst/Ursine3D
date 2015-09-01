////////////////////////////////////////////////////////////////////////////////
#ifndef MOVE_TO_POSITION_EFFECT_H
#define MOVE_TO_POSITION_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#include "ParticleEffect.h"
#include "Randomizer.h"
#include "TimeSpan.h"

namespace ursine
{
    class MoveToPositionEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(MoveToPositionEffect, _start);
        ParticleEffectSerializerAllow(MoveToPositionEffect, _middle);
        ParticleEffectSerializerAllow(MoveToPositionEffect, _end);
        Randomizer _start, _middle, _end;

        ParticleEffectSerializerAllow(MoveToPositionEffect, _first_delay);
        ParticleEffectSerializerAllow(MoveToPositionEffect, _first_duration);
        ParticleEffectSerializerAllow(MoveToPositionEffect, _second_delay);
        ParticleEffectSerializerAllow(MoveToPositionEffect, _second_duration);
        TimeSpan _first_delay, _first_duration,
                 _second_delay, _second_duration;

    public:
        MoveToPositionEffect(void)
            : BaseParticleEffect()
            , _first_delay(1)
            , _first_duration(1)
            , _second_delay(1)
            , _second_duration(1) { }

        void InitializeParticle(ecs::Entity *particle) const override;
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !MOVE_TO_POSITION_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef MOVE_TO_POSITION_REGISTERED
#define MOVE_TO_POSITION_REGISTERED

namespace ursine
{
    RegisterParticleEffect(MoveToPositionEffect);
    RegisterParticleEffectData(MoveToPositionEffect, _start,            "start");
    RegisterParticleEffectData(MoveToPositionEffect, _middle,           "middle");
    RegisterParticleEffectData(MoveToPositionEffect, _end,              "end");
    RegisterParticleEffectData(MoveToPositionEffect, _first_delay,      "first_delay");
    RegisterParticleEffectData(MoveToPositionEffect, _first_duration,   "first_duration");
    RegisterParticleEffectData(MoveToPositionEffect, _second_delay,     "second_delay");
    RegisterParticleEffectData(MoveToPositionEffect, _second_duration,  "second_duration");
}

#endif

#endif