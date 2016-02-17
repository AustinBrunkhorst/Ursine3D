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
