////////////////////////////////////////////////////////////////////////////////
#ifndef TEXTURE_SETTER_EFFECT_H
#define TEXTURE_SETTER_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#include "ParticleEffect.h"
#include "Texture2D.h"

namespace ursine
{
    class TextureSetterEffect : public ParticleEffect
    {
        ParticleEffectSerializerAllow(TextureSetterEffect, _textures);
        std::vector<Texture2D *> _textures;

    public:
        TextureSetterEffect(void)
            : BaseParticleEffect() { }

        void InitializeParticle(ecs::Entity *particle) const override;
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // !TEXTURE_SETTER_EFFECT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_PARTICLE_EFFECT

#ifndef TEXTURE_SETTER_REGISTERED
#define TEXTURE_SETTER_REGISTERED

namespace ursine
{
    RegisterParticleEffect(TextureSetterEffect);
    RegisterParticleEffectData(TextureSetterEffect, _textures, "textures");
}

#endif

#endif
