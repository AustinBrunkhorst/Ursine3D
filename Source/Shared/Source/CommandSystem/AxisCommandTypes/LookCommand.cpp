/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LookCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LookCommand.h"
#include "GameEvents.h"
#include <CharacterControllerComponent.h>
#include <Model3DComponent.h>

LookCommand::LookCommand() 
    : AxisCommand(ursine::Vec2(0,0)) { }

LookCommand::LookCommand(const ursine::Vec2& axis) 
    : AxisCommand(axis) { }


void LookCommand::Execute(ursine::ecs::Entity* receiver)
{
    game::MovementEventArgs lookChange( m_axis );
    receiver->Dispatch( game::LOOK_COMMAND, &lookChange );

}

void LookCommand::StopExecute(ursine::ecs::Entity* receiver)
{

}
