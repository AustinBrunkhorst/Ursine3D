/* ---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationEvent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimationEvent.h"

namespace ursine
{
    AnimationEvent::AnimationEvent(void)
        : m_ratio( 0.0f )
        , m_sent( false ) { }

    const std::string &AnimationEvent::GetStateName(void) const
    {
        return m_stateName;
    }

    void AnimationEvent::SetStateName(const std::string &state)
    {
        m_stateName = state;
    }

    float AnimationEvent::GetRatio(void) const
    {
        return m_ratio;
    }

    void AnimationEvent::SetRatio(float ratio)
    {
        m_ratio = ratio;
    }

    const std::string &AnimationEvent::GetEventMessage(void) const
    {
        return m_message;
    }

    void AnimationEvent::SetEventMessage(const std::string &message)
    {
        m_message = message;
    }
}
