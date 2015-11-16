/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Renderable.h"
#include "GfxAPI.h"
#include "AnimationBuilder.h"

namespace ursine
{
    namespace ecs
    {
        class AnimationClip
        {
        public:
            enum MaskBlendMode
            {
                MASKBLEND_OVERRIDE = 0, // only override for now, can't blend multiple masks

                MASKBLEND_COUNT
            };

        public:
            const std::string &GetName(void) const;
            void SetName(const std::string &name);
            
            float GetStartTime(void) const;
            void SetStartTime(const float startTime);

            float GetEndTime(void) const;
            void SetEndTime(const float endTime);

            bool IsLooping(void);
            bool SetLooping(const bool looping);

            MaskBlendMode GetMaskBlendingMode(void) const;
            void SetMaskBlendingMode(const MaskBlendMode mode);

            ursine::AnimationState &GetAnimationState(void);
            void SetAnimationState(const ursine::AnimationState &state);

        private:
            // name of this clip
            std::string m_name; 

            // start time in the animation
            float m_start;

            // end time in the animation
            float m_end;

            // should we loop playing?
            bool m_looping;

            // how should we blend when applying multiple masks?
            MaskBlendMode m_maskBlendMode;

            // the animation state for this clip
            ursine::AnimationState m_animationState;
        };

        /////////////////////////////////////////////////////////////

        class Animator : public Component
        {
            NATIVE_COMPONENT;

        public:
            Animator( void );
            ~Animator( void );

            Meta( Disable )
                void OnInitialize( void ) override;

        private:

        } Meta( Enable, DisplayName( "Animator" ) );
    }
}