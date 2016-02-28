/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerTwoAxisAction.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "PlayerTwoAxisAction.h"

PlayerTwoAxisAction::AxisBinding::AxisBinding(Binding left, Binding right, Binding up, Binding down, game::GameEvents commandEvent)
    : m_left( left )
    , m_right( right )
    , m_up( up )
    , m_down( down )
    , m_eventToSend( commandEvent )
{ }

PlayerTwoAxisAction::AxisBinding::AxisBinding(const AxisBinding& rhs)
    : m_left( rhs.m_left )
    , m_right( rhs.m_right )
    , m_up( rhs.m_up )
    , m_down( rhs.m_down )
    , m_eventToSend( rhs.m_eventToSend )
{ }

PlayerTwoAxisAction::PlayerTwoAxisAction()
    : m_action( nullptr )
    , m_owner( nullptr )
{ }

PlayerTwoAxisAction::PlayerTwoAxisAction(PlayerAction* action, ursine::ecs::Entity* owner)
    : m_action( action )
    , m_owner( owner )
{
}

PlayerTwoAxisAction& PlayerTwoAxisAction::AddAxisBinding(AxisBinding& binding)
{
    m_axisBindings.push_back(binding);
    return *this;
}

void PlayerTwoAxisAction::ProcessCommands(void)
{
    for ( auto bindings : m_axisBindings )
    {
        if ( Acting(bindings) )
        {
            game::MovementEventArgs axisChange(m_axis);
            m_owner->Dispatch(bindings.m_eventToSend, &axisChange);
        }
    }
}

bool PlayerTwoAxisAction::Acting(AxisBinding& bindings)
{
    if (
        m_action->StickLeft(bindings.m_left)   ||
        m_action->StickRight(bindings.m_right) ||
        m_action->StickUp(bindings.m_up)       ||
        m_action->StickDown(bindings.m_down)
       )
    {
        m_action->GetAxis(bindings.m_left, m_axis);
        return true;
    }

    return false;
}
