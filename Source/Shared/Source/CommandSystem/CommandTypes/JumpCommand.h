/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JumpCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "Command.h"

class JumpCommand
    : public Command
{
public:
    JumpCommand(void);

    void Execute(ursine::ecs::Entity *receiver) override;
    void StopExecute(ursine::ecs::Entity *receiver) override;

};