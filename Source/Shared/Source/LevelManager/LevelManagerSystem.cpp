/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LevelManagerSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LevelManagerSystem.h"

ENTITY_SYSTEM_DEFINITION( LevelManager );

using namespace ursine;
using namespace ecs;

LevelManager::LevelManager(World *world)
    : EntitySystem( world )
{
}

void LevelManager::SegmentTransition(LevelSegments segment)
{
    LevelSegmentChangeArgs args;

    args.segment = segment;

    Dispatch( LevelManagerEvents::SegmentChanged, &args );
}