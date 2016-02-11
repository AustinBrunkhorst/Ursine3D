/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ReloadCommand.h
**
** Author:
** - Chad George - chad.george@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ReloadCommand.h"
#include "GameEvents.h"
#include <CharacterFireControllerComponent.h>

RECORDABLE_COMMAND_DEFINITION(ReloadCommand);

ReloadCommand::ReloadCommand( )
{
    m_weight = 0;
}

void ReloadCommand::Execute(ursine::ecs::Entity* receiver)
{
    receiver->Dispatch(game::RELOAD_COMMAND, ursine::EventArgs::Empty);
}

void ReloadCommand::StopExecute(ursine::ecs::Entity* receiver)
{
}

void ReloadCommand::StartRecording(ursine::ecs::Entity* receiver)
{

}

void ReloadCommand::Record(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{

}

void ReloadCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{

}
