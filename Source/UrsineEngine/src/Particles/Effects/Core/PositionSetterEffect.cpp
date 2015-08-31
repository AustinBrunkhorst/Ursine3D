#include "UrsinePrecompiled.h"
#include "PositionSetterEffect.h"
#include "ParticleEmitterComponent.h"

namespace Ursine
{
    void PositionSetterEffect::InitializeParticle(ECS::Entity *particle) const
    {
        auto particle_trans = particle->GetTransform();
        auto emitter_trans = GetEmitter()->GetOwner()->GetTransform();
        auto emitter_pos = emitter_trans->PositionWC();

        particle_trans->SetPositionWC({
            emitter_trans->ToWorld(SVec3(_randomizer.GetValue(), _randomizer.GetValue(), 1))
        });
    }
}
