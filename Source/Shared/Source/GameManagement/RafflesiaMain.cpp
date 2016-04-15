#include "Precompiled.h"

#include "RafflesiaMain.h"

#include "UIScreensConfigComponent.h"

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

    ui->AddSplash( );
}

void RafflesiaMain::onWindowFocusChanged(EVENT_HANDLER(ursine::GameContext))
{
    EVENT_ATTRS(ursine::GameContext, GameContextWindowFocusArgs);

    if (args->focused)
    {
        auto *ui = GetConfigComponent<UIScreensConfig>( );
        auto *hud = ui->GetPlayerHUD( );

        if (hud)
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
}