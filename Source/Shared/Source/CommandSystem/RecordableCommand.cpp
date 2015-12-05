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

int RecordableCommand::GetStartTime() const
{
    return m_startTime;
}

int RecordableCommand::GetDuration() const
{
    return m_duration;
}