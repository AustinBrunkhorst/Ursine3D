/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerGunAnimationControllerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <AnimatorComponent.h>

#include "HitscanWeaponComponent.h"
#include "AnimatorControllerStateMachine.h"
#include "AnimatorControllerState.h"
#include "PlayAnimationState.h"

class PlayerGunAnimationController : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        std::string idleState,
        GetIdleState,
        SetIdleState
    );

    EditorField(
        std::string jumpState,
        GetJumpState,
        SetJumpState
    );

    EditorField(
        std::string runState,
        GetRunState,
        SetRunState
    );

    EditorField(
        std::string shootBeginState,
        GetShootBeginState,
        SetShootBeginState
    );

    EditorField(
        std::string shootLoopState,
        GetShootLoopState,
        SetShootLoopState
    );

    EditorField(
        std::string shootEndState,
        GetShootEndState,
        SetShootEndState
    );

    EditorField(
        std::string reloadClip,
        GetReloadClip,
        SetReloadClip
    );

    PlayerGunAnimationController(void);
    ~PlayerGunAnimationController(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const std::string &GetIdleState(void) const;
    void SetIdleState(const std::string &state);

    const std::string &GetJumpState(void) const;
    void SetJumpState(const std::string &state);

    const std::string &GetRunState(void) const;
    void SetRunState(const std::string &state);

    const std::string &GetShootBeginState(void) const;
    void SetShootBeginState(const std::string &state);

    const std::string &GetShootLoopState(void) const;
    void SetShootLoopState(const std::string &state);

    const std::string &GetShootEndState(void) const;
    void SetShootEndState(const std::string &state);

    const std::string &GetReloadClip(void) const;
    void SetReloadClip(const std::string &clip);

private:
    std::string m_idleState;
    std::string m_jumpState;
    std::string m_runState;

    std::string m_shootBeginState;
    std::string m_shootLoopState;
    std::string m_shootEndState;

    std::string m_reloadClip;
    float m_reloadTime;

    AnimatorControllerStateMachine::Handle m_controller;

    bool m_connected;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    void connectToEvents(bool toggle);

    void onJump(EVENT_HANDLER(ursine::ecs::Entity));
    void onMove(EVENT_HANDLER(ursine::ecs::Entity));

    void onStartShoot(EVENT_HANDLER(ursine::ecs::Entity));
    void onEndShoot(EVENT_HANDLER(ursine::ecs::Entity));

    void onStartReload(EVENT_HANDLER(ursine::ecs::Entity));
    void onEndReload(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(
    Enable,
) EditorMeta(
    RequiresComponents(
        typeof( ursine::ecs::Animator )
    )    
);
