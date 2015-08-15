#include "UrsinePrecompiled.h"
#include "ColorSetterEffect.h"
#include "RenderableComponent.h"

namespace Ursine
{
    void ColorSetterEffect::InitializeParticle(ECS::Entity *particle) const
    {
        auto renderable = particle->GetComponent<ECS::Renderable>();
        uint index = rand() % _colors.size();

        renderable->SetTint(_colors[index]);
    }
}
