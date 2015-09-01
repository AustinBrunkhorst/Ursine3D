#include "UrsinePrecompiled.h"
#include "ScaleSetterEffect.h"
#include "ParticleEmitterComponent.h"

namespace ursine
{
    void ScaleSetterEffect::InitializeParticle(ecs::Entity *particle) const
    {
        auto particle_trans = particle->GetTransform();
        auto emitter_trans = GetEmitter()->GetOwner()->GetTransform();
        auto emitter_scale = emitter_trans->ScaleWC();

        particle_trans->SetScaleWC({
            emitter_scale * SVec3(_randomizer.GetValue(), _randomizer.GetValue(), 0)
        });
    }
}
