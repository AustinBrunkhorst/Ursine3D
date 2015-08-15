#include "UrsinePrecompiled.h"
#include "OrbitEffect.h"
#include "OrbitDataComponent.h"
#include "ParticleEmitterComponent.h"

namespace Ursine
{
    void OrbitEffect::InitializeParticle(ECS::Entity *particle) const
    {
        auto trans = particle->GetTransform();
        auto emitter_trans = GetEmitter()->GetOwner()->GetTransform();

        auto center = emitter_trans->PositionWC();
        float scale_x = emitter_trans->ScaleWC().x;

        float start = scale_x * _start.GetValue(),
              middle = scale_x * _middle.GetValue(),
              end = scale_x * _end.GetValue();

        // set the starting radius
        auto data = particle->AddComponent<ECS::OrbitData>(start);

        // create the tween
        particle->GetTweens().Create()
            .BeginGroup()
                .Delay(_first_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(data, &ECS::OrbitData::SetRadius, start, middle, _first_duration)
            .EndGroup()
            .BeginGroup()
                .Delay(_second_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(data, &ECS::OrbitData::SetRadius, middle, end, _second_duration)
            .EndGroup();
        
        // rotate the object about the emitter's position at the given radius
        auto setter = [=] (float t) {
            float radius = data->GetRadius();

            trans->SetPositionWC({
                center.x + cos(t) * radius,
                center.y + sin(t) * radius,
                0
            });
        };

        // calculate the total amount of radians we're going to move
        TimeSpan total_timespan = _first_delay + _first_duration + _second_delay + _second_duration;
        float total_radians = _speed * total_timespan.Seconds();
        float start_radians = Math::Rand(0.0f, Math::PI_2);

        // set initial position
        trans->SetPositionWC({
            center.x + cos(start_radians) * data->GetRadius(),
            center.y + sin(start_radians) * data->GetRadius(),
            0
        });

        particle->GetTweens().Create()
            .Setter(setter, start_radians, start_radians + total_radians, total_timespan);
    }
}
