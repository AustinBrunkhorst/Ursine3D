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
#include "Animation.h"
#include "Notification.h"

#include "AnimationClipData.h"
#include "Component.h"

namespace ursine
{
    using namespace resources;

    AnimationState::AnimationState(void)
        : m_looping( true )
        , m_timePos( 0.0f )
        , m_transPos( 0.0f )
        , m_animation( nullptr )
        , m_loaded( false ) { }

    bool AnimationState::IsLooping(void) const
    {
        return m_looping;
    }

    void AnimationState::SetLooping(bool isLooping)
    {
        m_looping = isLooping;
    }

    const std::string &AnimationState::GetStateName(void) const
    {
        return m_stateName;
    }

    void AnimationState::SetStateName(const std::string& name)
    {
        m_stateName = name;
    }
    
    float AnimationState::GetTimePosition(void) const
    {
        return m_timePos;
    }

    void AnimationState::SetTimePosition(float position)
    {
        m_timePos = position;
    }

    void AnimationState::IncrementTimePosition(float dt)
    {
        m_timePos += dt;
    }

    // change this to push_back or something that can handle multiple animations insdie
    const Animation* AnimationState::GetAnimation(void) const
    {
        if (m_animation)
            return m_animation;
        else
            return nullptr;
    }

    ///// FOR INSTATE ANIMATION
    //const std::vector<Animation*> AnimationState::GetAnimationVector(void) const
    //{
    //    return m_animationVec;
    //}

    const ResourceReference &AnimationState::GetClip(void) const
    {
        return m_clip;
    }

    void AnimationState::SetClip(const ResourceReference &clip)
    {
        m_clip = clip;

        m_loaded = false;
    }

    const float &AnimationState::GetTransPosition(void) const
    {
        return m_transPos;
    }

    void AnimationState::SetTransPosition(float tPos)
    {
        m_transPos = tPos;
    }

    float AnimationState::GetRatio(void) const
    {
        if (!m_animation)
            return false;

        unsigned keyframeCount1 = m_animation->GetRigKeyFrameCount( );
        auto &lastFrame = m_animation->GetKeyframe( keyframeCount1 - 1, 0 );

        return m_timePos / lastFrame.length;
    }

    bool AnimationState::PlayingAnimation(void)
    {
        if (!m_animation)
            return false;

        unsigned keyframeCount1 = m_animation->GetRigKeyFrameCount( );
        auto &curr_firstFrame = m_animation->GetKeyframe( 0, 0 );
        auto &curr_lastFrame = m_animation->GetKeyframe( keyframeCount1 - 1, 0 );

        if (m_timePos > curr_lastFrame.length)
        {
            if (m_looping)
                SetTimePosition( curr_firstFrame.length );
            else
                SetTimePosition( curr_lastFrame.length );

            return true;
        }

        return false;
    }
}
