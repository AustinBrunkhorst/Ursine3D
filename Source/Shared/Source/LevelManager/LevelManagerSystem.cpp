/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include <KeyboardManager.h>

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

void LevelManager::OnInitialize(void)
{
    m_world->Listener( this )
        .On( WORLD_UPDATE, &LevelManager::onUpdate );
}

void LevelManager::onUpdate(EVENT_HANDLER(World))
{
    auto keyboardManager = GetCoreSystem( KeyboardManager );

    if (keyboardManager->IsTriggeredDown(KEY_1))
        SegmentTransition( LevelSegments::Intro );
    else if (keyboardManager->IsTriggeredDown(KEY_2))
        SegmentTransition(LevelSegments::BattleBowl1);
    else if (keyboardManager->IsTriggeredDown(KEY_3))
        SegmentTransition(LevelSegments::Tutorial1Start);
}
