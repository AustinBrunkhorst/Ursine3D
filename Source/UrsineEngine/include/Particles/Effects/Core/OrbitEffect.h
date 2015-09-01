////////////////////////////////////////////////////////////////////////////////
#ifndef ORBIT_EFFECT_H
#define ORBIT_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#include "ParticleEffect.h"
#include "Randomizer.h"
#include "TimeSpan.h"

namespace ursine
{
    class OrbitEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(OrbitEffect, _start);
        ParticleEffectSerializerAllow(OrbitEffect, _middle);
        ParticleEffectSerializerAllow(OrbitEffect, _end);
        Randomizer _start, _middle, _end;

        ParticleEffectSerializerAllow(OrbitEffect, _first_delay);
        ParticleEffectSerializerAllow(OrbitEffect, _first_duration);
        ParticleEffectSerializerAllow(OrbitEffect, _second_delay);
        ParticleEffectSerializerAllow(OrbitEffect, _second_duration);
        TimeSpan _first_delay, _first_duration,
                 _second_delay, _second_duration;

        ParticleEffectSerializerAllow(OrbitEffect, _speed);
        // radians per second
        float _speed;

    public:
        OrbitEffect(void)
            : BaseParticleEffect()
            , _first_delay(1)
            , _first_duration(1)
            , _second_delay(1)
            , _second_duration(1) { }

        void InitializeParticle(ecs::Entity *particle) const override;
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !ORBIT_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef ORBIT_REGISTERED
#define ORBIT_REGISTERED

namespace ursine
{
    RegisterParticleEffect(OrbitEffect);
    RegisterParticleEffectData(OrbitEffect, _start,           "start");
    RegisterParticleEffectData(OrbitEffect, _middle,          "middle");
    RegisterParticleEffectData(OrbitEffect, _end,             "end");
    RegisterParticleEffectData(OrbitEffect, _first_delay,     "first_delay");
    RegisterParticleEffectData(OrbitEffect, _first_duration,  "first_duration");
    RegisterParticleEffectData(OrbitEffect, _second_delay,    "second_delay");
    RegisterParticleEffectData(OrbitEffect, _second_duration, "second_duration");
    RegisterParticleEffectData(OrbitEffect, _speed,           "speed");
}

#endif

#endif
