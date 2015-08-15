/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EmitterComponent.h
** - The 2D Particle Emitter component
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef PARTICLE_EMITTER_COMPONENT_H
#define PARTICLE_EMITTER_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#include <unordered_map>
#include "ParticleEffect.h"

namespace Ursine
{
    namespace ECS
    {
        enum IntervalType
        {
            INTERVAL_COMPLETED,
            INTERVAL_TICKED
        };

        class ParticleEmitter : public Component
        {
            friend class ParticleEmitterSystem;
            friend class JsonSerializer;

            // Array of effects this emitter will apply to the particles
            SerializerAllow(ParticleEmitter, _effects);
            ParticleEffectVector _effects;
            ParticleEffectTypeMask _mask;

            // The name of the entity the emitter is spawning
            SerializerAllow(ParticleEmitter, _entity_name);
            std::string _entity_name;

            // Number of particles spawned for each interval
            SerializerAllow(ParticleEmitter, _rate);
            uint _rate;

            // Interval in which we're spawning [rate] number of particles
            SerializerAllow(ParticleEmitter, _interval);
            TimeSpan _interval;
            
            // Type of interval we're using.  Determines when the particles are spawned.
            SerializerAllow(ParticleEmitter, _type);
            IntervalType _type;

            // Initial color of the particle
            SerializerAllow(ParticleEmitter, _color);
            Color _color;

            // Flag for starting and stopping emission each frame
            SerializerAllow(ParticleEmitter, _emit);
            bool _emit;

            // Timer used for counting by the ParticleEmitterSystem
            TimeSpan _timer;

            // Timer marker letting us know the last time we spawned a particle
            TimeSpan _last_spawned;

            void copy(const ParticleEmitter &other);

            // internal implementation
            void addEffect(ParticleEffect *effect);

        public:
            ParticleEmitter(void);
            ParticleEmitter(const ParticleEmitter &other);
            ParticleEmitter(uint rate, TimeSpan interval, IntervalType type);
            ~ParticleEmitter(void);

            const ParticleEmitter &operator=(const ParticleEmitter &rhs);

            void OnInitialize(void) override;

            void SetColor(const Color &color);

            // Tell the emitter to start/stop emitting particles
            void SetEmit(bool flag);

            // Tell the emitter to emit a specific number of particles
            void EmitParticles(uint count);

            void SetRate(uint rate);
            uint GetRate(void) const;

            void SetInterval(TimeSpan interval);
            TimeSpan GetInterval(void) const;

            // Add an effect to the emitter
            template <class EffectType, typename... Args>
            EffectType *AddEffect(Args&&... args);

            // Get an effect from the emitter
            template<class EffectType>
            EffectType *GetEffect(void);

            // Remove an effect from the emitter
            template<class EffectType>
            void RemoveEffect(void);

            // See if an emitter has an effect
            template<class EffectType>
            bool HasEffect(void);
        };
    }

    extern template
    Json JsonSerializer::Serialize(ECS::IntervalType &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, ECS::IntervalType &out);

    extern template
    Json JsonSerializer::Serialize(ParticleEffectVector &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, ParticleEffectVector &out);
}

#include "ParticleEmitterComponent.hpp"

////////////////////////////////////////////////////////////////////////////////
#endif // !PARTICLE_EMITTER_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS

#ifndef PARTICLE_EMITTER_REGISTERED
#define PARTICLE_EMITTER_REGISTERED

namespace Ursine
{
    namespace ECS
    {
        RegisterComponent(ParticleEmitter);
        RegisterComponentData(ParticleEmitter, _effects, "effects");
        RegisterComponentData(ParticleEmitter, _entity_name, "entity_name");
        RegisterComponentData(ParticleEmitter, _rate, "rate");
        RegisterComponentData(ParticleEmitter, _interval, "interval");
        RegisterComponentData(ParticleEmitter, _type, "type");
        RegisterComponentData(ParticleEmitter, _color, "color");
        RegisterComponentData(ParticleEmitter, _emit, "emit");
    }
}

#endif

#endif
