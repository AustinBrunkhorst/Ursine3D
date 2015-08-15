#include "UrsinePrecompiled.h"
#include "ParticleEmitterSystem.h"
#include "ParticleEmitterComponent.h"
#include "RenderableComponent.h"

namespace Ursine
{
    namespace ECS
    {
        ParticleEmitterSystem::ParticleEmitterSystem(World *world)
            : FilterSystem(world, Filter().All<ParticleEmitter>())
            , _dt(0)
        {

        }

        ParticleEmitterSystem::~ParticleEmitterSystem(void)
        {

        }

        void ParticleEmitterSystem::Begin(void)
        {
            _dt = TimeSpan::FromSeconds(gApplication->GetDelta());
        }

        void ParticleEmitterSystem::Process(Entity *entity)
        {
            auto emitter = entity->GetComponent<ParticleEmitter>();

            if (emitter->_emit)
            {
                if (emitter->_type == INTERVAL_COMPLETED)
                {
                    // decrement the timer
                    emitter->_timer -= _dt;

                    // if timer is up
                    if (emitter->_timer <= TimeSpan(0.0f))
                    {
                        // spawn all of the particles
                        spawnParticles(emitter, emitter->_rate);

                        // reset the timer
                        emitter->_timer = emitter->_interval;
                    }
                }
                else
                {
                    // calculate the time marker for spawning a particle
                    TimeSpan marker = emitter->_interval.Milliseconds() / static_cast<float>(emitter->_rate);

                    // decrement the timer
                    emitter->_timer -= _dt;

                    // calculate the difference
                    TimeSpan diff = emitter->_last_spawned - emitter->_timer;

                    // if the difference is larger than our spawn marker
                    if (diff >= marker)
                    {
                        // calcualte the number to spawn
                        uint num = diff.Milliseconds() / marker.Milliseconds();

                        spawnParticles(emitter, num);

                        // reset timer if need be
                        if (emitter->_timer <= TimeSpan(0.0f))
                            emitter->_timer = emitter->_interval;

                        // update the last spawned timer
                        emitter->_last_spawned = emitter->_timer;
                    }
                }
            }
        }

        void ParticleEmitterSystem::spawnParticles(ParticleEmitter *emitter, uint num)
        {
#ifndef _DEBUG
            auto emitter_entity = emitter->GetOwner();
            auto &tint = emitter->_color;

            if (!emitter_entity->IsDeleting())
                for (uint i = 0; i < num; ++i)
                {
                    auto particle = _world->CreateEntity(emitter->_entity_name);

                    particle->GetComponent<Renderable>()->SetTint(tint);

                    for (auto &effect : emitter->_effects)
                    {
                        effect->InitializeParticle(particle);
                    }
                }
#endif
        }
    }
}
