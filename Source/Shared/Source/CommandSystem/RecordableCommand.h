/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RecordableCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <UrsineTypes.h>

#include "Command.h"
#include "RecordableCommandConfig.h"

class RecordableCommand 
    : public Command
{
public:
    virtual void StartRecording(ursine::ecs::Entity *receiver) = 0;
    virtual void Record(ursine::ecs::Entity *receiver, const ursine::uint64 time) = 0;
    virtual void RecordedExecutionPrep(ursine::ecs::Entity *receiver, const ursine::uint64 time) = 0;

	virtual ursine::uint GetTypeID(void) = 0;

    float GetStartTime(void) const;
	void SetStartTime(ursine::uint64 startTime);

    float GetDuration(void) const;
	void SetDuration(ursine::uint64 duration);

protected:
    ursine::uint64 m_startTime;
    ursine::uint64 m_duration;

	static ursine::uint GetID(void);
};