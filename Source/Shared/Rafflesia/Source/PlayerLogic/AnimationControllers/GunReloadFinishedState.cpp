/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GunReloadFinishedState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "GunReloadFinishedState.h"

#include "AnimatorControllerStateMachine.h"
#include "UIScreensConfigComponent.h"
#include "PlayerIdComponent.h"

#include <AnimatorComponent.h>
#include <Scene.h>
#include <World.h>

GunReloadFinishedState::GunReloadFinishedState(void)
    : AnimatorControllerState( "Animation Message UI" ) { }

void GunReloadFinishedState::Enter(AnimatorControllerStateMachine *machine)
{
    // message the UI that the gun is reloaded
    auto animator = machine->GetAnimator( );
    auto world = animator->GetOwner( )->GetWorld( );
    auto *scene = world->GetOwner( );

    UAssert( scene != nullptr,
        "Scene was null."    
    );

    auto manager = scene->GetGameContext( )->GetManager( );

    UAssert( manager != nullptr,
        "Manager was null."
    );

    auto *ui = manager->GetConfigComponent<UIScreensConfig>( );

    UAssert( ui != nullptr,
        "UIConfig was null."    
    );

    auto player = animator->GetOwner( )->GetRoot( )->GetComponent<PlayerID>( );

    ui_event::PlayerAmmoUpdated e;

    e.percent = 1.0f;
    e.playerID = player->GetID( );

    ui->TriggerPlayerHUDEvent( e );
}
