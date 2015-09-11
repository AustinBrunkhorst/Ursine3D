/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadTriggers.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    class GamepadTriggers
    {
        friend class GamepadManager;
        friend class GamepadState;
        friend class VirtualGamepad;

        const float VALUE_MAX = 32767.0f;

        float _left = 0.0f;
        float _right = 0.0f;

        void setLeft(int16 value);
        void setRight(int16 value);

        void setLeft(float value);
        void setRight(float value);

    public:
        float Left(void) const;
        float Right(void) const;
    };
}
