/* ---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationState.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** - Hyung Jun Park - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Animation.h"
#include "AnimationClipData.h"

namespace ursine
{
    namespace ecs
    {
        class Animator;
        class AnimatorSystem;
    }

    class AnimationState
    {
    public:
        EditorField(
            bool loopAnimation,
            IsLooping,
            SetLooping
        );

        EditorField(
            std::string stateName,
            GetStateName,
            SetStateName
        );

        // how can I add multiple animations in animation state?
        // how can I use combo box for this?
        // Array<Animation> => no, in this case, we should edit
        // Get animation as a resource not by name
        EditorResourceField(
            ursine::resources::AnimationClipData,
            clip,
            GetClip,
            SetClip
        );

        /** @brief constructor
        *
        *  @return Void.
        */
        AnimationState(void);

        bool IsLooping(void) const;
        void SetLooping(bool isLooping);

        const std::string &GetStateName(void) const;
        void SetStateName(const std::string& name);

        /** @brief gets the time position of the current animation state
        *
        *  @return time (decimal) that the animation is at.
        */
        float GetTimePosition(void) const;

        /** @brief sets the animation position
        *
        *  This will cause the animation to jump to a specific frame
        *
        *  @param position where we want to go
        *  @return Void.
        */
        void SetTimePosition(float position);

        /** @brief Modifies the animation state by a delta
        *
        *  used to advance the animation along with the game when
        *  it is running
        *
        *  @param dt change in animation position
        *  @return Void.
        */
        void IncrementTimePosition(float dt);

        /** @brief gets the currently running animation
        *
        *  @return pointer to the animation
        */
        const Animation *GetAnimation(void) const;

        const ursine::resources::ResourceReference &GetClip(void) const;
        void SetClip(const ursine::resources::ResourceReference &clip);

        const float &GetTransPosition(void) const;
        void SetTransPosition(float tPos);
        
        void PlayingAnimation(void);

    private:
        friend class ecs::Animator;
        friend class ecs::AnimatorSystem;

        std::string m_stateName;

        resources::ResourceReference m_clip;

        //current animation
        const Animation *m_animation;

        bool m_looping;

        bool m_loaded;

        //time position to play animation
        float m_timePos;

        //time to start transitioning
        float m_transPos;

    } Meta(Enable, EnableArrayType, DisplayName("AnimationState"));
}