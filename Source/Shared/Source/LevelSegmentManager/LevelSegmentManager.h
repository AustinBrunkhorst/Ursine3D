/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LevelManagerSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>
#include <EventDispatcher.h>

#include "LevelSegments.h"

class LevelSegmentChangeArgs : public ursine::EventArgs
{
public:
    LevelSegments segment;
};

enum class LevelSegmentManagerEvents
{
    SegmentChanged
};

class LevelSegmentManager 
    : public ursine::ecs::EntitySystem
    , public ursine::EventDispatcher<LevelSegmentManagerEvents>
{
    ENTITY_SYSTEM;

public:

    LevelSegmentManager(ursine::ecs::World *world);

    void SegmentTransition(LevelSegments segment);

private:

    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(World));

} Meta(Enable, AutoAddEntitySystem);
