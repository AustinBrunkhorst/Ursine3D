/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ChangeSegmentOnAnimatorFinishComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "EntityAnimatorComponent.h"
#include "LevelSegments.h"

class ChangeSegmentOnAnimatorFinish : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        LevelSegments destinationSegment,
        GetDestinationSegment,
        SetDestinationSegment
    );

    ChangeSegmentOnAnimatorFinish(void);
    ~ChangeSegmentOnAnimatorFinish(void);

    void OnSceneReady(ursine::Scene *scene) override;

    LevelSegments GetDestinationSegment(void) const;
    void SetDestinationSegment(LevelSegments destination);

private:

    LevelSegments m_destination;

    void onAnimationFinish(EVENT_HANDLER(EntityAnimator));

} Meta(Enable, RequiresComponents(typeof(EntityAnimator)));
