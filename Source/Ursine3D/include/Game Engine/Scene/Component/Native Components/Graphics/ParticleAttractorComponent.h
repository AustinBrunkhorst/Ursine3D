/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleAttractorComponent.h
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
        class ParticleAttractor
            : public Component
        {
            NATIVE_COMPONENT;

        public:
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


            ParticleAttractor(void);
            ~ParticleAttractor(void);

            Meta(Disable)
            void OnInitialize(void) override;

            float GetMinDistance(void) const;
            void SetMinDistance(const float distance);

            float GetMaxDistance(void) const;
            void SetMaxDistance(const float distance);

            float GetStrength(void);
            void SetStrength(const float strength);

        private:
            void onParticleUpdate(EVENT_HANDLER(Entity));

            // parent component, this is kinda important
            ParticleSystem *m_particleComponent;

            float m_minDistance;
            float m_maxDistance;

            float m_strength;

        } Meta(
            Enable,
            DisplayName("ParticleAttractor"),
            RequiresComponents(typeof(ursine::ecs::ParticleSystem))
        );
    }
}