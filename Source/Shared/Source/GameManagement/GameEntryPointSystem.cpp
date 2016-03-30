/* ---------------------------------------------------------------------------
** Team Bear King
** (c) 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GameEntryPointSystem.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "GameEntryPointSystem.h"

#include "UIScreensConfigComponent.h"

#include "UIScreen.h"

ENTITY_SYSTEM_DEFINITION( GameEntryPointSystem );

GameEntryPointSystem::GameEntryPointSystem(ursine::ecs::World *world)
    : EntitySystem( world )
{
    
}

void GameEntryPointSystem::OnInitialize(void)
{
    
}

void GameEntryPointSystem::OnSceneReady(ursine::Scene *scene)
{
    auto *uiScreens = m_world->GetSettings( )->GetComponent<UIScreensConfig>( );

    if (!uiScreens)
        return;

    uiScreens->AddSplash( );
}

void GameEntryPointSystem::OnRemove(void)
{
    
}
