/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RecordableAxisCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "RecordableAxisCommand.h"

RecordableAxisCommand::RecordableAxisCommand(const ursine::Vec2& axis)
    : m_axis(axis) {}

const ursine::Vec2& RecordableAxisCommand::GetAxis() const
{
    return m_axis;
}