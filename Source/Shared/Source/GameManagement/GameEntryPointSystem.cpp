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

#include "UIEvents.h"

#include <Scene.h>
#include <UIScreenManager.h>

#if defined(URSINE_WITH_EDITOR)

#include <Notification.h>

#endif

using namespace ursine;

ENTITY_SYSTEM_DEFINITION( GameEntryPointSystem );

GameEntryPointSystem::GameEntryPointSystem(ecs::World *world)
    : EntitySystem( world )
{

}

void GameEntryPointSystem::OnInitialize(void)
{

}

void GameEntryPointSystem::OnSceneReady(Scene *scene)
{
    scene->GetScreenManager( ).Listener( this )
        .On( ui_event::global::GameplayStarted, &GameEntryPointSystem::onGameplayStarted );

    auto *ui = m_world->GetSettings( )->GetComponent<UIScreensConfig>( );

#if defined(URSINE_WITH_EDITOR)

    if (!ui)
    {
        NotificationConfig warning;

        warning.type = NOTIFY_WARNING;
        warning.header = "Warning";
        warning.message = "World settings missing <strong class=\"highlight\">UIScreensConfig</strong> component.";
            
        EditorPostNotification( warning );

        return;
    }

#else

    UAssert( ui != nullptr,
        "World settings missing UIScreensConfig component."    
    );

#endif

    auto *splash = ui->AddSplash( );

#if defined(URSINE_WITH_EDITOR)

    if (!splash)
    {
        NotificationConfig warning;

        warning.type = NOTIFY_WARNING;
        warning.header = "Warning";
        warning.message = "UI Screen <strong class=\"highlight\">Splash</strong> invalid or not configured.";
            
        EditorPostNotification( warning );
    }

#else

    UAssert( splash != nullptr,
        "UIScreen 'Splash' invalid or not configured."    
    );

#endif
}

void GameEntryPointSystem::OnRemove(void)
{
    m_world->GetOwner( )->GetScreenManager( ).Listener( this )
        .Off( ui_event::global::GameplayStarted, &GameEntryPointSystem::onGameplayStarted );
}

void GameEntryPointSystem::onGameplayStarted(EVENT_HANDLER(UIScreenManager))
{
    EVENT_ATTRS(UIScreenManager, UIScreenMessageArgs);

    auto *ui = m_world->GetSettings( )->GetComponent<UIScreensConfig>( );

#if defined(URSINE_WITH_EDITOR)

    if (!ui)
    {
        NotificationConfig warning;

        warning.type = NOTIFY_WARNING;
        warning.header = "Warning";
        warning.message = "World settings missing <strong class=\"highlight\">UIScreensConfig</strong> component.";
            
        EditorPostNotification( warning );

        return;
    }

#else

    UAssert( ui != nullptr,
        "World settings missing UIScreensConfig component."    
    );

#endif

    m_world->ImportWorld( 
        m_world->GetOwner( )->GetResourceManager( ), 
        ui->GetStartingGameplayWorld( ) 
    );
}
