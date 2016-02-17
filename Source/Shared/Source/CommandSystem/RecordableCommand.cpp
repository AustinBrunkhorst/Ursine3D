/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
    return static_cast<float>(m_startTime);
}

void RecordableCommand::SetStartTime(ursine::uint64 startTime)
{
    m_startTime = startTime;
}

float RecordableCommand::GetDuration(void) const
{
    return static_cast<float>(m_duration);
}

void RecordableCommand::SetDuration(ursine::uint64 duration)
{
    m_duration = duration;
}

ursine::uint RecordableCommand::GetID(void)
{
    static ursine::uint id = 0;

    return id++;
}