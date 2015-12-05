/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RecordableCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "RecordableCommand.h"

float RecordableCommand::GetStartTime(void) const
{
    return m_startTime;
}

void RecordableCommand::SetStartTime(float startTime)
{
	m_startTime = startTime;
}

float RecordableCommand::GetDuration(void) const
{
    return m_duration;
}

void RecordableCommand::SetDuration(float duration)
{
	m_duration = duration;
}

ursine::uint RecordableCommand::GetID(void)
{
	static ursine::uint id = 0;

	return id++;
}