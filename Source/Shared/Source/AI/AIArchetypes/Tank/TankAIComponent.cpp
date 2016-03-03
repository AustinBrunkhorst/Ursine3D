/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TankAIComponent.cpp
**
** Author:
** - Joshua Shlemmer- joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "TankAIComponent.h"

//#include <FloatCondition.h>

#include <SystemManager.h>

// use this for anything you want to draw in the editor
//#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( TankAI );

using namespace ursine;
using namespace ecs;

TankAI::TankAI(void)
    :BaseComponent()
    ,m_stateMachine( nullptr )
{
}

TankAI::~TankAI(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .Off( WORLD_UPDATE, &TankAI::onUpdate );
}

void TankAI::OnInitialize(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .On( WORLD_UPDATE, &TankAI::onUpdate );

    // set up the state machine
}

void TankAI::onUpdate(EVENT_HANDLER(World))
{
    // update our state machine
    m_stateMachine.Update();
}
