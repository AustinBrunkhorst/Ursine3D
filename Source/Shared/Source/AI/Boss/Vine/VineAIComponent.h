/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** VineAIComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "VineAIStateMachine.h"

#include "HealthComponent.h"

#include <AnimatorComponent.h>
#include <EventDispatcher.h>

enum VineAIEvents
{
    VINE_HEALTH_THRESHOLD_REACHED
};

struct HealthThreshold
{
    EditorMeta(InputRange(0.0f, 1.0f, 0.01f, "{{(value * 100.0).toFixed( 2 )}} %"))
    float percentage;

    HealthThreshold(void)
        : percentage( 1.0f ) { }

} Meta(Enable, EnableArrayType);

class VineAI 
    : public ursine::ecs::Component
    , public ursine::EventDispatcher<VineAIEvents>
{
    NATIVE_COMPONENT;

public:
    EditorButton(
        drawRange,
        "Draw Range"
    );

    EditorButton(
        pursueTarget,
        "Pursue Target"
    );

    EditorButton(
        goHome,
        "Go Home"
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
        float uprootCooldown,
        GetUprootCooldown,
        SetUprootCooldown
    );

    ursine::Array<HealthThreshold> healthThresholds;

    EditorMeta(Annotation("List of ROOT entities we're colliding with (used for uproot)."))
    ursine::Array<std::string> collisionList;

    EditorMeta(Annotation("List of entities we're ignoring collision with (used for uproot)."))
    ursine::Array<std::string> ignoreList;

    VineAI(void);
    ~VineAI(void);

    void OnSceneReady(ursine::Scene *scene) override;

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

    float GetUprootCooldown(void) const;
    void SetUprootCooldown(float cooldown);

    ursine::ecs::Animator *GetAnimator(void);

    ursine::ecs::EntityHandle GetTarget(void);
    void SetTarget(ursine::ecs::EntityHandle target);

    void SetHomeLocation(const ursine::SVec3 &homeLocation);
    const ursine::SVec3 &GetHomeLocation(void) const;

    // Tell the vine to go back to it's home location
    void GoToHomeLocation(void);
    bool IsHome(void);

    void PursueTarget(void);

    const ursine::TimeSpan &GetTimeOfLastPursue(void) const;

private:
    friend class VineUprootState;

    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(World));

    void onChildrenSerialized(EVENT_HANDLER(ursine::ecs::Entity));

    void onDamageTaken(EVENT_HANDLER(Health));

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
    float m_uprootCooldown;

    ursine::SVec3 m_homeLocation;

    std::string m_digParticleEmitterName;

    VineAIStateMachine m_stateMachine;

    ursine::ecs::Animator *m_animator;

    ursine::ecs::EntityHandle m_target;

    ursine::TimeSpan m_timeOfLastPursue;

    // This is our current index into the health threshold array
    int m_currentHealthThreshold;

} Meta(
    Enable
) EditorMeta(
    RequiresComponents( typeof( Health ) )
);
