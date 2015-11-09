#include "UrsinePrecompiled.h"
#include "AnimationState.h"

namespace ursine
{
    AnimationState::AnimationState()
        : m_timePos(0.0f)
        , m_animation(nullptr)
    {}

    float AnimationState::GetTimePosition() const
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

    const Animation* AnimationState::GetAnimation() const
    {
        return m_animation;
    }

    void AnimationState::SetAnimation(const Animation* animation)
    {
        m_animation = animation;
    }
}