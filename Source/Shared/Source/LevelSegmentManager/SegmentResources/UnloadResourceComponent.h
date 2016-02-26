/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UnloadResourceComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "LevelSegments.h"

class LevelSegmentManager;

class UnloadResource : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    
    EditorField(
        LevelSegments unloadSegment,
        GetUnloadSegment,
        SetUnloadSegment
    );

    UnloadResource(void);

    UnloadResource(LevelSegmentManager *manager, LevelSegments unloadSegment);

    LevelSegments GetUnloadSegment(void) const;
    void SetUnloadSegment(LevelSegments segment);

private:

    LevelSegments m_unloadSegment;

    void onSegmentChange(EVENT_HANDLER(LevelSegmentManager));
} Meta(Enable, HiddenInSelector);
