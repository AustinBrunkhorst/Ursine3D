/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RandomSlerpComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <Squat.h>

class RandomSlerp
    : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    RandomSlerp(void);

    void Update(const float dt);


private:
    float m_slerpTime;
    float m_slerpLength;

    float m_minSlowTime;
    float m_maxSlowTime;

    float m_minFastTime;
    float m_maxFastTime;

    bool m_goingFast;

    ursine::SQuat m_startRotation;
    ursine::SQuat m_destinationRotation;
}Meta(Enable, DisplayName("RandomSlerp"));