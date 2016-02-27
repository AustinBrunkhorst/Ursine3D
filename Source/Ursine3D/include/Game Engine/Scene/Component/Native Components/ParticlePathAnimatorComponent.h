/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticlePathAnimatorComponent.h
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
        class ParticlePathAnimator
            : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField(
                bool loopPath,
                GetIsLooping,
                SetIsLooping
            );

            EditorField(
                bool renderCurve,
                GetDebugRender,
                SetDebugRender
            );

            EditorField(
                unsigned stepCount,
                GetStepCount, 
                SetStepCount
            );
             
            EditorField(
                float strength,
                GetStrength,
                SetStrength
            );

            ParticlePathAnimator(void);
            ~ParticlePathAnimator(void);
             
            Meta(Disable)
            void OnInitialize(void) override;
             
            bool GetIsLooping(void) const;
            void SetIsLooping(bool isLooping);

            bool GetDebugRender(void) const;
            void SetDebugRender(bool useDebug);

            unsigned GetStepCount(void) const;
            void SetStepCount(unsigned stepCount);

            float GetStrength(void) const;
            void SetStrength(float strength);

        private:
            void onParticleUpdate(EVENT_HANDLER(Entity));

            bool checkForPointUpdate(void);

            void updatePointArray(void);
            void updateVectorArray(void);

            SVec3 getPoint(unsigned index, float t);


            // parent component, this is kinda important
            ParticleSystem *m_particleComponent;

            std::vector<SVec3> m_pointArray;
            std::vector<SVec3> m_vectorArray;

            bool m_loopPath;
            bool m_renderCurve;
            unsigned m_stepCount;
            float m_strength;

        } Meta(
            Enable,
            DisplayName("ParticlePathAnimator"),
            RequiresComponents(typeof(ursine::ecs::ParticleSystem))
        );
    }
}