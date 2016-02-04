/* ----------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractCommand.h
**
** Author:
** - Chad George - chad.george@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "InteractCommand.h"
#include "GameEvents.h"

RECORDABLE_COMMAND_DEFINITION(InteractCommand);

InteractCommand::InteractCommand( )
{
    m_weight = 0;
}

void InteractCommand::Execute(ursine::ecs::Entity* receiver)
{
    receiver->Dispatch(game::INTERACT_COMMAND, ursine::EventArgs::Empty);
}

void InteractCommand::StopExecute(ursine::ecs::Entity* receiver)
{
}

void InteractCommand::StartRecording(ursine::ecs::Entity* receiver)
{

}

void InteractCommand::Record(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{

}

void InteractCommand::RecordedExecutionPrep(ursine::ecs::Entity* receiver, const ursine::uint64 time)
{

}
