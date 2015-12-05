#include "Precompiled.h"

#include "MultiplayerPlayScreen.h"

#include <WorldSerializer.h>
#include <AudioManager.h>
#include <Timer.h>

using namespace ursine;

namespace
{
    const auto kWorldFile = "Assets/Worlds/SubmissionSemester1-Cpt.uworld";
}

MultiplayerPlayScreen::MultiplayerPlayScreen(ScreenManager *manager)
    : GameplayScreen( manager )
{
    URSINE_TODO( "this is to fix the transition flicker" );
    Timer::Create( 250 ).Completed( [&] {
        world = ecs::WorldSerializer( ).Deserialize( kWorldFile );

        world->SetOwner( this );

        URSINE_TODO( "This guy should be saved in an editor specific file eventually" );
        //world->GetEntityFromName( "Editor Camera" )->Delete( );
    } );

    const std::string init = "INIT.bnk";
    const std::string bgm = "BGM.bnk";
    const std::string car = "Car.bnk";
    const std::string RPM = "RPM";

    const std::string play_CarEngine = "Play_RecordableMusic";

    AkBankID initID = AK_INVALID_BANK_ID;
    AkBankID carID = AK_INVALID_BANK_ID;
    AkBankID bgmID = AK_INVALID_BANK_ID;

    const AkGameObjectID GAME_OBJECT_ID_CAR = 100;
    const AkGameObjectID GAME_OBJECT_NON_RECORDABLE = 200;

    auto *audio = GetCoreSystem( AudioManager );

    audio->LoadBank( init, initID );
    audio->LoadBank( car, carID );
    audio->LoadBank( bgm, bgmID );

    audio->RegisterObject( GAME_OBJECT_ID_CAR, 0x08 );
    audio->PlayEvent( play_CarEngine, GAME_OBJECT_ID_CAR );
}