/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SwapCommand.h
**
** Author:
** - Chad George - chad.george@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SwapCommand.h"
#include "GameEvents.h"

RECORDABLE_COMMAND_DEFINITION(SwapCommand);

SwapCommand::SwapCommand( )
{
    m_weight = 0;
}

void SwapCommand::Execute(ursine::ecs::Entity* receiver)
{
    receiver->Dispatch(game::SWAP_COMMAND, ursine::EventArgs::Empty);
}

void SwapCommand::StopExecute(ursine::ecs::Entity* receiver)
{
}

void SwapCommand::StartRecording(ursine::ecs::Entity* receiver)
{

}

void SwapCommand::Record(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{

}

void SwapCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{

}
