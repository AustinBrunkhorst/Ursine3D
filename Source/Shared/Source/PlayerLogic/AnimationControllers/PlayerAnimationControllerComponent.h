/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerAnimationControllerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <AnimatorComponent.h>

#include "AnimatorControllerStateMachine.h"
#include "AnimatorControllerState.h"
#include "PlayAnimationState.h"
#include "HealthComponent.h"
#include "CharacterControllerComponent.h"

#include <AudioItemEventData.h>
#include <SweptControllerComponent.h>

class PlayerAnimationController : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        std::string idleState,
        GetIdleState,
        SetIdleState
    );

    EditorField(
        std::string runState,
        GetRunState,
        SetRunState
    );

    EditorField(
        std::string runLeftState,
        GetRunLeftState,
        SetRunLeftState
    );

    EditorField(
        std::string runRightState,
        GetRunRightState,
        SetRunRightState
    );

    EditorField(
        std::string jumpInState,
        GetJumpInState,
        SetJumpInState
    );

    EditorField(
        std::string jumpLoopState,
        GetJumpLoopState,
        SetJumpLoopState
    );

    EditorField(
        std::string jumpOutState,
        GetJumpOutState,
        SetJumpOutState
    );

    EditorField(
        std::string dieState,
        GetDieState,
        SetDieState
    );

    EditorField(
        std::string winState,
        GetWinState,
        SetWinState
    );

    PlayerAnimationController(void);
    ~PlayerAnimationController(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const std::string &GetIdleState(void) const;
    void SetIdleState(const std::string &state);

    const std::string &GetRunState(void) const;
    void SetRunState(const std::string &state);

    const std::string &GetRunLeftState(void) const;
    void SetRunLeftState(const std::string &state);

    const std::string &GetRunRightState(void) const;
    void SetRunRightState(const std::string &state);

    const std::string &GetJumpInState(void) const;
    void SetJumpInState(const std::string &state);

    const std::string &GetJumpLoopState(void) const;
    void SetJumpLoopState(const std::string &state);

    const std::string &GetJumpOutState(void) const;
    void SetJumpOutState(const std::string &state);

    const std::string &GetDieState(void) const;
    void SetDieState(const std::string &state);

    const std::string &GetWinState(void) const;
    void SetWinState(const std::string &state);

    void SetWonFlag(bool flag);

private:
    std::string m_idleState;

    std::string m_runState;
    std::string m_runLeftState;
    std::string m_runRightState;

    std::string m_jumpInState;
    std::string m_jumpLoopState;
    std::string m_jumpOutState;

    std::string m_dieState;

    std::string m_winState;

    AnimatorControllerStateMachine::Handle m_controller;

    bool m_connected;

    ursine::ecs::SweptController *m_sweptController;
    CharacterController *m_characterController;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    void connectToEvents(bool toggle);

    void onJump(EVENT_HANDLER(ursine::ecs::Entity));
    void onMove(EVENT_HANDLER(ursine::ecs::Entity));
    void onDeath(EVENT_HANDLER(Health));
    void onRevive(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(
    Enable,
) EditorMeta(
    RequiresComponents(
        typeof( ursine::ecs::Animator )
    )
);
