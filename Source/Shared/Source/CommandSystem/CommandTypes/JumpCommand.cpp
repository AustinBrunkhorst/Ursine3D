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
#include <AudioEmitterComponent.h>
#include <PlayerAnimationComponent.h>

namespace
{
	const std::string kJumpSound = "PLAYER_JUMP";
}

RECORDABLE_COMMAND_DEFINITION( JumpCommand );

JumpCommand::JumpCommand(void)
{
    m_weight = 5;
}

void JumpCommand::Execute(ursine::ecs::Entity* receiver)
{
    auto *controller = receiver->GetComponent<CharacterController>( );
	auto *emitter = receiver->GetComponent<ursine::ecs::AudioEmitterComponent>();

	controller->Jump( );

	if (emitter)
		emitter->AddSoundToPlayQueue(kJumpSound);
}

void JumpCommand::StopExecute(ursine::ecs::Entity* receiver)
{

}

void JumpCommand::StartRecording(ursine::ecs::Entity* receiver)
{
    
}

void JumpCommand::Record(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{
    
}

void JumpCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{
    
}