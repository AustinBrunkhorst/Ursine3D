#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractCommand.h
**
** Author:
** - Chad George - chad.george@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "RecordableCommand.h"

class InteractCommand
    : public RecordableCommand
{
    RECORDABLE_COMMAND;

public:
    InteractCommand(void);

    void Execute(ursine::ecs::Entity *receiver) override;
    void StopExecute(ursine::ecs::Entity *receiver) override;

    void StartRecording(ursine::ecs::Entity *receiver) override;
    void Record(ursine::ecs::Entity *receiver, const ursine::uint64 time) override;
    void RecordedExecutionPrep(ursine::ecs::Entity *receiver, const ursine::uint64 time) override;

};
