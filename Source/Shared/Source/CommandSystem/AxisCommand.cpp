/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AxisCommand.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "AxisCommand.h"

AxisCommand::AxisCommand(const ursine::Vec2& axis) 
    : m_axis(axis) {}

const ursine::Vec2& AxisCommand::GetAxis() const
{
    return m_axis;
}