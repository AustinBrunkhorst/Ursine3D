/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RecordableCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once
#include "Command.h"

class RecordableCommand 
    : public Command
{
public:
    virtual void StartRecording(ursine::ecs::Entity *receiver) = 0;
    virtual void Record(ursine::ecs::Entity *receiver, const int time) = 0;
    virtual void RecordedExecutionPrep(ursine::ecs::Entity *receiver, const int time) = 0;

    int GetStartTime(void) const;
    int GetDuration(void) const;

protected:
    int m_startTime;
    int m_duration;
};