/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MoveCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "MoveCommand.h"
#include <RigidbodyComponent.h>
#include <CharacterControllerComponent.h>

MoveCommand::MoveCommand() 
    : AxisCommand( ursine::Vec2(0, 0)) {}

MoveCommand::MoveCommand(const ursine::Vec2& axis) 
    : AxisCommand( axis )
{
}

void MoveCommand::Execute(ursine::ecs::Entity* receiver)
{
    auto *controller = receiver->GetComponent<CharacterController>( );

    controller->SetMoveDirection( m_axis );
}

void MoveCommand::StopExecute(ursine::ecs::Entity* receiver)
{

}
