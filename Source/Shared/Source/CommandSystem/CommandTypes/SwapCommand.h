#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SwapCommand.h
**
** Author:
** - Chad George - chad.george@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "Command.h"

class SwapCommand
    : public Command
{
public:
    SwapCommand(void);

    void Execute(ursine::ecs::Entity *receiver) override;
    void StopExecute(ursine::ecs::Entity *receiver) override;

};
