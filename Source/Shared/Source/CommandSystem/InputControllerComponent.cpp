/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandInputControllerComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "InputControllerComponent.h"
#include <CommandSystem/ActionCommands/ButtonActionCommand.h>
#include <CommandSystem/ActionCommands/TwoAxisActionCommand.h>
#include <CommandSystem/AxisCommandTypes/MoveCommand.h>
#include <CommandSystem/AxisCommandTypes/LookCommand.h>
#include <CommandSystem/CommandTypes/FireCommand.h>
#include <CommandSystem/CommandTypes/JumpCommand.h>
#include <CommandSystem/CommandTypes/InteractCommand.h>
#include <CommandSystem/CommandTypes/SwapCommand.h>
#include <CommandSystem/CommandTypes/ReloadCommand.h>
#include "PlayerIdComponent.h"

NATIVE_COMPONENT_DEFINITION(InputController);

InputController::InputController(void)
    : BaseComponent( )
    , m_keyBoard( false )
{

}

InputController::~InputController(void)
{
    for(auto &x : m_commandList)
    {
        delete x;
    }
}

bool InputController::GetKeyBoard(void) const
{
    return m_keyBoard;
}

void InputController::SetKeyBoard(const bool useKeyBoard)
{
    m_keyBoard = useKeyBoard;

    if (m_keyBoard)
        MapKeyboard( );
    else
        MapXboxContoller( );

    MapCommandList( );
}

void InputController::OnInitialize(void)
{
    SetKeyBoard( m_keyBoard );
}

void InputController::MapXboxContoller( void )
{
    PlayerID* idComp = GetOwner( )->GetComponent<PlayerID>( );

    

    /////////////////////////////////////////////////////////////////
    // this is all XBOX specific ////////////////////////////////////
    m_moveForward = PlayerAction(
        idComp,
        PlayerAction::Xbox,
        PlayerAction::LeftStickUp
        );
    m_moveBack = PlayerAction(
        idComp,
        PlayerAction::Xbox,
        PlayerAction::LeftStickDown
        );
    m_moveLeft = PlayerAction(
        idComp,
        PlayerAction::Xbox,
        PlayerAction::LeftStickLeft
        );
    m_moveRight = PlayerAction(
        idComp,
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
        idComp,
        PlayerAction::Xbox,
        PlayerAction::RightStickUp
        );
    m_lookDown = PlayerAction(
        idComp,
        PlayerAction::Xbox,
        PlayerAction::RightStickDown
        );
    m_lookLeft = PlayerAction(
        idComp,
        PlayerAction::Xbox,
        PlayerAction::RightStickLeft
        );
    m_lookRight = PlayerAction(
        idComp,
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
        idComp,
        PlayerAction::Xbox,
        PlayerAction::RightTrigger
        );
    m_reload = PlayerAction(
        idComp,
        PlayerAction::Xbox,
        PlayerAction::Action3
        );
    m_swap = PlayerAction(
        idComp,
        PlayerAction::Xbox,
        PlayerAction::Action4
        );
    m_jump = PlayerAction(
        idComp,
        PlayerAction::Xbox,
        PlayerAction::Action1
        );
}

void InputController::MapKeyboard(void)
{
    PlayerID* idComp =  GetOwner( )->GetComponent<PlayerID>( );

    /////////////////////////////////////////////////////////////////
    // this is all XBOX specific ////////////////////////////////////
    m_moveForward = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::KeyUp
        );
    m_moveBack = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::KeyDown
        );
    m_moveLeft = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::KeyLeft
        );
    m_moveRight = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::KeyRight
        );

    m_move = PlayerTwoAxisAction(
        m_moveLeft,
        m_moveRight,
        m_moveForward,
        m_moveBack
        );

    m_lookUp = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::MouseUp
        );
    m_lookDown = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::MouseDown
        );
    m_lookLeft = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::MouseLeft
        );
    m_lookRight = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::MouseRight
        );

    m_look = PlayerTwoAxisAction(
        m_lookLeft,
        m_lookRight,
        m_lookUp,
        m_lookDown
        );

    m_fire = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::RightTrigger
        );
    m_reload = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::Action3
        );
    m_swap = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::Action4
        );
    m_jump = PlayerAction(
        idComp,
        PlayerAction::Keyboard,
        PlayerAction::Action1
        );
}

void InputController::MapCommandList(void)
{
    // clear all commands previously created
    for (auto &command : m_commandList)
        delete command;

    m_commandList.clear( );

     m_commandList = {
        new ButtonActionCommand<JumpCommand>(
            m_jump,
            ButtonActionCommand<JumpCommand>::Interaction::Down
        ),

        new TwoAxisActionCommand<MoveCommand>( m_move ),

        new TwoAxisActionCommand<LookCommand>( m_look ),

        new ButtonActionCommand<FireCommand>(
            m_fire,
            ButtonActionCommand<FireCommand>::Interaction::Held
        ),
        
        new ButtonActionCommand<InteractCommand>(
            m_reload,
            ButtonActionCommand<InteractCommand>::Interaction::Held
        ),

        new ButtonActionCommand<InteractCommand>(
             m_reload,
             ButtonActionCommand<InteractCommand>::Interaction::Up
        ),

        new ButtonActionCommand<SwapCommand>(
            m_swap,
            ButtonActionCommand<SwapCommand>::Interaction::Down
        )
    };
}

std::vector<ActionCommandBase*> &InputController::GetCommandList(void)
{
    return m_commandList;
}