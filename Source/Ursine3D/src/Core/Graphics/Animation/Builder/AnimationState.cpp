/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationState.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** - Hyung Jun Park - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "AnimationState.h"
#include "AnimationBuilder.h"
#include "Notification.h"

#include "AnimationClipData.h"
#include "Component.h"

namespace ursine
{
    AnimationState::AnimationState()
        : m_name("")
        , m_looping(true)
        , m_timePos(0.0f)
        , m_transPos(0.0f)
        , m_animname("")
        , m_animation(nullptr)
    {}

    void AnimationState::OnSceneReady(void)
    {
        Animation* targetAnimation = AnimationBuilder::GetAnimationByName(m_animname);
        if (!targetAnimation)
        {
#if defined(URSINE_WITH_EDITOR)
            NotificationConfig error;

            error.type = NOTIFY_ERROR;
            error.header = "Animation doesn't exist";
            error.message = "To add animation into the state, animation should exist in the Animation List";

            EditorPostNotification(error);
#endif
            return;
        }

        m_animation = targetAnimation;
    }

    bool AnimationState::IsLooping(void) const
    {
        return m_looping;
    }

    void AnimationState::SetLooping(const bool isLooping)
    {
        m_looping = isLooping;
    }

    const std::string &AnimationState::GetName(void) const
    {
        return m_name;
    }

    void AnimationState::SetName(const std::string& name)
    {
        m_name = name;
    }
    
    float AnimationState::GetTimePosition(void) const
    {
        return m_timePos;
    }

    void AnimationState::SetTimePosition(const float position)
    {
        m_timePos = position;
    }

    void AnimationState::IncrementTimePosition(const float dt)
    {
        m_timePos += dt;
    }

    // change this to push_back or something that can handle multiple animations insdie
    const Animation* AnimationState::GetAnimation(void) const
    {
        return m_animation;
    }

    void AnimationState::SetAnimation(Animation* animation)
    {
        m_animation = animation;
    }

    const std::string &AnimationState::GetAnimationName(void) const
    {
        return m_animname;
    }
    
    void AnimationState::SetAnimationName(const std::string& name)
    {
        m_animname = name;
    }

    // make this can handle multiple names of animation name
    const Animation* AnimationState::GetAnimationByName(void) const
    {
        return m_animation;
    }

    void AnimationState::SetAnimationByName(const std::string& name)
    {
        Animation* targetAnimation = AnimationBuilder::GetAnimationByName(name);
        if (!targetAnimation)
        {
            m_animation = nullptr;
            return;
        }
        m_animation = targetAnimation;
    }

    const float &AnimationState::GetTransPosition(void) const
    {
        return m_transPos;
    }

    void AnimationState::SetTransPosition(const float& tPos)
    {
        m_transPos = tPos;
    }
    
    void AnimationState::PlayingAnimation(void)
    {
        if (!m_animation)
            return;

        unsigned keyframeCount1 = m_animation->GetRigKeyFrameCount();
        auto &curr_firstFrame = m_animation->GetKeyframe(0, 0);
        auto &curr_lastFrame = m_animation->GetKeyframe(keyframeCount1 - 1, 0);

        if (GetTimePosition() > curr_lastFrame.length)
        {
            if (IsLooping())
                SetTimePosition(curr_firstFrame.length);
            else
                SetTimePosition(curr_lastFrame.length);
        }
    }
}
