#include "UrsinePrecompiled.h"
#include "ScaleSetterEffect.h"
#include "ParticleEmitterComponent.h"

namespace Ursine
{
    void ScaleSetterEffect::InitializeParticle(ECS::Entity *particle) const
    {
        auto particle_trans = particle->GetTransform();
        auto emitter_trans = GetEmitter()->GetOwner()->GetTransform();
        auto emitter_scale = emitter_trans->ScaleWC();

        particle_trans->SetScaleWC({
            emitter_scale * Vector3(_randomizer.GetValue(), _randomizer.GetValue(), 0)
        });
    }
}
