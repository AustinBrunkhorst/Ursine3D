/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

enum class LevelManagerEvents
{
    SegmentChanged
};

class LevelManager 
    : public ursine::ecs::EntitySystem
    , public ursine::EventDispatcher<LevelManagerEvents>
{
    ENTITY_SYSTEM;

public:

    LevelManager(ursine::ecs::World *world);

    void SegmentTransition(LevelSegments segment);

} Meta(Enable, AutoAddEntitySystem);
