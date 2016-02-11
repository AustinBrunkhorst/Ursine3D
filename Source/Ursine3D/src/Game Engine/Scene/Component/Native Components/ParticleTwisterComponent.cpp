/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleTwisterComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "ParticleTwisterComponent.h"

#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(ParticleTwister);

        ParticleTwister::ParticleTwister(void)
            : BaseComponent()
            , m_particleComponent(nullptr)
            , m_axis(0.0f, 1.0f, 0.0f)
            , m_minDistance(0.0f)
            , m_maxDistance(10.0f)
            , m_strength(10.0f)
        {
        }

        ParticleTwister::~ParticleTwister(void)
        {
            GetOwner()->Listener(this)
                .Off(ENTITY_PARTICLE_UPDATE, &ParticleTwister::onParticleUpdate);
        }

        void ParticleTwister::OnInitialize(void)
        {
            Component::OnInitialize();

            GetOwner()->Listener(this)
                .On(ENTITY_PARTICLE_UPDATE, &ParticleTwister::onParticleUpdate);
        }

        const SVec3 & ParticleTwister::GetAxis(void) const
        {
            return m_axis;
        }

        void ParticleTwister::SetAxis(const SVec3 & axis)
        {
            m_axis = axis;
        }

        float ParticleTwister::GetMinDistance(void) const
        {
            return m_minDistance;
        }

        void ParticleTwister::SetMinDistance(const float distance)
        {
            m_minDistance = distance;
        }

        float ParticleTwister::GetMaxDistance(void) const
        {
            return m_maxDistance;
        }

        void ParticleTwister::SetMaxDistance(const float distance)
        {
            m_maxDistance = distance;
        }

        float ParticleTwister::GetStrength(void) const
        {
            return m_strength;
        }

        void ParticleTwister::SetStrength(const float strength)
        {
            m_strength = strength;
        }

        void ParticleTwister::onParticleUpdate(EVENT_HANDLER(Entity))
        {
            m_particleComponent = GetOwner()->GetComponent<ParticleSystem>();
            float dt = Application::Instance->GetDeltaTime();

            // get all the particles
            auto &gpuData = m_particleComponent->GetGPUParticleData();
            auto &cpuData = m_particleComponent->GetCPUParticleData();

            auto center = GetOwner()->GetTransform()->GetWorldPosition();

            int particleCount = m_particleComponent->GetActiveParticleCount();

            float axisDotAxis = m_axis.Dot(m_axis);
            float sqrdMax = m_maxDistance * m_maxDistance;
            float sqrdMin = m_minDistance * m_minDistance;

            for ( int x = 0; x < particleCount; ++x )
            {
                SVec3 particlePos = SVec3(
                    gpuData[ x ].position[ 0 ], 
                    gpuData[ x ].position[ 1 ], 
                    gpuData[ x ].position[ 2 ]
                );

                // get vector from ps position to us
                auto psToParticle = particlePos - center;

                // get point projected on axis
                auto axisPoint = center + (psToParticle.Dot(m_axis) / axisDotAxis) * m_axis;

                // get vector
                auto projToParticle = particlePos - axisPoint;

                // get distance from that
                float distanceFromAxis = projToParticle.LengthSquared();

                // if distance is okay
                SVec3 velocity;
                if ( distanceFromAxis < sqrdMax )
                {
                    // generate tangent
                    SVec3 tangentVec = m_axis;
                    tangentVec.Cross(projToParticle);

                    tangentVec.Normalize();

                    if ( distanceFromAxis <= sqrdMin )
                    {
                        velocity = tangentVec * dt * m_strength;
                    }
                    else
                    {
                        float scalar = 1.f - ((distanceFromAxis - sqrdMin) / (sqrdMax - sqrdMin));
                        velocity = tangentVec * dt * m_strength * scalar;
                    }
                }
                else
                {
                    continue;
                }

                // apply to particle
                cpuData[ x ].velocity += velocity;
            }
        }
    }
}