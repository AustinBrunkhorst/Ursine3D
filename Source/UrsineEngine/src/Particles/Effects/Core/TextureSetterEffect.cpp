#include "UrsinePrecompiled.h"
#include "TextureSetterEffect.h"
#include "RenderableComponent.h"

namespace ursine
{
    void TextureSetterEffect::InitializeParticle(ecs::Entity *particle) const
    {
        auto renderable = particle->GetComponent<ecs::Renderable>();

        uint index = rand() % _textures.size();

        if (renderable)
            renderable->SetTexture(_textures[index]);
    }
}
