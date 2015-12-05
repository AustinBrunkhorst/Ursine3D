/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandInputControllerComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CommandInputControllerComponent.h"
#include <CommandSystem/ActionCommands/ButtonActionCommand.h>
#include <CommandSystem/ActionCommands/TwoAxisActionCommand.h>
#include <CommandSystem/AxisCommandTypes/MoveCommand.h>
#include <CommandSystem/AxisCommandTypes/LookCommand.h>
#include <CommandSystem/CommandTypes/FireCommand.h>

NATIVE_COMPONENT_DEFINITION(CommandInputController);

CommandInputController::CommandInputController()
    : BaseComponent()
{

}

CommandInputController::~CommandInputController()
{
    for(auto &x : m_commandList)
    {
        delete x;
    }
}

void CommandInputController::OnInitialize()
{
    /////////////////////////////////////////////////////////////////
    // this is all XBOX specific ////////////////////////////////////
    m_moveForward = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::LeftStickUp
        );
    m_moveBack = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::LeftStickDown
        );
    m_moveLeft = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::LeftStickLeft
        );
    m_moveRight = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::LeftStickRight
        );

    m_move = PlayerTwoAxisAction(
        m_moveLeft,
        m_moveRight,
        m_moveForward,
        m_moveBack
        );

    m_lookUp = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::RightStickUp
        );
    m_lookDown = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::RightStickDown
        );
    m_lookLeft = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::RightStickLeft
        );
    m_lookRight = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::RightStickRight
        );

    m_look = PlayerTwoAxisAction(
        m_lookLeft,
        m_lookRight,
        m_lookUp,
        m_lookDown
        );

    m_fire = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::RightTrigger
        );
    m_jump = PlayerAction(
        GetOwner(),
        PlayerAction::Xbox,
        PlayerAction::Action1
        );

    m_commandList = {
        new ButtonActionCommand<JumpCommand>(
        m_jump,
            ButtonActionCommand<JumpCommand>::Interaction::Down
            ),

        new TwoAxisActionCommand<MoveCommand>(m_move),

        new TwoAxisActionCommand<LookCommand>(m_look),

        new ButtonActionCommand<FireCommand>(
            m_fire,
            ButtonActionCommand<FireCommand>::Interaction::Held
            )
    };
}

std::vector<ActionCommandBase*>& CommandInputController::GetCommandList()
{
    return m_commandList;
}