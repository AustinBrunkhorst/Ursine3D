/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadThumbSticks.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Vec2.h"
#include "GamepadDeadzoneType.h"

#include "UrsineTypes.h"

namespace ursine
{
    class GamepadThumbSticks
    {
        friend class GamepadManager;
        friend class GamepadState;
        friend class VirtualGamepad;

        const static float VALUE_MAX;

        Vec2 m_left;
        Vec2 m_right;

        void setLeft(int16 x, int16 y);
        void setRight(int16 x, int16 y);

        void setLeft(const Vec2 &left);
        void setRight(const Vec2 &right);

        void applyDeadZone(GamepadDeadZoneType type, float size);
    public:
        const Vec2 &Left(void) const;
        const Vec2 &Right(void) const;
    };
}