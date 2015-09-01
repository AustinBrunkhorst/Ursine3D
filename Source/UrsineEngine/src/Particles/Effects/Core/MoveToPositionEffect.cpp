#include "UrsinePrecompiled.h"
#include "MoveToPositionEffect.h"
#include "ParticleEmitterComponent.h"

namespace ursine
{
    void MoveToPositionEffect::InitializeParticle(ecs::Entity *particle) const
    {
        auto particle_trans = particle->GetTransform();
        auto emitter_trans = GetEmitter()->GetOwner()->GetTransform();
        Vec3 start_pos, middle_pos, end_pos;

        start_pos = emitter_trans->ToWorld(SVec3(_start.GetValue(), _start.GetValue(), 1));
        middle_pos = emitter_trans->ToWorld(SVec3(_middle.GetValue(), _middle.GetValue(), 1));
        end_pos = emitter_trans->ToWorld(SVec3(_end.GetValue(), _end.GetValue(), 1));

        // set the starting position
        particle_trans->SetPositionWC(start_pos);

        // create the tween
        particle->GetTweens().Create()
            .BeginGroup()
                .Delay(_first_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(particle_trans, &ecs::Transform::SetPositionWC, start_pos, middle_pos, _first_duration)
            .EndGroup()
            .BeginGroup()
                .Delay(_second_delay)
            .EndGroup()
            .BeginGroup()
                .Setter(particle_trans, &ecs::Transform::SetPositionWC, middle_pos, end_pos, _second_duration)
            .EndGroup();
    }
}
