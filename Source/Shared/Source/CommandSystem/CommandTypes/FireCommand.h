/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FireCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "Command.h"

class FireCommand
    : public Command
{
public:
    FireCommand(void);

    void Execute(ursine::ecs::Entity *receiver) override;
    void StopExecute(ursine::ecs::Entity *receiver) override;

};