/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MoveCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "RecordableAxisCommand.h"

class MoveCommand
    : public RecordableAxisCommand
{
	RECORDABLE_COMMAND;

public:
    MoveCommand();
    MoveCommand(const ursine::Vec2 &axis);

    void Execute(ursine::ecs::Entity *receiver) override;
    void StopExecute(ursine::ecs::Entity *receiver) override;

    void StartRecording(ursine::ecs::Entity *receiver) override;
    void Record(ursine::ecs::Entity *receiver, const ursine::uint64 time) override;
    void RecordedExecutionPrep(ursine::ecs::Entity *receiver, const ursine::uint64 time) override;

private:
    std::vector<ursine::SVec3> m_positionList;
    bool m_playback = false;
    ursine::SVec3 m_position;
};