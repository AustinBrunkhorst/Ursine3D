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
#include "ActionCommands/ButtonActionCommand.h"
#include "PlayerIDComponent.h"

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
    std::vector<ActionCommandBase*> &GetActionList(void);

private:
    typedef ButtonActionCommand::Interactions Interactions;
    typedef ButtonActionCommand::InteractionMethod Method;
    typedef PlayerTwoAxisAction::AxisBinding AxisBinding;
    typedef game::GameEvents gEvent;
    typedef PlayerAction::InputBinding Binding;

    PlayerAction m_action;

    // action commands
    std::vector<ActionCommandBase*> m_actionList;

    // all of our actions
    Binding m_moveForward,
            m_moveBack, 
            m_moveLeft,
            m_moveRight;

    Binding m_lookUp,
            m_lookDown,
            m_lookLeft,
            m_lookRight;

    // weapon actions
    Binding m_pullTrigger,
            m_reload,
            m_swap,
            m_jump;

    // is user using keyboard
    bool m_keyBoard;

    void MapXboxContoller(void);
    void MapKeyboard(void);

    void MapCommandList(PlayerID* idComp);

    void Clear(void);

}Meta(Enable, DisplayName("InputController"), RequiresComponents(typeof(PlayerID)));
