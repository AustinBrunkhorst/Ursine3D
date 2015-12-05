/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JumpCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "JumpCommand.h"
#include <CharacterControllerComponent.h>

RECORDABLE_COMMAND_DEFINITION( JumpCommand );

JumpCommand::JumpCommand(void)
{
    m_weight = 5;
}

void JumpCommand::Execute(ursine::ecs::Entity* receiver)
{
    auto *controller = receiver->GetComponent<CharacterController>( );

	controller->jump = true;
}

void JumpCommand::StopExecute(ursine::ecs::Entity* receiver)
{
	auto *controller = receiver->GetComponent<CharacterController>();

	controller->jump = false;
}

void JumpCommand::StartRecording(ursine::ecs::Entity* receiver)
{
    
}

void JumpCommand::Record(ursine::ecs::Entity* receiver, const float time)
{
    
}

void JumpCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const float time)
{
    
}