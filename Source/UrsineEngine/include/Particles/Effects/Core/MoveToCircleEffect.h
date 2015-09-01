////////////////////////////////////////////////////////////////////////////////
#ifndef MOVE_TO_CIRCLE_EFFECT_H
#define MOVE_TO_CIRCLE_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#include "ParticleEffect.h"
#include "Randomizer.h"
#include "TimeSpan.h"

namespace ursine
{
    class MoveToCircleEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(MoveToCircleEffect, _start_angle);
        ParticleEffectSerializerAllow(MoveToCircleEffect, _start_radius);
        ParticleEffectSerializerAllow(MoveToCircleEffect, _middle_angle);
        ParticleEffectSerializerAllow(MoveToCircleEffect, _middle_radius);
        ParticleEffectSerializerAllow(MoveToCircleEffect, _end_angle);
        ParticleEffectSerializerAllow(MoveToCircleEffect, _end_radius);
        Randomizer _start_angle, _start_radius,
                   _middle_angle, _middle_radius,
                   _end_angle, _end_radius;

        ParticleEffectSerializerAllow(MoveToCircleEffect, _first_delay);
        ParticleEffectSerializerAllow(MoveToCircleEffect, _first_duration);
        ParticleEffectSerializerAllow(MoveToCircleEffect, _second_delay);
        ParticleEffectSerializerAllow(MoveToCircleEffect, _second_duration);
        TimeSpan _first_delay, _first_duration,
                 _second_delay, _second_duration;

    public:
        MoveToCircleEffect(void)
            : BaseParticleEffect()
            , _first_delay(1)
            , _first_duration(1)
            , _second_delay(1)
            , _second_duration(1) { }

        void InitializeParticle(ecs::Entity *particle) const override;

        void SetMiddleRadius(Randomizer randomizer);
        void SetMiddleAngle(Randomizer randomizer);
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !MOVE_TO_CIRCLE_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef MOVE_TO_CIRCLE_REGISTERED
#define MOVE_TO_CIRCLE_REGISTERED

namespace ursine
{
    RegisterParticleEffect(MoveToCircleEffect);
    RegisterParticleEffectData(MoveToCircleEffect, _start_angle, "start_angle");
    RegisterParticleEffectData(MoveToCircleEffect, _start_radius, "start_radius");
    RegisterParticleEffectData(MoveToCircleEffect, _middle_angle, "middle_angle");
    RegisterParticleEffectData(MoveToCircleEffect, _middle_radius, "middle_radius");
    RegisterParticleEffectData(MoveToCircleEffect, _end_angle, "end_angle");
    RegisterParticleEffectData(MoveToCircleEffect, _end_radius, "end_radius");
    RegisterParticleEffectData(MoveToCircleEffect, _first_delay, "first_delay");
    RegisterParticleEffectData(MoveToCircleEffect, _first_duration, "first_duration");
    RegisterParticleEffectData(MoveToCircleEffect, _second_delay, "second_delay");
    RegisterParticleEffectData(MoveToCircleEffect, _second_duration, "second_duration");
}

#endif

#endif
