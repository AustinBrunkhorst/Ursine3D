/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadThumbSticks.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GamepadThumbSticks.h"

namespace Ursine
{
    const float GamepadThumbSticks::VALUE_MAX = 32767.0f;

    const Vector2 &GamepadThumbSticks::Left(void) const
    {
        return m_left;
    }

    const Vector2 &GamepadThumbSticks::Right(void) const
    {
        return m_right;
    }

    void GamepadThumbSticks::setLeft(int16 x, int16 y)
    {
        m_left.x = x / VALUE_MAX;
        m_left.y = y / VALUE_MAX;
    }

    void GamepadThumbSticks::setRight(int16 x, int16 y)
    {
        m_right.x = x / VALUE_MAX;
        m_right.y = y / VALUE_MAX;
    }

    void GamepadThumbSticks::setLeft(const Vector2 &left)
    {
        m_left =
        {
            Math::Clamp( left.x, -1.0f, 1.0f ),
            Math::Clamp( left.y, -1.0f, 1.0f )
        };
    }

    void GamepadThumbSticks::setRight(const Vector2 &right)
    {
        m_right =
        {
            Math::Clamp( right.x, -1.0f, 1.0f ),
            Math::Clamp( right.y, -1.0f, 1.0f )
        };
    }

    void GamepadThumbSticks::applyDeadZone(GamepadDeadZoneType type, float size)
    {
        switch (type)
        {
            case DZ_NONE:
                break;
            case DZ_INDEPENDENT_AXES:
                if (abs( m_left.x ) < size)
                    m_left.x = 0.0f;
                if (abs( m_left.y ) < size)
                    m_left.y = 0.0f;
                if (abs( m_right.x ) < size)
                    m_right.x = 0.0f;
                if (abs( m_right.y ) < size)
                    m_right.y = 0.0f;
                break;
            case DZ_CIRCULAR:
                const float size_squared = size * size;

                if (m_left.LengthSquared( ) < size_squared)
                    m_left = Vector2::Zero( );

                if (m_right.LengthSquared( ) < size_squared)
                    m_right = Vector2::Zero( );
                break;
        }
    }
}
