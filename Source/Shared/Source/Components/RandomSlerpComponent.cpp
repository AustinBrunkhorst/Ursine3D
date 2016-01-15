/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RandomSlerpComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "RandomSlerpComponent.h"
#include "Randomizer.h"

using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION(RandomSlerp);

RandomSlerp::RandomSlerp() 
    : BaseComponent()
    , m_slerpTime(0)
    , m_slerpLength(-1)
    , m_minSlowTime(1)
    , m_maxSlowTime(2)
    , m_minFastTime(0.3f)
    , m_maxFastTime(0.8f) 
    , m_goingFast (false)
    , m_startRotation(SQuat())
    , m_destinationRotation(SQuat()) {}

void RandomSlerp::Update(const float dt)
{
    if(m_slerpTime >= m_slerpLength)
    {
        Randomizer myRandomizer;
        if(m_goingFast)
        {
            // go slow
            myRandomizer.SetMin(m_minSlowTime);
            myRandomizer.SetMax(m_maxSlowTime);
        }
        else
        {
            // go fast!
            myRandomizer.SetMin(m_minFastTime);
            myRandomizer.SetMax(m_maxFastTime);
        }

        // get length of time to rotate
        m_slerpLength = myRandomizer.GetValue( );

        m_slerpTime = 0;

        // rotate our current rotation by 2 random quats
        myRandomizer.SetMin(-180);
        myRandomizer.SetMax(180);

        m_startRotation = GetOwner()->GetTransform()->GetWorldRotation();

        m_destinationRotation = SQuat(
            myRandomizer.GetValue(), 
            myRandomizer.GetValue(), 
            myRandomizer.GetValue()
        );

        m_destinationRotation *= SQuat(
            myRandomizer.GetValue(),
            myRandomizer.GetValue(),
            myRandomizer.GetValue()
        );

        // change
        m_goingFast = !m_goingFast;
    }

    // slerp
    auto currentRot = m_startRotation.Slerp(
        m_destinationRotation,
        m_slerpTime / m_slerpLength
    );

    // update time
    m_slerpTime += dt;

    // set rotation
    GetOwner()->GetTransform()->SetWorldRotation(currentRot);
}
