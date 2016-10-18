/*---------------------------------------------------------------------------
**Team Bear King
**?2015 DigiPen Institute of Technology, All Rights Reserved.
**
* *AnimatorComponent.h
**
**Author:
**- Jordan Ellis - j.ellis@digipen.edu
*
**Contributors:
**- <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include "AnimationData.h"

namespace ursine
{
    namespace ecs
    {
        class Animator : public Component
        {
            NATIVE_COMPONENT;

            friend class AnimatorSystem;

        public:
            EditorResourceField(
                ursine::resources::AnimationData,
                animation,
                GetAnimation,
                SetAnimation
            );

            EditorField(
                bool playing,
                GetPlaying,
                SetPlaying
            );

            EditorField(
                AnimationPlaymode playmode,
                GetPlaymode,
                SetPlaymode
            );

            Animator(void);

            const ursine::resources::ResourceReference &GetAnimation(void) const;
            void SetAnimation(const ursine::resources::ResourceReference &animation);

            bool GetPlaying(void) const;
            void SetPlaying(bool playing);

            ursine::AnimationPlaymode GetPlaymode(void) const;
            void SetPlaymode(ursine::AnimationPlaymode playmode);

        private:
            resources::ResourceReference m_animation;

            bool m_playing;

            // Current time marker
            float m_time;

            // Direction we're moving (-1 -> 1)
            float m_direction;

            AnimationPlaymode m_playmode;
        } Meta(
            Enable,
            DisplayName("Animator")
        ) EditorMeta(
                RequiresComponents(typeof(ursine::ecs::Rig))
        );
    }
}
