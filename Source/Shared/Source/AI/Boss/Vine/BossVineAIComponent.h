/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossVineAIComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "BossVineStateMachine.h"

class BossVineAI : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorButton(
        drawRange,
        "Draw Range"
    );

    EditorField(
        bool faceClosestPlayer,
        GetFaceClosestPlayer,
        SetFaceClosestPlayer
    );

    EditorMeta(InputRange(0.0f, 720.0f, 0.5f, "{{value.toFixed( 2 )}} deg/sec"))
    EditorField(
        float turnSpeed,
        GetTurnSpeed,
        SetTurnSpeed
    );

    EditorMeta(InputRange(0.0f, 360.0f, 0.5f, "{{value.toFixed( 2 )}} deg"))
    EditorField(
        float whipAngle,
        GetWhipAngle,
        SetWhipAngle
    );

    EditorField(
        float whipRange,
        GetWhipRange,
        SetWhipRange
    );

    EditorField(
        float whipDuration,
        GetWhipDuration,
        SetWhipDuration
    );

    EditorField(
        float whipDamage,
        GetWhipDamage,
        SetWhipDamage
    );

    EditorField(
        float whipCooldown,
        GetWhipCooldown,
        SetWhipCooldown
    );

    BossVineAI(void);
    ~BossVineAI(void);

    bool GetFaceClosestPlayer(void) const;
    void SetFaceClosestPlayer(bool flag);

    float GetTurnSpeed(void) const;
    void SetTurnSpeed(float speed);

    float GetWhipAngle(void) const;
    void SetWhipAngle(float angle);

    float GetWhipRange(void) const;
    void SetWhipRange(float range);

    float GetWhipDuration(void) const;
    void SetWhipDuration(float duration);

    float GetWhipDamage(void) const;
    void SetWhipDamage(float damage);

    float GetWhipCooldown(void) const;
    void SetWhipCooldown(float cooldown);

private:

    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(World));

    bool m_faceClosestPlayer;

    float m_turnSpeed;
    float m_angle;
    float m_range;
    float m_duration;
    float m_damage;
    float m_cooldown;

    BossVineStateMachine m_stateMachine;
} Meta(Enable);
