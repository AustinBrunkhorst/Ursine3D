/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEmitterSystem.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef PARTICLE_EMITTER_SYSTEM_H
#define PARTICLE_EMITTER_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#include "FilterSystem.h"

namespace ursine
{
    namespace ecs
    {
        class ParticleEmitterSystem : public FilterSystem
        {
            friend class ParticleEmitter;

            TimeSpan _dt;

            void spawnParticles(ParticleEmitter *emitter, uint num);

        public:
            ParticleEmitterSystem(World *world);
            ~ParticleEmitterSystem(void);

            void Begin(void) override;
            void Process(Entity *entity) override;
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !PARTICLE_EMITTER_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_SYSTEMS
#ifndef PARTICLE_EMITTER_SYSTEM_REGISTERED
#define PARTICLE_EMITTER_SYSTEM_REGISTERED

namespace ursine
{
    namespace ecs
    {
        RegisterEntitySystem(ParticleEmitterSystem);
    }
}

#endif
#endif