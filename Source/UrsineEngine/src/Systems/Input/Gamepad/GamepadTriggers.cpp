/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadTriggers.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GamepadTriggers.h"

namespace ursine
{
    float GamepadTriggers::Left(void) const
    {
        return _left;
    }

    float GamepadTriggers::Right(void) const
    {
        return _right;
    }

    void GamepadTriggers::setLeft(int16 value)
    {
        _left = Math::Clamp(value / VALUE_MAX, 0.0f, 1.0f);
    }

    void GamepadTriggers::setRight(int16 value)
    {
        _right = Math::Clamp(value / VALUE_MAX, 0.0f, 1.0f);
    }

    void GamepadTriggers::setLeft(float value)
    {
        _left = Math::Clamp(value, 0.0f, 1.0f);
    }

    void GamepadTriggers::setRight(float value)
    {
        _right = Math::Clamp(value, 0.0f, 1.0f);
    }
}