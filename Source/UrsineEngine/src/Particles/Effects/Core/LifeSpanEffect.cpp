#include "UrsinePrecompiled.h"

#include "LifeSpanEffect.h"
#include "ParticleEmitterComponent.h"

namespace ursine
{    
    void LifeSpanEffect::InitializeParticle(ecs::Entity *particle) const
    {
        particle->GetTimers().Create(_life_span).Completed([=]
        {
            particle->Delete();
        });
    }
}