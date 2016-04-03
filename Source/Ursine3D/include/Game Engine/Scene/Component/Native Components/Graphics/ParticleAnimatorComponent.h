/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleAnimatorComponent.h
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
#include "Color.h"
#include "ParticleSystemComponent.h"

namespace ursine
{
    namespace ecs
    {
        class ParticleAnimator
            : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField(
                float dampening,
                GetDampening,
                SetDampening
            );

            EditorField(
                float growth,
                GetGrowth,
                SetGrowth
            );

            EditorField(
                float torque,
                GetTorque,
                SetTorque
            );

            EditorField(
                SVec3 force,
                GetForce,
                SetForce
            );

            ParticleAnimator(void);
            ~ParticleAnimator(void);

            Meta(Disable)
            void OnInitialize(void) override;

            float GetDampening(void) const;
            void SetDampening(const float value);

            float GetGrowth(void) const;
            void SetGrowth(const float growth);

            float GetTorque(void) const;
            void SetTorque(float torque);

            const SVec3 &GetForce(void) const;
            void SetForce(const SVec3 &force);

        private:
            void onParticleUpdate(EVENT_HANDLER(Entity));

            // parent component, this is kinda important
            ParticleSystem *m_particleComponent;

            float m_dampeningConstant;
            float m_growth;
            float m_torque;

            SVec3 m_force;
            SVec3 m_randomForce;

        } Meta(
            Enable,
            DisplayName( "ParticleAnimator" )
        ) EditorMeta(
            RequiresComponents( typeof( ursine::ecs::ParticleSystem ) )    
        );
    }
}