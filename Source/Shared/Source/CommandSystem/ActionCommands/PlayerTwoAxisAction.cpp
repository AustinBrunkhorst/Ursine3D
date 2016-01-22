/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerTwoAxisAction.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerTwoAxisAction.h"

#include <PlayerInputComponent.h>


PlayerTwoAxisAction::PlayerTwoAxisAction()
    : m_left()
    , m_right()
    , m_up()
    , m_down() {}


PlayerTwoAxisAction::PlayerTwoAxisAction(const PlayerAction &leftAction, const PlayerAction &rightAction, const PlayerAction &upAction, const PlayerAction &downAction)
    : m_left(leftAction)
    , m_right(rightAction)
    , m_up(upAction)
    , m_down(downAction) {}

bool PlayerTwoAxisAction::Acting()
{
    return m_left.StickLeft() || m_right.StickRight() || m_up.StickUp() || m_down.StickDown();
}

ursine::Vec2 PlayerTwoAxisAction::GetAxis() const
{
    return m_left.GetAxis();
}