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
    virtual void Record(ursine::ecs::Entity *receiver, const float time) = 0;
    virtual void RecordedExecutionPrep(ursine::ecs::Entity *receiver, const float time) = 0;

    float GetStartTime(void) const;
    float GetDuration(void) const;

protected:
    float m_startTime;
    float m_duration;
};