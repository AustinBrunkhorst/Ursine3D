/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

// this needs to go in another header
// what input will we listen to?

// main class for individual input controllers
class InputController
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
        Reload,
        Swap,

        ActionCount
    };

public:
    InputController(void);
    ~InputController(void);

    void OnInitialize(void) override;

    EditorField(
        bool UseKeyboard,
        GetKeyBoard,
        SetKeyBoard
        );
    bool GetKeyBoard(void) const;
    void SetKeyBoard(const bool useKeyBoard);

    Meta(Disable)
    std::vector<ActionCommandBase*> &GetCommandList(void);

private:
    // is user using keyboard
    bool m_keyBoard;

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

    // weapon actions
    PlayerAction    m_fire,
                    m_reload,
                    m_swap;

    PlayerAction m_jump;

    void MapXboxContoller(void);
    void MapKeyboard(void);

    void MapCommandList(void);

}Meta(Enable, DisplayName("InputController"));
