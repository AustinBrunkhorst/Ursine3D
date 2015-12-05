/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LookCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "RecordableAxisCommand.h"

class LookCommand
    : public RecordableAxisCommand
{
public:
    LookCommand(void);
    LookCommand(const ursine::Vec2 &axis);

    void Execute(ursine::ecs::Entity *receiver) override;
    void StopExecute(ursine::ecs::Entity *receiver) override;

    void StartRecording(ursine::ecs::Entity *receiver) override;
    void Record(ursine::ecs::Entity *receiver, const int time) override;
    void RecordedExecutionPrep(ursine::ecs::Entity *receiver, const int time) override;

private:
    std::vector<ursine::SQuat> m_characterRot;
    std::vector<ursine::SQuat> m_camRot;

    bool m_playback;
};