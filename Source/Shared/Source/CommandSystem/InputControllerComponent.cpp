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
#include "PlayerIdComponent.h"

#define HELD PlayerAction::Held
#define RELEASED PlayerAction::Released
#define TRIGGERED PlayerAction::Down
#define UP PlayerAction::Up


NATIVE_COMPONENT_DEFINITION(InputController);

InputController::InputController(void)
    : BaseComponent( )
    , m_keyBoard( false )
{

}

InputController::~InputController(void)
{
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

    PlayerID* idComp = GetOwner( )->GetComponent<PlayerID>( );

    MapCommandList( idComp );
}

void InputController::OnInitialize(void)
{
    SetKeyBoard( m_keyBoard );
}

void InputController::MapXboxContoller(void)
{
    m_action.SetActionMode(PlayerAction::Xbox);

    /////////////////////////////////////////////////////////////////
    // this is all XBOX specific ////////////////////////////////////
    m_moveForward = PlayerAction::LeftStickUp;
    m_moveBack    = PlayerAction::LeftStickDown;
    m_moveLeft    = PlayerAction::LeftStickLeft;
    m_moveRight   = PlayerAction::LeftStickRight;

    m_lookUp    = PlayerAction::RightStickUp;
    m_lookDown  = PlayerAction::RightStickDown;
    m_lookLeft  = PlayerAction::RightStickLeft;
    m_lookRight = PlayerAction::RightStickRight;

    m_pullTrigger = PlayerAction::RightTrigger;
    m_reload      = PlayerAction::Action3;
    m_swap        = PlayerAction::Action4;
    m_jump        = PlayerAction::Action1;
}

void InputController::MapKeyboard(void)
{
    m_action.SetActionMode(PlayerAction::Keyboard);

    /////////////////////////////////////////////////////////////////
    // this is all XBOX specific ////////////////////////////////////
    m_moveForward = PlayerAction::KeyUp;
    m_moveBack    = PlayerAction::KeyDown;
    m_moveLeft    = PlayerAction::KeyLeft;
    m_moveRight   = PlayerAction::KeyRight;
     
    m_lookUp    = PlayerAction::MouseUp;
    m_lookDown  = PlayerAction::MouseDown;
    m_lookLeft  = PlayerAction::MouseLeft;
    m_lookRight = PlayerAction::MouseRight;

    m_pullTrigger = PlayerAction::RightTrigger;
    m_reload      = PlayerAction::Action3;
    m_swap        = PlayerAction::Action4;
    m_jump        = PlayerAction::Action1;
}

void InputController::MapCommandList(PlayerID* idComp)
{
    Clear( );

    m_action.SetPlayerIDComp(idComp);

    ButtonActionCommand* actions = new ButtonActionCommand( &m_action, GetOwner( ) );
    actions->AddActionCommand( Method( m_pullTrigger, HELD, gEvent::FIRE_START ) )
        .AddActionCommand ( Method( m_pullTrigger, RELEASED, gEvent::FIRE_END ) )
        .AddActionCommand( Method( m_reload, HELD, gEvent::INTERACT_COMMAND ) )
        .AddActionCommand( Method( m_reload, RELEASED, gEvent::RELOAD_COMMAND ) )
        .AddActionCommand( Method( m_jump, TRIGGERED, gEvent::JUMP_COMMAND ) )
        .AddActionCommand( Method( m_swap, TRIGGERED, gEvent::SWAP_COMMAND ) );

    PlayerTwoAxisAction* axisActions = new PlayerTwoAxisAction( &m_action, GetOwner( ) );
    axisActions->AddAxisBinding( AxisBinding( m_moveLeft, m_moveRight, m_moveForward, m_moveBack, gEvent::MOVEMENT_COMMAND ) )
        .AddAxisBinding( AxisBinding( m_lookLeft, m_lookRight, m_lookUp, m_lookDown, gEvent::LOOK_COMMAND ) );
    
    m_actionList = { axisActions, actions };
}

std::vector<ActionCommandBase*> &InputController::GetActionList(void)
{
    return m_actionList;
}

void InputController::Clear(void)
{
    for ( auto &action : m_actionList )
        delete action;

    m_actionList.clear( );
}