/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleTwisterComponent.h
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
        class ParticleTwister
            : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField(
                SVec3 axis,
                GetAxis,
                SetAxis
            );

            EditorField(
                float minDistance,
                GetMinDistance,
                SetMinDistance
            );

            EditorField(
                float maxDistance,
                GetMaxDistance,
                SetMaxDistance
            );

            EditorField(
                float strength,
                GetStrength,
                SetStrength
            );

            ParticleTwister(void);
            ~ParticleTwister(void);

            Meta(Disable)
            void OnInitialize(void) override;

            const SVec3 &GetAxis(void) const;
            void SetAxis(const SVec3 &axis);

            float GetMinDistance(void) const;
            void SetMinDistance(const float distance);

            float GetMaxDistance(void) const;
            void SetMaxDistance(const float distance);

            float GetStrength(void) const;
            void SetStrength(const float strength);

        private:
            void onParticleUpdate(EVENT_HANDLER(Entity));

            // parent component, this is kinda important
            ParticleSystem *m_particleComponent;

            SVec3 m_axis;

            float m_minDistance;
            float m_maxDistance;

            float m_strength;

        }Meta(
            Enable,
            DisplayName( "ParticleTwister" )
        ) EditorMeta( 
            RequiresComponents( typeof( ursine::ecs::ParticleSystem ) )    
        );
    }
}