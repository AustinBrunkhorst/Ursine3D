/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ToggleHudState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ToggleHudState.h"

#include "SegmentLogicStateIncludes.h"

#include "UIScreensConfigComponent.h"

#include <Scene.h>

ToggleHudState::ToggleHudState(bool toggle)
    : SegmentLogicState( toggle ? "Turn Hud On" : "Turn Hud Off" )
    , m_toggle( toggle ) { }

void ToggleHudState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto world = segmentManager->GetOwner( )->GetWorld( );

    auto *scene = world->GetOwner( );

    UAssert( scene != nullptr,
        "Scene was null."    
    );

    auto *ui = world->GetSettings( )->GetComponent<UIScreensConfig>( );

    UAssert( ui != nullptr,
        "UIConfig was null."    
    );

    ui_event::ToggleHUD e;

    e.toggled = m_toggle;
    e.playerID = -1;

    ui->TriggerPlayerHUDEvent( e );
}
