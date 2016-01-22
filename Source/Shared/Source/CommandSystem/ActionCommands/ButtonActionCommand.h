/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ButtonActionCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "ActionCommand.h"
#include "PlayerAction.h"

template<typename T>
class ButtonActionCommand
    : public ActionCommand<T>
{
public:
    enum Interaction
    {
        Down,
        Held,
        Up
    };

public:
    ButtonActionCommand(PlayerAction action, Interaction type);

    bool Acting(void) override;

    std::shared_ptr<Command> CreateCommand(void) override;

private:
    PlayerAction m_action;
    Interaction m_type;
};

/////////////////////////////////////////////////////////////////////

template<typename T>
ButtonActionCommand<T>::ButtonActionCommand(PlayerAction action, Interaction type)
    : m_action(action)
    , m_type(type) {}

template<typename T>
bool ButtonActionCommand<T>::Acting()
{
    switch(m_type)
    {
    case Down:
        return m_action.WasPressed( );
    case Held:
        return m_action.IsPressed( );
    default:
        return m_action.WasReleased( );
    }
}

// return a copy of our command
template<typename T>
std::shared_ptr<Command> ButtonActionCommand<T>::CreateCommand()
{
    return std::make_shared<T>();
}