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
                ResetSpawnCount,
                "Reset Spawn Count"
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
                Vec3 velocity,
                GetVelocity,
                SetVelocity
            );

            EditorField(
                Vec3 velocityVariance,
                GetVelocityRange,
                SetVelocityRange
            );

            EditorField(
                float emitterSize,
                GetEmitterSize,
                SetEmitterSize
            );

            EditorField(
                float fill,
                GetFill,
                SetFill
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

            float GetRotation(void);
            void SetRotation(const float rotation);

            float GetRotationRange(void) const;
            void SetRotationRange(const float range);

            const SVec3 &GetVelocity(void) const;
            void SetVelocity(const SVec3 &velocity);

            const SVec3 GetVelocityRange(void) const;
            void SetVelocityRange(const SVec3 &range);

            const float GetEmitterSize(void) const;
            void SetEmitterSize(const float size);

            const float GetEmitterSizeRange(void) const;
            void SetEmitterSizeRange(const float range);

            const float GetFill(void) const;
            void SetFill(const float fill);

        private:
            // temporary updating
            void onParticleUpdate(EVENT_HANDLER(Entity));

            int spawnParticle(void);

            // parent component, this is kinda important
            ParticleSystem *m_particleComponent;

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

            // velocity
            SVec3 m_initialVelocity;
            Randomizer m_xVelRange;
            Randomizer m_yVelRange;
            Randomizer m_zVelRange;

            // emitter size (related to scale of transform?)
            float m_emitterSize;
            Randomizer m_emitterSizeRange;

            float m_currentTime;
            unsigned m_spawnCount;

            Randomizer m_angleGenerator;
            Randomizer m_radiusGenerator;

            float m_fill;

        } Meta(
            Enable, 
            DisplayName( "ParticleEmitter" ),
            RequiresComponents( typeof( ursine::ecs::ParticleSystem ) )
        );
    }
}