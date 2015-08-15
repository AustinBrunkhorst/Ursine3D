#include "UrsinePrecompiled.h"

#include "LifeSpanEffect.h"
#include "ParticleEmitterComponent.h"

namespace Ursine
{    
    void LifeSpanEffect::InitializeParticle(ECS::Entity *particle) const
    {
        particle->GetTimers().Create(_life_span).Completed([=]
        {
            particle->Delete();
        });
    }
}