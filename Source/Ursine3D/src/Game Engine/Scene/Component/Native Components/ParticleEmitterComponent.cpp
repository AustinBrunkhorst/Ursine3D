/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

#define PI 3.14159265359f

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(ParticleEmitter);

        ParticleEmitter::ParticleEmitter(void)
            : BaseComponent( )
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
            , m_emitterSize(0.0f, 0.0f, 0.0f)
            , m_currentTime(0.0f)
            , m_spawnCount(0)
            , m_fillGenerator(0.0f, 0.0f)
            , m_radiusGenerator(-1.0f, 1.0f)
            , m_fill(0.0f)
            , m_xGenerator(0, 0)
            , m_yGenerator(0, 0)
            , m_zGenerator(0, 0)
        {
        }

        ParticleEmitter::~ParticleEmitter(void)
        {
            GetOwner( )->Listener(this)
                .Off(ENTITY_PARTICLE_UPDATE, &ParticleEmitter::onParticleUpdate);
        }

        void ParticleEmitter::OnInitialize(void)
        {
            Component::OnInitialize( );

            m_particleComponent = GetOwner( )->GetComponent<ParticleSystem>( );

            GetOwner( )->Listener(this)
                .On(ENTITY_PARTICLE_UPDATE, &ParticleEmitter::onParticleUpdate);
        }

        // GENERATOR METHODS ////////////////////////////////////////

        float ParticleEmitter::GenerateLifetime(void)
        {
            return m_lifetime + m_lifetimeRange.GetValue( );
        }

        float ParticleEmitter::GenerateScale(void)
        {
            return m_size + m_sizeRange.GetValue( );
        }

        float ParticleEmitter::GenerateRotation(void)
        {
            return m_rotation + m_rotationRange.GetValue( );
        }

        SVec3 ParticleEmitter::GenerateVelocity(void)
        {
            SVec3 offset;

            // this should be faster than a ton of trig operations... I think
            do
            {
                offset.SetX(m_radiusGenerator.GetValue( ));
                offset.SetY(m_radiusGenerator.GetValue( ));
                offset.SetZ(m_radiusGenerator.GetValue( ));
            } while ( offset.LengthSquared( ) > 1 );

            offset *= SVec3(m_xVelRange.GetValue( ), m_yVelRange.GetValue( ), m_zVelRange.GetValue( ));

            return GetOwner( )->GetTransform( )->GetWorldRotation( ) * (m_initialVelocity + offset);
        }

        SVec3 ParticleEmitter::GeneratePosition(void)
        {
            // generate random sphere position in range -1, 1
            // scale by values

            SVec3 offset;

            // this should be faster than a ton of trig operations... I think
            offset.SetX(m_radiusGenerator.GetValue( ) * m_emitterSize.X());
            offset.SetY(m_radiusGenerator.GetValue( ) * m_emitterSize.Y());
            offset.SetZ(m_radiusGenerator.GetValue( ) * m_emitterSize.Z());

            // normalize
            if(offset.LengthSquared() > 0)
                offset.Normalize( );

            // scale by offset * size to size
            offset *= SVec3(
                m_xGenerator.GetValue( ), 
                m_yGenerator.GetValue( ), 
                m_zGenerator.GetValue( )
            );

            if(m_particleComponent->GetSystemSpace( ) == SystemSpace::LocalSpace)
                return offset;

            return GetOwner( )->GetTransform( )->GetWorldPosition( ) + offset;
        }

        void ParticleEmitter::ResetSpawnCount(void)
        {
            m_spawnCount = 0;
        }

        #if defined(URSINE_WITH_EDITOR)

        void ParticleEmitter::resetSpawnCount(void)
        {
            ResetSpawnCount( );
        }

        #endif

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
            return m_emitRateRange.GetMax( );
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
            return m_lifetimeRange.GetMax( );
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
            return m_sizeRange.GetMax( );
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
            return (m_rotationRange.GetMax( ) * 180.0f) / PI;
        }

        void ParticleEmitter::SetRotationRange(const float range)
        {
            m_rotationRange.SetMax((range * PI) / 180.0f);
            m_rotationRange.SetMin(-(range * PI) / 180.0f);
        }

        float ParticleEmitter::GetRoll(void)
        {
            return (m_roll * 180.0f) / PI;
        }

        void ParticleEmitter::SetRoll(const float roll)
        {
            m_roll = (roll * PI) / 180.0f;;
        }

        float ParticleEmitter::GetRollRange(void) const
        {
            return (m_rollRange.GetMax() * 180.0f) / PI;
        }

        void ParticleEmitter::SetRollRange(const float range)
        {
            m_rollRange.SetMax((range * PI) / 180.0f);
            m_rollRange.SetMin( -(range * PI) / 180.0f);
        }

        const SVec3 &ParticleEmitter::GetVelocity(void) const
        {
            return m_initialVelocity;
        }

        void ParticleEmitter::SetVelocity(const SVec3 & velocity)
        {
            m_initialVelocity = velocity;
        }

        SVec3 ParticleEmitter::GetVelocityRange(void) const
        {
            return SVec3(
                m_xVelRange.GetMax( ),
                m_yVelRange.GetMax( ),
                m_zVelRange.GetMax( )
                );
        }

        void ParticleEmitter::SetVelocityRange(const SVec3 & range)
        {
            m_xVelRange.SetMax(range.X( ));
            m_yVelRange.SetMax(range.Y( ));
            m_zVelRange.SetMax(range.Z( ));

            m_xVelRange.SetMin(-range.X( ));
            m_yVelRange.SetMin(-range.Y( ));
            m_zVelRange.SetMin(-range.Z( ));
        }

        const SVec3 &ParticleEmitter::GetEmitterSize(void) const
        {
            return m_emitterSize;
        }
        void ParticleEmitter::SetEmitterSize(const SVec3 &size)
        {
            m_emitterSize = size;

            m_xGenerator = Randomizer(m_fill * size.X( ), size.X( ));
            m_yGenerator = Randomizer(m_fill * size.Y( ), size.Y( ));
            m_zGenerator = Randomizer(m_fill * size.Z( ), size.Z( ));
        }

        const float ParticleEmitter::GetFill(void) const
        {
            return m_fill;
        }

        void ParticleEmitter::SetFill(const float fill)
        {
            m_fill = fill;

            m_xGenerator = Randomizer(m_fill * m_emitterSize.X( ), m_emitterSize.X( ));
            m_yGenerator = Randomizer(m_fill * m_emitterSize.Y( ), m_emitterSize.Y( ));
            m_zGenerator = Randomizer(m_fill * m_emitterSize.Z( ), m_emitterSize.Z( ));
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

                ++m_spawnCount;
            }

            return newParticle;
        }

        void ParticleEmitter::onParticleUpdate(EVENT_HANDLER(Entity))
        {
            float dt = Application::Instance->GetDeltaTime( );

            if ( (m_spawnCount < m_emitCount) || m_emitCount == 0 )
            {
                m_currentTime += dt;

                // calculate time needed to spawn 1 particle
                float spawnTime = 1.f / static_cast<float>(m_emitRate);

                while ( m_currentTime >= spawnTime && ((m_spawnCount < m_emitCount) || m_emitCount == 0) )
                {
                    m_currentTime -= spawnTime;
                    spawnParticle( );
                }
            }

            auto &gpuData = m_particleComponent->GetGPUParticleData( );
            auto &cpuData = m_particleComponent->GetCPUParticleData( );

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

                gpuData[x].rotation[0] -= cpuData[x].roll * dt;

                // update position with velocity
                gpuData[ x ].position[ 0 ] += cpuData[ x ].velocity.X( ) * dt;
                gpuData[ x ].position[ 1 ] += cpuData[ x ].velocity.Y( ) * dt;
                gpuData[ x ].position[ 2 ] += cpuData[ x ].velocity.Z( ) * dt;
            }                                                         
        }                                                             
    }
}