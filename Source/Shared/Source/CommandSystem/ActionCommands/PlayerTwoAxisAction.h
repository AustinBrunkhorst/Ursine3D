/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerTwoAxisAction.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "PlayerAction.h"
#include <Entity.h>
#include "ActionCommand.h"


class Command;

class PlayerTwoAxisAction : public ActionCommandBase
{   
public:
    typedef PlayerAction::InputBinding Binding;
    
    Meta(Disable)
    struct AxisBinding
    {
        AxisBinding(Binding left, Binding right, Binding up, Binding down, game::GameEvents commandEvent);

        AxisBinding(const AxisBinding& rhs);

        PlayerAction::InputBinding m_left,
                                   m_right,
                                   m_up,
                                   m_down;

        game::GameEvents m_eventToSend;
    };

    PlayerTwoAxisAction(void);
    PlayerTwoAxisAction(PlayerAction* action, const ursine::ecs::EntityHandle &owner);

    PlayerTwoAxisAction& AddAxisBinding(AxisBinding& binding);

    void ProcessCommands(void);

private:
    PlayerAction *m_action;
    ursine::ecs::EntityHandle m_owner;

    std::vector<AxisBinding> m_axisBindings;

    ursine::Vec2 m_axis;

    bool Acting(AxisBinding& bindings);

};

