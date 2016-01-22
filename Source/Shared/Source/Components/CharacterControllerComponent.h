/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CharacterControllerComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <UrsineLogTools.h>

class CharacterController : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    friend class CharacterControllerSystem;

    CharacterController(void);
    ~CharacterController(void);

    EditorField(
        float moveSpeed,
        GetMoveSpeed,
        SetMoveSpeed
    );

    float GetMoveSpeed(void) const;
    void SetMoveSpeed(float moveSpeed);

    EditorField(
        float rotateSpeed,
        GetRotateSpeed,
        SetRotateSpeed
    );

    float GetRotateSpeed(void) const;
    void SetRotateSpeed(float rotateSpeed);

    EditorField(
        float jumpSpeed,
        GetJumpSpeed,
        SetJumpSpeed
    );

    float GetJumpSpeed(void) const;
    void SetJumpSpeed(float jumpSpeed);

    EditorField(
        float jumpInterval,
        GetJumpInterval,
        SetJumpInterval
    );

    float GetJumpInterval(void) const;
    void SetJumpInterval(float jumpInterval);

    const ursine::Vec2 &GetMoveDirection(void) const;
    void SetMoveDirection(const ursine::Vec2 &moveDir);

    const ursine::Vec2 &GetLookDirection(void) const;
    void SetLookDirection(const ursine::Vec2 &lookDir);

    void OnInitialize(void) override;

private:

    void onUpdate(EVENT_HANDLER(World));
    void SetLookDirection(EVENT_HANDLER(commandEvent::LOOK_COMMAND));
    void Jump(EVENT_HANDLER(commandEvent::JUMP_COMMAND));

    float m_moveSpeed;
    
    float m_rotateSpeed;
    
    float m_jumpSpeed;
    float m_jumpInterval;
    float m_jumpTimer;
    bool m_jump;

    ursine::Vec2 m_moveDir;
    ursine::Vec2 m_lookDir;

} Meta(Enable, DisplayName("CharacterController"));
