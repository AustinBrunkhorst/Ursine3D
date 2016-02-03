/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleAttractorComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "ParticleAttractorComponent.h"

#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(ParticleAttractor);

        ParticleAttractor::ParticleAttractor(void)
            : BaseComponent()
            , m_particleComponent(nullptr)
            , m_minDistance(0.0f)
            , m_maxDistance(10.0f)
            , m_strength(10.0f)
        {
        }

        ParticleAttractor::~ParticleAttractor(void)
        {
        }

        void ParticleAttractor::OnInitialize(void)
        {
            Component::OnInitialize();

            m_particleComponent = GetOwner()->GetComponent<ParticleSystem>();

            GetOwner()->Listener(this)
                .On(ENTITY_PARTICLE_UPDATE, &ParticleAttractor::onParticleUpdate);
        }

        float ParticleAttractor::GetMinDistance(void) const
        {
            return m_minDistance;
        }

        void ParticleAttractor::SetMinDistance(const float distance)
        {
            m_minDistance = distance;
        }

        float ParticleAttractor::GetMaxDistance(void) const
        {
            return m_maxDistance;
        }

        void ParticleAttractor::SetMaxDistance(const float distance)
        {
            m_maxDistance = distance;
        }

        float ParticleAttractor::GetStrength(void)
        {
            return m_strength;
        }

        void ParticleAttractor::SetStrength(const float strength)
        {
            m_strength = strength;
        }

        void ParticleAttractor::onParticleUpdate(EVENT_HANDLER(Entity))
        {
            float dt = Application::Instance->GetDeltaTime();

            // get all the particles
            auto &gpuData = m_particleComponent->GetGPUParticleData();
            auto &cpuData = m_particleComponent->GetCPUParticleData();

            int particleCount = m_particleComponent->GetActiveParticleCount();

            float sqrdMax = m_maxDistance * m_maxDistance;
            float sqrdMin = m_minDistance * m_minDistance;

            auto center = GetOwner()->GetTransform()->GetWorldPosition();

            for ( int x = 0; x < particleCount; ++x )
            {
                // for each particle, determine distance from ourselves
                auto vec = (center - SVec3(gpuData[ x ].position[ 0 ],
                    gpuData[ x ].position[ 1 ],
                    gpuData[ x ].position[ 2 ]
                    ));
                float distance = vec.LengthSquared();

                // calculate how much strength to apply, within min is full, else interp from min to max
                SVec3 velocity;
                if ( distance <= sqrdMin )
                {
                    velocity = vec * dt * m_strength;
                }
                else
                {
                    float scalar = 1.f - ((distance - sqrdMin) / (sqrdMax - sqrdMin));
                    velocity = vec * dt * m_strength * scalar;
                }

                // apply to particle
                cpuData[ x ].velocity += velocity;
            }
        }
    }
}