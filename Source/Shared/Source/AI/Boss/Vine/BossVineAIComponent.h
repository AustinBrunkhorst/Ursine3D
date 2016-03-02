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

#include "EntityAnimatorComponent.h"

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
        float whipTurnSpeed,
        GetWhipTurnSpeed,
        SetWhipTurnSpeed
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
        float whipCooldown,
        GetWhipCooldown,
        SetWhipCooldown
    );

    EditorField(
        float digSpeed,
        GetDigSpeed,
        SetDigSpeed
    );

    EditorMeta(InputRange(0.0f, 720.0f, 0.5f, "{{value.toFixed( 2 )}} deg/sec"))
    EditorField(
        float digTurnSpeed,
        GetDigTurnSpeed,
        SetDigTurnSpeed
    );

    EditorField(
        std::string digParticleEmitterName,
        GetDigParticleEmitterName,
        SetDigParticleEmitterName
    );

    EditorField(
        float uprootDistance,
        GetUprootDistance,
        SetUprootDistance
    );

    EditorField(
        float uprootDelay,
        GetUprootDelay,
        SetUprootDelay
    );

    EditorField(
        ursine::SVec3 colliderSize,
        GetColliderSize,
        SetColliderSize
    );
    
    EditorMeta(Annotation("List of ROOT entities we're colliding with (used for uproot)."))
    ursine::Array<std::string> collisionList;

    EditorMeta(Annotation("List of entities we're ignoring collision with (used for uproot)."))
    ursine::Array<std::string> ignoreList;

    BossVineAI(void);
    ~BossVineAI(void);

    bool GetFaceClosestPlayer(void) const;
    void SetFaceClosestPlayer(bool flag);

    float GetWhipTurnSpeed(void) const;
    void SetWhipTurnSpeed(float turnSpeed);

    float GetWhipAngle(void) const;
    void SetWhipAngle(float angle);

    float GetWhipRange(void) const;
    void SetWhipRange(float range);

    float GetWhipDuration(void) const;
    void SetWhipDuration(float duration);

    float GetWhipCooldown(void) const;
    void SetWhipCooldown(float cooldown);

    float GetDigSpeed(void) const;
    void SetDigSpeed(float digSpeed);

    float GetDigTurnSpeed(void) const;
    void SetDigTurnSpeed(float turnSpeed);

    const std::string &GetDigParticleEmitterName(void) const;
    void SetDigParticleEmitterName(const std::string &name);

    float GetUprootDistance(void) const;
    void SetUprootDistance(float distance);

    float GetUprootDelay(void) const;
    void SetUprootDelay(float delay);

    const ursine::SVec3 &GetColliderSize(void) const;
    void SetColliderSize(const ursine::SVec3 &colliderSize);

    EntityAnimator *GetAnimator(void);

    ursine::ecs::Entity *GetTarget(void);
    void SetTarget(ursine::ecs::Entity *target);

private:

    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(World));

    void onChildrenSerialized(EVENT_HANDLER(ursine::ecs::Entity));

    bool m_faceClosestPlayer;

    float m_whipTurnSpeed;
    float m_whipAngle;
    float m_whipRange;
    float m_whipDuration;
    float m_whipCooldown;

    float m_digSpeed;
    float m_digTurnSpeed;
    float m_uprootDistance;
    float m_uprootDelay;
    ursine::SVec3 m_colliderSize;

    std::string m_digParticleEmitterName;

    BossVineStateMachine m_stateMachine;

    EntityAnimator *m_animator;

    ursine::ecs::Entity *m_target;

} Meta(Enable);
