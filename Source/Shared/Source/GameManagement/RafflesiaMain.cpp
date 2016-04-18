#include "Precompiled.h"

#include "RafflesiaMain.h"

#include "UIScreensConfigComponent.h"
#include "LevelSegmentManagerComponent.h"

#include <GameContext.h>
#include <Scene.h>

using namespace ursine;

RafflesiaMain::RafflesiaMain(void)
{

}

RafflesiaMain::~RafflesiaMain(void)
{
    GetContext( )->Listener( this )
        .Off( GC_WINDOW_FOCUS_CHANGED, &RafflesiaMain::onWindowFocusChanged );

    GetContext( )->GetScene( )->GetScreenManager( ).Listener( this )
        .Off( ui_event::global::MainMenuEntered, &RafflesiaMain::onMainMenuEntered )
        .Off( ui_event::global::GameStart, &RafflesiaMain::onGameplayStarted )
        .Off( ui_event::global::GamePause, &RafflesiaMain::onGameplayPaused )
        .Off( ui_event::global::GameResume, &RafflesiaMain::onGameplayResumed )
        .Off( ui_event::global::GameExit, &RafflesiaMain::onGameplayExited )
        .Off( ui_event::global::GameRestart, &RafflesiaMain::onGameplayRestarted );
}

void RafflesiaMain::SpawnEndingCredits(void)
{
    Application::PostMainThread( [=] {
        auto *ui = GetConfigComponent<UIScreensConfig>( );

        setWorld( ui->GetEndingCreditsWorld( ) );

        ui_event::SpawnEndingCredits creditsEvent;

        ui->TriggerPlayerHUDEvent( creditsEvent );
    } );
}

void RafflesiaMain::OnInitialize(GameContext *context, const Json &configObj)
{
    GameManager::OnInitialize( context, configObj );

    GetContext( )->Listener( this )
        .On( GC_WINDOW_FOCUS_CHANGED, &RafflesiaMain::onWindowFocusChanged );

    context->GetScene( )->GetScreenManager( ).Listener( this )
        .On( ui_event::global::MainMenuEntered, &RafflesiaMain::onMainMenuEntered )
        .On( ui_event::global::GameStart, &RafflesiaMain::onGameplayStarted )
        .On( ui_event::global::GamePause, &RafflesiaMain::onGameplayPaused )
        .On( ui_event::global::GameResume, &RafflesiaMain::onGameplayResumed )
        .On( ui_event::global::GameExit, &RafflesiaMain::onGameplayExited )
        .On( ui_event::global::GameRestart, &RafflesiaMain::onGameplayRestarted );

    auto *ui = GetConfigComponent<UIScreensConfig>( );

    if (ui->GetDebugMode( ))
    {
        auto segment = ui->GetDebugSegment( );

        auto *world = setWorld( ui->GetDebugWorld( ), false );

        if (segment != LevelSegments_enum::Empty)
        {
            if (!ui->HasPlayerHUD( ))
                ui->AddPlayerHUD( );

            auto segmentManagers = world->GetEntitiesFromFilter( ecs::Filter( ).All<LevelSegmentManager>( ) );

            if (!segmentManagers.empty( ))
                segmentManagers[ 0 ]->GetComponent<LevelSegmentManager>( )->SetCurrentSegment( segment );
        }

        m_context->GetScene( )->LoadConfiguredSystems( );
    }
    else
    {
        ui->AddSplash( );
    }
}

void RafflesiaMain::onWindowFocusChanged(EVENT_HANDLER(ursine::GameContext))
{
    EVENT_ATTRS(ursine::GameContext, GameContextWindowFocusArgs);

    auto *scene = m_context->GetScene( );

    scene->GetScreenManager( ).MessageGlobalRemote( ui_event::global::WindowFocusChanged, args->focused );

    if (args->focused)
    {
        auto *ui = GetConfigComponent<UIScreensConfig>( );

        if (!ui)
            return;

        auto *hud = ui->GetPlayerHUD( );

        auto *screenData = ui->GetPauseRef( ).Load<resources::UIScreenData>( 
            scene->GetResourceManager( )
        );

        if (!screenData)
            return;

        // add pause screen if it doesn't exist
        if (hud && !scene->GetScreenManager( ).GetScreen( screenData->GetPath( ) ))
        {
            UIScreenConfig config;

            config.inputBlocking = true;

            ui->AddPause( config, Json::object {
                { "sender", static_cast<int>( hud->GetID( ) ) }
            } );
        }
    }
}

void RafflesiaMain::onMainMenuEntered(EVENT_HANDLER(UIScreenManager))
{
    EVENT_ATTRS(UIScreenManager, UIScreenMessageArgs);

    auto *ui = GetConfigComponent<UIScreensConfig>( );

    auto *scene = m_context->GetScene( );

    scene->SetPaused( false );

    setWorld( ui->GetMainMenuWorld( ) );
}

void RafflesiaMain::onGameplayStarted(EVENT_HANDLER(UIScreenManager))
{
    EVENT_ATTRS(UIScreenManager, UIScreenMessageArgs);

    auto *ui = GetConfigComponent<UIScreensConfig>( );

    auto *scene = m_context->GetScene( );

    scene->SetPaused( false );

    ui->AddPlayerHUD( );

    setWorld( ui->GetStartingGameplayWorld( ) );
}

void RafflesiaMain::onGameplayPaused(EVENT_HANDLER(UIScreenManager))
{
    EVENT_ATTRS(UIScreenManager, UIScreenMessageArgs);

    m_context->GetScene( )->SetPaused( true );
}

void RafflesiaMain::onGameplayResumed(EVENT_HANDLER(UIScreenManager))
{
    EVENT_ATTRS(UIScreenManager, UIScreenMessageArgs);

    m_context->GetScene( )->SetPaused( false );
}

void RafflesiaMain::onGameplayExited(EVENT_HANDLER(UIScreenManager))
{
    EVENT_ATTRS(UIScreenManager, UIScreenMessageArgs);
}

void RafflesiaMain::onGameplayRestarted(EVENT_HANDLER(UIScreenManager))
{
    EVENT_ATTRS(UIScreenManager, UIScreenMessageArgs);

    auto *scene = m_context->GetScene( );
    auto *ui = GetConfigComponent<UIScreensConfig>( );
    auto world = scene->GetActiveWorld( );

    auto currentManagers = world->GetEntitiesFromFilter( ecs::Filter( ).All<LevelSegmentManager>( ) );

    UAssert( !currentManagers.empty( ), 
        "No level segment managers." 
    );

    auto currentSegment = currentManagers[ 0 ]->GetComponent<LevelSegmentManager>( )->GetCurrentSegment( );

    auto *newWorld = setWorld( ui->GetStartingGameplayWorld( ), false );

    auto managers = newWorld->GetEntitiesFromFilter( ecs::Filter( ).All<LevelSegmentManager>( ) );

    UAssert( !managers.empty( ),
        "No new level segment managers." 
    );

    managers[ 0 ]->GetComponent<LevelSegmentManager>( )->SetCurrentSegment( currentSegment );

    m_context->GetScene( )->LoadConfiguredSystems( );
}