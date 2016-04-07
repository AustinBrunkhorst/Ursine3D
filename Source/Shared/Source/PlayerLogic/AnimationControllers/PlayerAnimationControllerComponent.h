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
        std::string jumpState,
        GetJumpState,
        SetJumpState
    );

    EditorField(
        std::string jumpCycleState,
        GetJumpCycleState,
        SetJumpCycleState
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

    const std::string &GetJumpState(void) const;
    void SetJumpState(const std::string &state);

    const std::string &GetJumpCycleState(void) const;
    void SetJumpCycleState(const std::string &state);

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

    std::string m_jumpState;
    std::string m_jumpCycleState;

    std::string m_dieState;

    std::string m_winState;

    AnimatorControllerStateMachine::Handle m_controller;

    bool m_connected;

    ursine::ecs::SweptController *m_sweptController;

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
