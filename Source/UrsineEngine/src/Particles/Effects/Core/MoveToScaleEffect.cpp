#include "UrsinePrecompiled.h"
#include "MoveToScaleEffect.h"
#include "ParticleEmitterComponent.h"

namespace Ursine
{
    void MoveToScaleEffect::InitializeParticle(ECS::Entity *particle) const
    {
        auto particle_trans = particle->GetTransform();
        Vec3 start_scale, middle_scale, end_scale;
        auto emitter_scale = GetEmitter()->GetOwner()->GetTransform()->ScaleWC();

        if (_uniform_scaling)
        {
            float start_value = _start.GetValue();
            float middle_value = _middle.GetValue();
            float end_value = _end.GetValue();
            
            start_scale = emitter_scale * Vec3(start_value, start_value, 0);
            middle_scale = emitter_scale * Vec3(middle_value, middle_value, 0);
            end_scale = emitter_scale * Vec3(end_value, end_value, 0);
        }
        else
        {
            start_scale = emitter_scale * Vec3(_start.GetValue(), _start.GetValue(), 0);
            middle_scale = emitter_scale * Vec3(_middle.GetValue(), _middle.GetValue(), 0);
            end_scale = emitter_scale * Vec3(_end.GetValue(), _end.GetValue(), 0);
        }

        // set the starting scale
        particle_trans->SetScaleWC(start_scale);

        // create the tween
        particle->GetTweens().Create()
            .BeginGroup()
                .Delay(_first_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(particle_trans, &ECS::Transform::SetScaleWC, start_scale, middle_scale, _first_duration)
            .EndGroup()
            .BeginGroup()
                .Delay(_second_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(particle_trans, &ECS::Transform::SetScaleWC, middle_scale, end_scale, _second_duration)
            .EndGroup();
    }
}
