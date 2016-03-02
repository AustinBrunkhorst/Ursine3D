/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSeedshotState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossSeedshotState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include "GameEvents.h"

#include <Application.h>

using namespace ursine;

BossSeedshotState::BossSeedshotState(void)
    : BossAIState( "Seedshot" )
    , m_timer( 0.0f )
    , m_on( false )
{
}

void BossSeedshotState::Update(BossAIStateMachine *machine)
{
    auto dt = Application::Instance->GetDeltaTime( );

    m_timer += dt;

    if (m_timer >= 2.0f)
    {
        auto seedshot = machine->GetBoss( )->GetSeedshotEntity( );

        if (!m_on)
            seedshot->Dispatch( game::FIRE_START, EventArgs::Empty );
        else
            seedshot->Dispatch( game::FIRE_END, EventArgs::Empty );

        m_timer = 0.0f;
    }
}
