/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ButtonActionCommand.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "ButtonActionCommand.h"

ButtonActionCommand::InteractionMethod::InteractionMethod(PlayerAction::InputBinding bind, PlayerAction::Interaction type, game::GameEvents commandEvent) 
    : binding( bind )
    , interactionType( type )
    , eventToSend( commandEvent ) { }

ButtonActionCommand::InteractionMethod::InteractionMethod(const InteractionMethod &rhs)
    : binding( rhs.binding )
    , interactionType( rhs.interactionType )
    , eventToSend( rhs.eventToSend ) { }

ButtonActionCommand::InteractionMethod::~InteractionMethod(void) { }

/////////////////////////////////////////////////////////////////////

ButtonActionCommand::ButtonActionCommand(PlayerAction *action, const ursine::ecs::EntityHandle &owner)
    : m_action( action )
    , m_owner( owner ) { }


ButtonActionCommand::ButtonActionCommand(PlayerAction *action, const ursine::ecs::EntityHandle &owner, Interactions &methods)
    : m_action( action )
    , m_owner( owner )
    , m_interactionMethods( methods ) { }

ButtonActionCommand &ButtonActionCommand::AddActionCommand(InteractionMethod &&interaction)
{
    m_interactionMethods.emplace_back( std::move( interaction ) );

    return *this;
}

void ButtonActionCommand::ProcessCommands(void)
{
    for (auto method : m_interactionMethods)
    {
        if (Acting( method ))
             m_owner->Dispatch( method.eventToSend, ursine::EventArgs::Empty );
    }
}

bool ButtonActionCommand::Acting(const InteractionMethod &method)
{
    switch (method.interactionType)
    {
        case PlayerAction::Down:
            return m_action->WasPressed( method.binding );
        case PlayerAction::Held:
            return m_action->IsPressed( method.binding );
        case PlayerAction::Released:
            return m_action->WasReleased( method.binding );
        case PlayerAction::Up:
            return m_action->IsUp( method.binding );
        default:
            return false;
    }
}
