/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ButtonActionCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "ActionCommand.h"
#include "PlayerAction.h"

class ButtonActionCommand
    : public ActionCommandBase
{
public:
    Meta(Disable)
    struct InteractionMethod
    {
        PlayerAction::InputBinding binding;
        PlayerAction::Interaction interactionType;
        game::GameEvents eventToSend;

        InteractionMethod(void);
        InteractionMethod(PlayerAction::InputBinding bind, PlayerAction::Interaction type, game::GameEvents commandEvent);
        InteractionMethod(const InteractionMethod& rhs);

        ~InteractionMethod(void);
    };

    typedef std::vector<InteractionMethod> Interactions;

    ButtonActionCommand(PlayerAction* action, ursine::ecs::Entity* owner);
    ButtonActionCommand(PlayerAction* action, ursine::ecs::Entity* owner, Interactions& methods);

    ButtonActionCommand& AddActionCommand(InteractionMethod&& interaction);

    void ProcessCommands(void) override;

protected:
    bool Acting(const InteractionMethod& method);

private:
    PlayerAction* m_action;
    ursine::ecs::Entity* m_owner;
    Interactions m_interactionMethods;

};


