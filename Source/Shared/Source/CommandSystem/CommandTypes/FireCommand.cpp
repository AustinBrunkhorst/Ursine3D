/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FireCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "FireCommand.h"
#include "GameEvents.h"

FireCommand::FireCommand()
{
    m_weight = 0;
}

void FireCommand::Execute(ursine::ecs::Entity* receiver)
{
    receiver->Dispatch(game::FIRE_START, ursine::EventArgs::Empty);
}

void FireCommand::StopExecute(ursine::ecs::Entity* receiver)
{
    receiver->Dispatch(game::FIRE_END, ursine::EventArgs::Empty);
}
