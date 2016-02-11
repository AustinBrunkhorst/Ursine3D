/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    const float GamepadThumbSticks::VALUE_MAX = 32767.0f;

    const Vec2 &GamepadThumbSticks::Left(void) const
    {
        return m_left;
    }

    const Vec2 &GamepadThumbSticks::Right(void) const
    {
        return m_right;
    }

    void GamepadThumbSticks::setLeft(int16 x, int16 y)
    {
        m_left.X() = x / VALUE_MAX;
        m_left.Y() = y / VALUE_MAX;
    }

    void GamepadThumbSticks::setRight(int16 x, int16 y)
    {
        m_right.X() = x / VALUE_MAX;
        m_right.Y() = y / VALUE_MAX;
    }

    void GamepadThumbSticks::setLeft(const Vec2 &left)
    {
        m_left =
        {
            math::Clamp( left.X(), -1.0f, 1.0f ),
            math::Clamp( left.Y(), -1.0f, 1.0f )
        };
    }

    void GamepadThumbSticks::setRight(const Vec2 &right)
    {
        m_right =
        {
            math::Clamp( right.X(), -1.0f, 1.0f ),
            math::Clamp( right.Y(), -1.0f, 1.0f )
        };
    }

    void GamepadThumbSticks::applyDeadZone(GamepadDeadZoneType type, float size)
    {
        switch (type)
        {
            case DZ_NONE:
                break;
            case DZ_INDEPENDENT_AXES:
                if (abs( m_left.X() ) < size)
                    m_left.X() = 0.0f;
                if (abs( m_left.Y() ) < size)
                    m_left.Y() = 0.0f;
                if (abs( m_right.X() ) < size)
                    m_right.X() = 0.0f;
                if (abs( m_right.Y() ) < size)
                    m_right.Y() = 0.0f;
                break;
            case DZ_CIRCULAR:
                const float size_squared = size * size;

                if (m_left.LengthSquared( ) < size_squared)
                    m_left = Vec2::Zero( );

                if (m_right.LengthSquared( ) < size_squared)
                    m_right = Vec2::Zero( );
                break;
        }
    }
}
