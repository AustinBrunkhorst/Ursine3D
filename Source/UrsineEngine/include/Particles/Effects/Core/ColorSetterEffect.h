////////////////////////////////////////////////////////////////////////////////
#ifndef COLOR_SETTER_EFFECT_H
#define COLOR_SETTER_EFFECT_H
////////////////////////////////////////////////////////////////////////////////
#include "ParticleEffect.h"

namespace Ursine
{
    class ColorSetterEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(ColorSetterEffect, _colors);
        std::vector<Color> _colors;

    public:
        ColorSetterEffect(void)
            : BaseParticleEffect() { }

        void InitializeParticle(ECS::Entity *particle) const override;
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !COLOR_SETTER_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef COLOR_SETTER_EFFECT_REGISTERED
#define COLOR_SETTER_EFFECT_REGISTERED

namespace Ursine
{
    RegisterParticleEffect(ColorSetterEffect);
    RegisterParticleEffectData(ColorSetterEffect, _colors, "colors");
}

#endif

#endif
