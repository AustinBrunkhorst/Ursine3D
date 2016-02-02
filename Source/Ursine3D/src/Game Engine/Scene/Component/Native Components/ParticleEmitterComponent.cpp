/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEmitterComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ParticleEmitterComponent.h"

#include "EntityEvent.h"

#include "GfxAPI.h"

#define PI 3.14f

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(ParticleEmitter);

        ParticleEmitter::ParticleEmitter(void)
            : BaseComponent()
            , m_emitRate(100)
            , m_emitRateRange(0.0f, 0.0f)
            , m_emitCount(0)
            , m_lifetime(1.0f)
            , m_lifetimeRange(0.0f, 0.0f)
            , m_size(1.0f)
            , m_sizeRange(0.0f, 0.0f)
            , m_rotation(0.0f)
            , m_rotationRange(-PI, PI)
            , m_initialVelocity(0.0f, 0.0f, 0.0f)
            , m_xVelRange(-1.0f, 1.0f)
            , m_yVelRange(-1.0f, 1.0f)
            , m_zVelRange(-1.0f, 1.0f)
            , m_radius(0.0f)
            , m_radiusRange(0.0f, 0.0f)
            , m_currentTime(0.0f)
        {
            
        }

        ParticleEmitter::~ParticleEmitter(void)
        {
            GetOwner()->Listener(this)
                .Off(ENTITY_PARTICLE_UPDATE, &ParticleEmitter::onParticleUpdate);
        }

        void ParticleEmitter::OnInitialize(void)
        {
            Component::OnInitialize();

            m_particleComponent = GetOwner()->GetComponent<ParticleSystem>();

            GetOwner()->Listener(this)
                .On(ENTITY_PARTICLE_UPDATE, &ParticleEmitter::onParticleUpdate);
        }

        // GENERATOR METHODS ////////////////////////////////////////

        float ParticleEmitter::GenerateLifetime(void) const
        {
            return m_lifetime + m_lifetimeRange.GetValue();
        }

        float ParticleEmitter::GenerateScale(void) const
        {
            return m_size + m_sizeRange.GetValue();
        }

        float ParticleEmitter::GenerateRotation(void) const
        {
            return m_rotation + m_rotationRange.GetValue();
        }

        SVec3 ParticleEmitter::GenerateVelocity(void) const
        {
            return m_initialVelocity + SVec3(
                m_xVelRange.GetValue(),
                m_yVelRange.GetValue(),
                m_zVelRange.GetValue()
                );
        }

        SVec3 ParticleEmitter::GeneratePosition(void) const
        {
            return GetOwner()->GetTransform()->GetWorldPosition();
        }

        // GETTER / SETTERS /////////////////////////////////////////
        float ParticleEmitter::GetEmitRate(void) const
        {
            return m_emitRate;
        }
        void ParticleEmitter::SetEmitRate(const float rate)
        {
            m_emitRate = rate;
        }

        float ParticleEmitter::GetEmitRateRange(void) const
        {
            return m_emitRateRange.GetMax();
        }
        void ParticleEmitter::SetEmitRateRange(const float range)
        {
            m_emitRateRange.SetMax(range);
            m_emitRateRange.SetMin(-range);
        }

        unsigned ParticleEmitter::GetEmitCount(void) const
        {
            return m_emitCount;
        }
        void ParticleEmitter::SetEmitCount(const unsigned count)
        {
            m_emitCount = count;
        }

        float ParticleEmitter::GetLifetime(void) const
        {
            return m_lifetime;
        }
        void ParticleEmitter::SetLifetime(const float lifetime)
        {
            m_lifetime = lifetime;
        }

        float ParticleEmitter::GetLifetimeRange(void) const
        {
            return m_lifetimeRange.GetMax();
        }
        void ParticleEmitter::SetLifetimeRange(const float range)
        {
            m_lifetimeRange.SetMax(range);
            m_lifetimeRange.SetMin(-range);
        }

        float ParticleEmitter::GetSize(void) const
        {
            return m_size;
        }
        void ParticleEmitter::SetSize(const float size)
        {
            m_size = size;
        }

        float ParticleEmitter::GetSizeRange(void) const
        {
            return m_sizeRange.GetMax();
        }
        void ParticleEmitter::SetSizeRange(const float range)
        {
            m_sizeRange.SetMax(range);
            m_sizeRange.SetMin(-range);
        }

        float ParticleEmitter::GetRotation(void)
        {
            return (m_rotation * 180.0f) / PI;
        }
        void ParticleEmitter::SetRotation(const float rotation)
        {
            m_rotation = (rotation * PI) / 180.0f;
        }

        float ParticleEmitter::GetRotationRange(void) const
        {
            return m_rotationRange.GetMax();
        }
        void ParticleEmitter::SetRotationRange(const float range)
        {
            m_rotationRange.SetMax(range);
            m_rotationRange.SetMin(-range);
        }

        const SVec3 & ParticleEmitter::GetVelocity(void) const
        {
            return m_initialVelocity;
        }
        void ParticleEmitter::SetVelocity(const SVec3 & velocity)
        {
            m_initialVelocity = velocity;
        }

        const SVec3 ParticleEmitter::GetVelocityRange(void) const
        {
            return SVec3(
                m_xVelRange.GetMax(),
                m_yVelRange.GetMax(),
                m_zVelRange.GetMax()
                );
        }
        void ParticleEmitter::SetVelocityRange(const SVec3 & range)
        {
            m_xVelRange.SetMax(range.X());
            m_yVelRange.SetMax(range.Y());
            m_zVelRange.SetMax(range.Z());

            m_xVelRange.SetMin(-range.X());
            m_yVelRange.SetMin(-range.Y());
            m_zVelRange.SetMin(-range.Z());
        }
        void ParticleEmitter::onParticleUpdate(EVENT_HANDLER(Entity))
        {
            float dt = Application::Instance->GetDeltaTime();

            m_currentTime += dt;

            // calculate time needed to spawn 1 particle
            float spawnTime = 1.f / static_cast<float>(m_emitRate);

            while( m_currentTime >= spawnTime )
            {
                m_currentTime -= spawnTime;
                spawnParticle();
            }

            auto &gpuData = m_particleComponent->GetGPUParticleData();
            auto &cpuData = m_particleComponent->GetCPUParticleData();

            // update ALL particles
            int activeParticles = m_particleComponent->GetActiveParticleCount( );
            for ( int x = 0; x < activeParticles; ++x )
            {
                // remove time from life
                cpuData[ x ].lifeTime -= dt;

                // destroy if dead
                if ( cpuData[ x ].lifeTime <= 0 )
                {
                    m_particleComponent->DestroyParticle(x);
                    --activeParticles;
                    continue;
                }

                // update position with velocity
                gpuData[ x ].position[ 0 ] += cpuData[ x ].velocity.X() * dt;
                gpuData[ x ].position[ 1 ] += cpuData[ x ].velocity.Y() * dt;
                gpuData[ x ].position[ 2 ] += cpuData[ x ].velocity.Z() * dt;

                /*GetCoreSystem(graphics::GfxAPI)->DrawingMgr.SetColor(gpuData[ x ].color[ 0 ], gpuData[ x ].color[ 1 ], gpuData[ x ].color[ 2 ], gpuData[ x ].color[ 3 ]);
                GetCoreSystem(graphics::GfxAPI)->DrawingMgr.SetSize(gpuData[ x ].scaleX);
                GetCoreSystem(graphics::GfxAPI)->DrawingMgr.DrawPoint(gpuData[ x ].position[ 0 ], gpuData[ x ].position[ 1 ], gpuData[ x ].position[ 2 ]);*/
            }                                                         
        }                                                             
        int ParticleEmitter::spawnParticle(void)
        {
            // create a particles
            int newParticle = m_particleComponent->SpawnParticle( );

            // check to see it was spawned properly
            if ( newParticle != -1 )
            {
                // grab its info
                auto &gpuData = m_particleComponent->GetGPUParticle(newParticle);
                auto &cpuData = m_particleComponent->GetCPUParticle(newParticle);

                auto &position = GeneratePosition( );
                auto &color = m_particleComponent->GetColor( );

                // set position
                gpuData.position[ 0 ] = position.X( );
                gpuData.position[ 1 ] = position.Y( );
                gpuData.position[ 2 ] = position.Z( );

                // set scale and rotation
                gpuData.scaleX = gpuData.scaleY = GenerateScale( );
                gpuData.rotation[ 0 ] = GenerateRotation( );

                // set color
                gpuData.color[ 0 ] = color.r;
                gpuData.color[ 1 ] = color.g;
                gpuData.color[ 2 ] = color.b;
                gpuData.color[ 3 ] = color.a;

                // set CPU data
                cpuData.velocity = GenerateVelocity( );
                cpuData.lifeTime = cpuData.totalLifetime = GenerateLifetime( );
                cpuData.acceleration = SVec3(0.f, 0.f, 0.f);
            }

            return newParticle;
        }
    }
}