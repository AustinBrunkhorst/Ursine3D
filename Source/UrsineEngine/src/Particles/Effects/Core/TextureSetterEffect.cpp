#include "UrsinePrecompiled.h"
#include "TextureSetterEffect.h"
#include "RenderableComponent.h"

namespace Ursine
{
    void TextureSetterEffect::InitializeParticle(ECS::Entity *particle) const
    {
        auto renderable = particle->GetComponent<ECS::Renderable>();

        uint index = rand() % _textures.size();

        if (renderable)
            renderable->SetTexture(_textures[index]);
    }
}
