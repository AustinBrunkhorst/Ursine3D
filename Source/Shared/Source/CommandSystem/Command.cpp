/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Command.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "Command.h"

Command::~Command() {}

void Command::Execute(ursine::ecs::Entity* receiver) {}

void Command::StopExecute(ursine::ecs::Entity* receiver) {}

unsigned Command::GetWeight() const
{
    return m_weight;
}