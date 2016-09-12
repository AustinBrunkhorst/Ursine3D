/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEmitterComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/


#pragma once

#include "Component.h"
#include "Renderable.h"
#include "GfxAPI.h"
#include "RenderableComponentBase.h"
#include "ParticleSystemComponent.h"

#include "SVec3.h"

#include "Randomizer.h"

namespace ursine
{
    namespace ecs
    {
        class ParticleEmitter
            : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorButton(
                resetSpawnCount,
                "Reset Spawn Count"
            );

            EditorButton(
                destroyAllParticles,
                "Reset All Particles"
            );

            EditorField(
                bool emitting,
                GetEmitting,
                SetEmitting
            );

            EditorField(
                float emitRate,
                GetEmitRate,
                SetEmitRate
            );

            EditorField(
                float emitRateVariance,
                GetEmitRateRange,
                SetEmitRateRange
            );

            EditorField(
                unsigned emitCount,
                GetEmitCount,
                SetEmitCount
            );

            EditorField(
                float lifetime,
                GetLifetime,
                SetLifetime
            );

            EditorField(
                float lifetimeVariance,
                GetLifetimeRange,
                SetLifetimeRange
            );

            EditorField(
                float size,
                GetSize,
                SetSize
            );

            EditorField(
                float sizeVariance,
                GetSizeRange,
                SetSizeRange
            );

            EditorField(
                float rotation,
                GetRotation,
                SetRotation
            );

            EditorField(
                float rotationVariance,
                GetRotationRange,
                SetRotationRange
            );

            EditorField(
                float roll,
                GetRoll,
                SetRoll
            );

            EditorField(
                float rollVariance,
                GetRollRange,
                SetRollRange
            );

            EditorField(
                SVec3 velocity,
                GetVelocity,
                SetVelocity
            );

            EditorField(
                SVec3 velocityVariance,
                GetVelocityRange,
                SetVelocityRange
            );

            EditorMeta(InputRange(0.0f, 1.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
            EditorField(
                float fill,
                GetFill,
                SetFill
            );

            EditorField(
                SVec3 emitterSize,
                GetEmitterSize,
                SetEmitterSize
            );

            EditorField(
                bool fastMovingEmitter,
                GetFastMovingEmitter,
                SetFastMovingEmitter
            );

            ParticleEmitter(void);
            ~ParticleEmitter(void);

            Meta(Disable)
            void OnInitialize(void) override;

            // generators for particle data
            float GenerateLifetime(void);
            float GenerateScale(void);
            float GenerateRotation(void);
            SVec3 GenerateVelocity(void);
            SVec3 GeneratePosition(void);

            // getter/setters
            bool GetEmitting(void) const;
            void SetEmitting(bool emitting);

            float GetEmitRate(void) const;
            void SetEmitRate(const float rate);

            float GetEmitRateRange(void) const;
            void SetEmitRateRange(const float range);

            unsigned GetEmitCount(void) const;
            void SetEmitCount(const unsigned count);

            float GetLifetime(void) const;
            void SetLifetime(const float lifetime);

            float GetLifetimeRange(void) const;
            void SetLifetimeRange(const float range);

            float GetSize(void) const;
            void SetSize(const float size);

            float GetSizeRange(void) const;
            void SetSizeRange(const float range);

            float GetRotation(void) const;
            void SetRotation(const float rotation);

            float GetRotationRange(void) const;
            void SetRotationRange(const float range);

            float GetRoll(void);
            void SetRoll(const float roll);

            float GetRollRange(void) const;
            void SetRollRange(const float range);

            const SVec3 &GetVelocity(void) const;
            void SetVelocity(const SVec3 &velocity);

            SVec3 GetVelocityRange(void) const;
            void SetVelocityRange(const SVec3 &range);

            const SVec3 &GetEmitterSize(void) const;
            void SetEmitterSize(const SVec3 &size);

            const float GetFill(void) const;
            void SetFill(const float fill);

            bool GetFastMovingEmitter(void) const;
            void SetFastMovingEmitter(bool isFastMoving);

            int spawnParticle(void);

            void ResetSpawnCount(void);
            

        private:
            // temporary updating
            void onParticleUpdate(EVENT_HANDLER(Entity));

            // parent component, this is kinda important
            Component::Handle<ParticleSystem> m_particleComponent;

            bool m_emitting;

            SVec3 m_lastPosition;

            // all of these need variance /////////////////////////////////////
            // emit speed
            float m_emitRate;
            Randomizer m_emitRateRange;

            // max emit count, 0 is unlimited
            unsigned m_emitCount;

            // lifetime
            float m_lifetime;
            Randomizer m_lifetimeRange;

            // size
            float m_size;
            Randomizer m_sizeRange;

            // rotation
            float m_rotation;
            Randomizer m_rotationRange;

            // roll
            float m_roll;
            Randomizer m_rollRange;

            // velocity
            SVec3 m_initialVelocity;
            Randomizer m_xVelRange;
            Randomizer m_yVelRange;
            Randomizer m_zVelRange;

            // emitter size (related to scale of transform?)
            SVec3 m_emitterSize;

            float m_currentTime;
            unsigned m_spawnCount;

            Randomizer m_fillGenerator;
            Randomizer m_radiusGenerator;

            Randomizer m_xGenerator;
            Randomizer m_yGenerator;
            Randomizer m_zGenerator;

            float m_fill;

            bool m_fastMovingEmitter;
        } Meta(
            Enable, 
            DisplayName( "ParticleEmitter" )
        ) EditorMeta( 
            RequiresComponents( typeof( ursine::ecs::ParticleSystem ) )    
        );
    }
}