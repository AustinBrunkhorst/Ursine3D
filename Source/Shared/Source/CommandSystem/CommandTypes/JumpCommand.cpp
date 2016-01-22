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
#include <AudioEmitterComponent.h>
#include "CommandEvents.h"

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
    URSINE_TODO("possibly move jump sound out of command");

	auto *emitter = receiver->GetComponent<ursine::ecs::AudioEmitter>();

	if (emitter)
		emitter->AddSoundToPlayQueue(kJumpSound);
    
    commandEvent::CommandEventArgs jumpArgs( commandEvent::JUMP_COMMAND );
    receiver->Dispatch(commandEvent::JUMP_COMMAND, &jumpArgs);
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