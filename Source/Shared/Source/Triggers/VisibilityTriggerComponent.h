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

enum class VisibilityTriggerType
{
    VIS_MODEL,
    VIS_EMITTER
} Meta(Enable);

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

    EditorField(
        VisibilityTriggerType triggerType,
        GetTriggerType,
        SetTriggerType
        );

    void OnInitialize(void) override;

    LevelSegments GetLevelSegment(void) const;
    void SetLevelSegment(LevelSegments levelSegment);

    float GetDelayInSec(void) const;
    void SetDelayInSec(float delay);

    VisibilityTriggerType GetTriggerType(void) const;
    void SetTriggerType(VisibilityTriggerType type);

private:
    LevelSegments m_segment;

    bool m_unsubscribed;
    float m_delay;
    VisibilityTriggerType m_type;

    void onSegmentChange(EVENT_HANDLER(LevelSegmentManager));

} Meta(Enable);