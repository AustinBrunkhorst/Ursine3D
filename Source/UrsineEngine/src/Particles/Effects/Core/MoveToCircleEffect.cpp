#include "UrsinePrecompiled.h"
#include "MoveToCircleEffect.h"
#include "ParticleEmitterComponent.h"

namespace Ursine
{
    void MoveToCircleEffect::InitializeParticle(ECS::Entity *particle) const
    {
        auto particle_trans = particle->GetTransform();
        auto emitter_trans = GetEmitter()->GetOwner()->GetTransform();

        Vector3 start_pos, middle_pos, end_pos;

        float start_angle = _start_angle.GetValue();
        float start_radius = _start_radius.GetValue();

        start_pos = emitter_trans->ToWorld(Vector3(Vector2::AngleVec(start_angle) * start_radius, 1));

        float middle_angle = _middle_angle.GetValue();
        float middle_radius = _middle_radius.GetValue();

        middle_pos = emitter_trans->ToWorld(Vector3(Vector2::AngleVec(middle_angle) * middle_radius, 1));

        float end_angle = _end_angle.GetValue();
        float end_radius = _end_radius.GetValue();

        end_pos = emitter_trans->ToWorld(Vector3(Vector2::AngleVec(end_angle) * end_radius, 1));

        // set the starting position
        particle_trans->SetPositionWC(start_pos);

        // create the tween
        particle->GetTweens().Create()
            .BeginGroup()
                .Delay(_first_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(particle_trans, &ECS::Transform::SetPositionWC, start_pos, middle_pos, _first_duration)
            .EndGroup()
            .BeginGroup()
                .Delay(_second_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(particle_trans, &ECS::Transform::SetPositionWC, middle_pos, end_pos, _second_duration)
            .EndGroup();
    }

    void MoveToCircleEffect::SetMiddleRadius(Randomizer randomizer)
    {
        _middle_radius = randomizer;
    }

    void MoveToCircleEffect::SetMiddleAngle(Randomizer randomizer)
    {
        _middle_angle = randomizer;
    }
}