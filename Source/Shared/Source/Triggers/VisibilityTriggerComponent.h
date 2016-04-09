/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** VisibilityTriggerComponent.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "LevelSegments.h"

class LevelSegmentManager;

class VisibilityTrigger : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    VisibilityTrigger(void);
    ~VisibilityTrigger(void);

    EditorField(
        LevelSegments changeOnThisSegment,
        GetLevelSegment,
        SetLevelSegment
    );

    EditorField(
        float delayInSeconds,
        GetDelayInSec,
        SetDelayInSec
        );

    void OnInitialize(void) override;

    LevelSegments GetLevelSegment(void) const;
    void SetLevelSegment(LevelSegments levelSegment);

    float GetDelayInSec(void) const;
    void SetDelayInSec(float delay);

private:
    LevelSegments m_segment;

    bool m_unsubscribed;
    float m_delay;

    void onSegmentChange(EVENT_HANDLER(LevelSegmentManager));

} Meta(Enable);