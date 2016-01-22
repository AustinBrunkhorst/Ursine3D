/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandInputControllerComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include <CommandSystem/ActionCommands/PlayerAction.h>
#include <CommandSystem/ActionCommands/PlayerTwoAxisAction.h>
#include <CommandSystem/ActionCommands/ActionCommand.h>
#include <CommandSystem/CommandTypes/JumpCommand.h>

// this needs to go in another header
// what input will we listen to?

// main class for individual input controllers
class CommandInputController
    : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    enum PlayerActions
    {
        Jump = 0,
        Move,
        Look,
        Shoot,
        ActionCount
    };

public:
    CommandInputController(void);
    ~CommandInputController(void);

    void OnInitialize(void) override;

    Meta(Disable)
    std::vector<ActionCommandBase*> &GetCommandList(void);

private:
    // action commands
    std::vector<ActionCommandBase*> m_commandList;

    // all of our actions
    PlayerAction    m_moveForward,
                    m_moveBack, 
                    m_moveLeft,
                    m_moveRight;
                 
    PlayerTwoAxisAction m_move;

    PlayerAction    m_lookUp,
                    m_lookDown,
                    m_lookLeft,
                    m_lookRight;

    PlayerTwoAxisAction m_look;

    PlayerAction m_fire;

    PlayerAction m_jump;
}Meta(Enable, DisplayName("CommandInputController"));
