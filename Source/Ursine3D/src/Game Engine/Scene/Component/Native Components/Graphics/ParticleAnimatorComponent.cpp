/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleAnimatorComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "ParticleAnimatorComponent.h"
#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(ParticleAnimator);

        ParticleAnimator::ParticleAnimator(void)
            : BaseComponent()
            , m_particleComponent(nullptr)
            , m_dampeningConstant(0.0f)
            , m_growth(0.0f)
            , m_force(0.0f, 0.0f, 0.0f)
            , m_randomForce(0.0f, 0.0f, 0.0f)
        {
        }

        ParticleAnimator::~ParticleAnimator(void)
        {
            GetOwner()->Listener(this)
                .Off(ENTITY_PARTICLE_UPDATE, &ParticleAnimator::onParticleUpdate);
        }

        void ParticleAnimator::OnInitialize(void)
        {
            Component::OnInitialize();

            m_particleComponent = GetOwner()->GetComponent<ParticleSystem>();

            GetOwner()->Listener(this)
                .On(ENTITY_PARTICLE_UPDATE, &ParticleAnimator::onParticleUpdate, 1);
        }

        float ParticleAnimator::GetDampening(void) const
        {
            return m_dampeningConstant;
        }

        void ParticleAnimator::SetDampening(const float value)
        {
            m_dampeningConstant = value;
        }

        float ParticleAnimator::GetGrowth(void) const
        {
            return m_growth;
        }

        void ParticleAnimator::SetGrowth(const float growth)
        {
            m_growth = growth;
        }

        float ParticleAnimator::GetTorque() const
        {
            return m_torque * 180.0f / math::PI;
        }

        void ParticleAnimator::SetTorque(float torque)
        {
            m_torque = torque * math::PI / 180.0f;
        }

        const SVec3 & ParticleAnimator::GetForce(void) const
        {
            return m_force;
        }

        void ParticleAnimator::SetForce(const SVec3 & force)
        {
            m_force = force;
        }

        void ParticleAnimator::onParticleUpdate(EVENT_HANDLER(Entity))
        {
            m_particleComponent = GetOwner()->GetComponent<ParticleSystem>();
            float dt = Application::Instance->GetDeltaTime();

            // get all the particles
            auto &gpuData = m_particleComponent->GetGPUParticleData();
            auto &cpuData = m_particleComponent->GetCPUParticleData();

            int particleCount = m_particleComponent->GetActiveParticleCount();

            for ( int x = 0; x < particleCount; ++x )
            {
                // apply force to velocity
                cpuData[ x ].velocity += m_force * dt;

                // apply growth
                gpuData[ x ].scaleX += dt * m_growth;

                // apply damping
                cpuData[ x ].velocity *= (1.f - m_dampeningConstant * dt);

                cpuData[ x ].roll += m_torque * dt;
            }
        }
    }
}