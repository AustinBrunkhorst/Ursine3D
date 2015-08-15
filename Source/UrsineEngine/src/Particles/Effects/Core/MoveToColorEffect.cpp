#include "UrsinePrecompiled.h"
#include "MoveToColorEffect.h"
#include "RenderableComponent.h"

namespace Ursine
{
    void MoveToColorEffect::AddStartColor(const Color &color)
    {
        _start.push_back(color);
    }

    void MoveToColorEffect::InitializeParticle(ECS::Entity *particle) const
    {
        auto renderable = particle->GetComponent<ECS::Renderable>();

        Color start_color, middle_color, end_color;

        if (_start.size())
        {
            uint start_index = rand() % _start.size();
            start_color = _start[start_index];
        }

        if (_middle.size())
        {
            uint middle_index = rand() % _middle.size();
            middle_color = _middle[middle_index];
        }

        if (_end.size())
        {
            uint end_index = rand() % _end.size();
            end_color = _end[end_index];
        }

        renderable->SetTint(start_color);

        // create the tween
        particle->GetTweens().Create()
            .BeginGroup()
                .Delay(_first_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(renderable, &ECS::Renderable::SetTint, start_color, middle_color, _first_duration)
            .EndGroup()
            .BeginGroup()
                .Delay(_second_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(renderable, &ECS::Renderable::SetTint, middle_color, end_color, _second_duration)
            .EndGroup();
    }
}
