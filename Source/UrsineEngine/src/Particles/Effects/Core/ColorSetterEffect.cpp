#include "UrsinePrecompiled.h"
#include "ColorSetterEffect.h"
#include "RenderableComponent.h"

namespace ursine
{
    void ColorSetterEffect::InitializeParticle(ecs::Entity *particle) const
    {
        auto renderable = particle->GetComponent<ecs::Renderable>();
        uint index = rand() % _colors.size();

        renderable->SetTint(_colors[index]);
    }
}
