/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScreenFadeState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ScreenFadeState.h"

#include "SegmentLogicStateIncludes.h"

#include "UIScreensConfigComponent.h"

#include <Scene.h>

ScreenFadeState::ScreenFadeState(float inTime, float pauseTime, float outTime, const ursine::Color &color)
    : SegmentLogicState( "Screen Fade State" )
    , m_inTime( inTime )
    , m_pauseTime( pauseTime )
    , m_outTime( outTime )
    , m_color( color ) { }

void ScreenFadeState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto world = segmentManager->GetOwner( )->GetWorld( );

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

    ui_event::ScreenFadeEffect e;

    e.fadeInTime = m_inTime;
    e.pauseTime = m_pauseTime;
    e.fadeOutTime = m_outTime;
    e.screenColor = m_color;

    ui->TriggerPlayerHUDEvent( e );
}
