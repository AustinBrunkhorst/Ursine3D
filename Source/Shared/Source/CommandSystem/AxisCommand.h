/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AxisCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "Command.h"

class AxisCommand
    : public Command
{
public:
    AxisCommand(const ursine::Vec2 &axis);

    inline const ursine::Vec2 &GetAxis(void) const;

protected:
    ursine::Vec2 m_axis;
};