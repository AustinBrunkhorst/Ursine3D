/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SegmentChangeTriggerComponent.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "LevelSegments.h"

class SegmentChangeTrigger : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    SegmentChangeTrigger(void);
    ~SegmentChangeTrigger(void);

    void OnInitialize(void) override;

    EditorField(
        LevelSegments changeOnThisSegment,
        GetCurrentLevelSegment,
        SetCurrentLevelSegment
    );

    EditorField(
        LevelSegments changeNewSegment,
        GetNewLevelSegment,
        SetNewLevelSegment
    );

    LevelSegments GetCurrentLevelSegment(void) const;
    void SetCurrentLevelSegment(LevelSegments levelSegment);

    LevelSegments GetNewLevelSegment(void) const;
    void SetNewLevelSegment(LevelSegments levelSegment);

    // Is trigger active
    bool triggerPlayer1;
    bool triggerPlayer2;

    void onCollision(EVENT_HANDLER(ursine::ecs::Entity));

private:
    LevelSegments m_currentSegment;
    LevelSegments m_newSegment;
    bool m_collisionPlayer1;
    bool m_collisionPlayer2;

    bool m_unsubscribed;


} Meta(Enable, DisplayName("SegmentChangeTrigger"));
