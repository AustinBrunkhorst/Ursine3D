/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimateOnSegmentComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "LevelSegments.h"
#include "EntityAnimatorComponent.h"

class LevelSegmentManager;

class AnimateOnSegment : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        LevelSegments changeOnThisSegment,
        GetLevelSegment,
        SetLevelSegment
    );

    EditorField(
        bool whenGreaterThan,
        GetWhenGreaterThan,
        SetWhenGreaterThan
    );

    AnimateOnSegment(void);
    ~AnimateOnSegment(void);

    void OnInitialize(void) override;

    LevelSegments GetLevelSegment(void) const;
    void SetLevelSegment(LevelSegments levelSegment);

    bool GetWhenGreaterThan(void) const;
    void SetWhenGreaterThan(bool flag);

private:

    LevelSegments m_segment;
    bool m_greaterThan;

    void onHierarchySerialize(EVENT_HANDLER(ursine::ecs::Entity));

    void onSegmentChange(EVENT_HANDLER(LevelSegmentManager));

    void connectToEvent(bool connect);

    void playAnimation(void);

} Meta(Enable);