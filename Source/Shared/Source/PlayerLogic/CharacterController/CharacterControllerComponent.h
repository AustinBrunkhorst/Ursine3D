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

namespace ursine
{
    namespace ecs
    {
        class SweptController;
    }
}

class CharacterController : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    CharacterController(void);
    ~CharacterController(void);

    EditorMeta(InputRange(0.0f, 15.0f, 0.01f))
    EditorField(
        float rotateSpeed,
        GetRotateSpeed,
        SetRotateSpeed
    );

    EditorMeta(InputRange(0.0f, 1.0f, 0.01f))
    EditorField(
        float deadZone,
        GetDeadZone,
        SetDeadZone
    );

    EditorMeta(InputRange(0.0f, 1.0f, 0.01f))
    EditorField(
        float deadZoneSnap,
        GetDeadZoneSnap,
        SetDeadZoneSnap
    );

    EditorMeta(InputRange(0.0f, 1.0f, 0.05f))
    EditorField(
        float MediumRotationFloor,
        GetMediumRotationFloor,
        SetMediumRotationFloor
    );

    EditorMeta(InputRange(0.0f, 1.0f, 0.05f))
    EditorField(
        float HighRotationFloor,
        GetHighRotationFloor,
        SetHighRotationFloor
    );

    EditorMeta(InputRange(0.0f, 1.0f, 0.05f))
    EditorField(
        float LowRotationFactor,
        GetLowRotationFactor,
        SetLowRotationFactor
    );

    EditorMeta(InputRange(0.0f, 1.0f, 0.05f))
    EditorField(
        float MediumRotationFactor,
        GetMediumRotationFactor,
        SetMediumRotationFactor
    );

    EditorField(
        bool lockMovement,
        GetLockMovement,
        SetLockMovement
    );

    EditorField(
        bool lockLooking,
        GetLockLooking,
        SetLockLooking
    );

    EditorMeta(InputRange(0.0f, 1.0f, 0.1f, "{{(value * 100.0).toFixed( 2 )}} %"))
    EditorField(
        float jumpDirectionScalar,
        GetJumpDirectionScalar,
        SetJumpDirectionScalar
    );

    float GetRotateSpeed(void) const;
    void SetRotateSpeed(float rotateSpeed);

    float GetRotationModifier(void) const;
    void SetRotationModifier(float modifier);

    float GetDeadZone(void) const;
    void SetDeadZone(float deadZone);

    float GetDeadZoneSnap(void) const;
    void SetDeadZoneSnap(float snap);

    float GetMediumRotationFloor(void) const;
    void SetMediumRotationFloor(float floor);

    float GetHighRotationFloor(void) const;
    void SetHighRotationFloor(float floor);

    float GetLowRotationFactor(void) const;
    void SetLowRotationFactor(float rotationFactor);

    float GetMediumRotationFactor(void) const;
    void SetMediumRotationFactor(float rotationFactor);

    const ursine::Vec2 &GetMoveDirection(void) const;
    void SetMoveDirection(const ursine::Vec2 &moveDir);

    const ursine::Vec2 &GetLookDirection(void) const;
    void SetLookDirection(const ursine::Vec2 &lookDir);

    bool GetLockMovement(void) const;
    void SetLockMovement(bool flag);

    bool GetLockLooking(void) const;
    void SetLockLooking(bool flag);

    float GetJumpDirectionScalar(void) const;
    void SetJumpDirectionScalar(float scalar);

    void OnInitialize(void) override;

private:
    friend class CharacterControllerSystem;

    void SetMoveDirection(EVENT_HANDLER(game::MOVEMENT_COMMAND));
    void SetLookDirection(EVENT_HANDLER(game::LOOK_COMMAND));
    void Jump(EVENT_HANDLER(game::JUMP_COMMAND));

    float m_rotateSpeed;
    
    float m_rotationModifier;

    float m_deadZone;

    float m_deadZoneSnap;

    float m_mediumRotationFloor;
    float m_highRotationFloor;
    float m_lowRotationFactor;
    float m_mediumRotationFactor;

    float m_jumpDirectionScalar;

    bool m_jump;

    bool m_lockMovement;
    bool m_lockLooking;

    ursine::Vec2 m_moveDir;
    ursine::Vec2 m_lookDir;

} Meta(
    Enable, 
    DisplayName( "CharacterController" ), 
) EditorMeta(
    RequiresComponents( typeof( ursine::ecs::SweptController ) )
);
