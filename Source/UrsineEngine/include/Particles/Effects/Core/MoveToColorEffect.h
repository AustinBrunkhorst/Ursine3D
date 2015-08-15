////////////////////////////////////////////////////////////////////////////////
#ifndef MOVE_TO_COLOR_EFFECT_H
#define MOVE_TO_COLOR_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#include "ParticleEffect.h"
#include "Color.h"
#include "TimeSpan.h"

namespace Ursine
{
    class MoveToColorEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(MoveToColorEffect, _start);
        ParticleEffectSerializerAllow(MoveToColorEffect, _middle);
        ParticleEffectSerializerAllow(MoveToColorEffect, _end);
        std::vector<Color> _start, _middle, _end;

        ParticleEffectSerializerAllow(MoveToColorEffect, _first_delay);
        ParticleEffectSerializerAllow(MoveToColorEffect, _first_duration);
        ParticleEffectSerializerAllow(MoveToColorEffect, _second_delay);
        ParticleEffectSerializerAllow(MoveToColorEffect, _second_duration);
        TimeSpan _first_delay, _first_duration,
                 _second_delay, _second_duration;

    public:
        MoveToColorEffect(void)
            : BaseParticleEffect()
            , _first_delay(1)
            , _first_duration(1)
            , _second_delay(1)
            , _second_duration(1) { }

        void InitializeParticle(ECS::Entity *particle) const override;

        void AddStartColor(const Color &color);
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !MOVE_TO_COLOR_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef MOVE_TO_COLOR_REGISTERED
#define MOVE_TO_COLOR_REGISTERED

namespace Ursine
{
    RegisterParticleEffect(MoveToColorEffect);
    RegisterParticleEffectData(MoveToColorEffect, _start,           "start");
    RegisterParticleEffectData(MoveToColorEffect, _middle,          "middle");
    RegisterParticleEffectData(MoveToColorEffect, _end,             "end");
    RegisterParticleEffectData(MoveToColorEffect, _first_delay,     "first_delay");
    RegisterParticleEffectData(MoveToColorEffect, _first_duration,  "first_duration");
    RegisterParticleEffectData(MoveToColorEffect, _second_delay,    "second_delay");
    RegisterParticleEffectData(MoveToColorEffect, _second_duration, "second_duration");
}

#endif

#endif
