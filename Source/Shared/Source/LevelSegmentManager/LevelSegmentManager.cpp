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

#include "LevelSegmentManager.h"

#include <KeyboardManager.h>

ENTITY_SYSTEM_DEFINITION( LevelSegmentManager );

using namespace ursine;
using namespace ecs;

LevelSegmentManager::LevelSegmentManager(World *world)
    : EntitySystem( world )
{
}

void LevelSegmentManager::SegmentTransition(LevelSegments segment)
{
    LevelSegmentChangeArgs args;

    args.segment = segment;

    Dispatch( LevelSegmentManagerEvents::SegmentChanged, &args );
}

void LevelSegmentManager::OnInitialize(void)
{
    m_world->Listener( this )
        .On( WORLD_UPDATE, &LevelSegmentManager::onUpdate );
}

void LevelSegmentManager::onUpdate(EVENT_HANDLER(World))
{
    auto keyboardManager = GetCoreSystem( KeyboardManager );

    if (keyboardManager->IsTriggeredDown( KEY_1 ))
        SegmentTransition( LevelSegments::Intro );
    else if (keyboardManager->IsTriggeredDown( KEY_2 ))
        SegmentTransition( LevelSegments::BattleBowl1 );
    else if (keyboardManager->IsTriggeredDown( KEY_3 ))
        SegmentTransition( LevelSegments::Tutorial1Start );
}
