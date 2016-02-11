/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RecordableAxisCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "RecordableCommand.h"

class RecordableAxisCommand 
    : public RecordableCommand
{
public:
    RecordableAxisCommand(const ursine::Vec2 &axis);

    inline const ursine::Vec2 &GetAxis(void) const;

protected:
    ursine::Vec2 m_axis;
};