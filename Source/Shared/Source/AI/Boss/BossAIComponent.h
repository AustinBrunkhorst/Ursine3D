/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossAIComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <ArchetypeData.h>

#include "BossAIStateMachine.h"
#include "LevelSegmentManagerComponent.h"
#include "HealthComponent.h"
#include "VineAIComponent.h"

#include <AudioItemEventData.h>
#include <AudioEmitterComponent.h>

enum class BossAIEvents
{
    ShieldUp,
    ShieldDown
};

class Health;

class BossAI 
    : public ursine::ecs::Component
    , public ursine::EventDispatcher<BossAIEvents>
{
    NATIVE_COMPONENT;

public:
    EditorButton(
        visualizePollinateSpread,
        "Visualize Pollinate Spread"
    );

    EditorField(
        std::string seedshotEntity,
        GetSeedshotEntityName,
        SetSeedshotEntityName
    );

    EditorMeta(InputRange(0.0f, 720.0f, 0.5f, "{{value.toFixed( 2 )}} deg/sec"))
    EditorField(
        float seedshotTurnSpeed,
        GetSeedshotTurnSpeed,
        SetSeedshotTurnSpeed
    );

    EditorField(
        float seedshotInterval,
        GetSeedshotInterval,
        SetSeedshotInterval
    );

    EditorField(
        float seedshotCooldown,
        GetSeedshotCooldown,
        SetSeedshotCooldown
    );

    EditorField(
        std::string sludgeshotEntity,
        GetSludgeshotEntityName,
        SetSludgeshotEntityName
    );

    EditorField(
        float sludgeshotAnimationTime,
        GetSludgeshotAnimationTime,
        SetSludgeshotAnimationTime
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        sludgeshotArchetype,
        GetSludgeshotArchetype,
        SetSludgeshotArchetype
    );

    EditorField(
        std::string pollinateEntity,
        GetPollinateEntityName,
        SetPollinateEntityName
    );

    EditorField(
        ursine::SVec3 pollinateLocalForward,
        GetPollinateLocalForward,
        SetPollinateLocalForward
    );

    EditorMeta(InputRange(0.0f, 90.0f, 0.1f, "{{value.toFixed( 2 )}} deg"))
    EditorField(
        float maxPollinateSpreadAngle,
        GetMaxPollinateSpreadAngle,
        SetMaxPollinateSpreadAngle
    );

    EditorField(
        int pollinateProjectileCount,
        GetPollinateProjectileCount,
        SetPollinateprojectileCount
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        pollinateArchetype,
        GetPollinateArchetype,
        SetPollinateArchetype
    );

    EditorField(
        float pollinateTickRate,
        GetPollinateTickRate,
        SetPollinateTickRate
    );

    EditorField(
        float pollinateTickAmount,
        GetPollinateTickAmount,
        SetPollinateTickAmount
    );

    EditorField(
        std::string pollinateSmogEntityName,
        GetPollinateSmogEntityName,
        SetPollinateSmogEntityName
    );

    EditorField(
        std::string pollinateGooFloorEntityName,
        GetPollinateGooFloorEntityName,
        SetPollinateGooFloorEntityName
    );

    EditorField(
        std::string pollinateSafetyShieldEntityName,
        GetPollinateSafetyShieldEntityName,
        SetPollinateSafetyShieldEntityName
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        vineArchetype,
        GetVineArchetype,
        SetVineArchetype
    );

    EditorMeta(InputRange(0.0f, 100.0f, 0.1f, "{{value.toFixed( 2 )}} %"))
    EditorField(
        float phase1HealthTransitionThreshold,
        GetPhase1HealthTransitionThreshold,
        SetPhase1HealthTransitionThreshold
    );

    EditorField(
        float phase1DazedResetTimer,
        GetPhase1DazedResetTimer,
        SetPhase1DazedResetTimer
    );

    EditorMeta(InputRange(0.0f, 100.0f, 0.1f, "{{value.toFixed( 2 )}} %"))
    EditorField(
        float phase2HealthTransitionThreshold,
        GetPhase2HealthTransitionThreshold,
        SetPhase2HealthTransitionThreshold
    );

    EditorField(
        float phase2DazedResetTimer,
        GetPhase2DazedResetTimer,
        SetPhase2DazedResetTimer
    );

    EditorMeta(InputRange(0.0f, 100.0f, 0.1f, "{{value.toFixed( 2 )}} %"))
    EditorField(
        float phase3HealthTransitionThreshold,
        GetPhase3HealthTransitionThreshold,
        SetPhase3HealthTransitionThreshold
    );

    EditorField(
        int phase3NumSludgeshots,
        GetPhase3NumSludgeshots,
        SetPhase3NumSludgeshots
    );

    EditorField(
        float phase3SludgeshotAnimScalar,
        GetPhase3SludgeshotAnimScalar,
        SetPhase3SludgeshotAnimScalar
    );

    EditorField(
        int phase4NumSludgeshots,
        GetPhase4NumSludgeshots,
        SetPhase4NumSludgeshots
    );

    EditorField(
        float phase4SeedshotDuration,
        GetPhase4SeedshotDuration,
        SetPhase4SeedshotDuration
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        introScream,
        GetIntroScream,
        SetIntroScream
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        stageScream,
        GetStageScream,
        SetStageScream
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        deathScream,
        GetDeathScream,
        SetDeathScream
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        hurtSfx,
        GetHurtSfx,
        SetHurtSfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        shieldBreakSfx,
        GetShieldBreakSfx,
        SetShieldBreakSfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        shieldRebuildSfx,
        GetShieldRebuildSfx,
        SetShieldRebuildSfx
    );

    Meta(Enable)
    BossAI(void);
    ~BossAI(void);

    const std::string &GetSeedshotEntityName(void) const;
    void SetSeedshotEntityName(const std::string &entityName);

    float GetSeedshotTurnSpeed(void) const;
    void SetSeedshotTurnSpeed(float turnSpeed);

    float GetSeedshotInterval(void) const;
    void SetSeedshotInterval(float interval);

    float GetSeedshotCooldown(void) const;
    void SetSeedshotCooldown(float cooldown);

    const std::string &GetSludgeshotEntityName(void) const;
    void SetSludgeshotEntityName(const std::string &entityName);

    float GetSludgeshotAnimationTime(void) const;
    void SetSludgeshotAnimationTime(float time);

    const ursine::resources::ResourceReference &GetSludgeshotArchetype(void) const;
    void SetSludgeshotArchetype(const ursine::resources::ResourceReference &sludgeArchetype);

    const std::string &GetPollinateEntityName(void) const;
    void SetPollinateEntityName(const std::string &entityname);

    const ursine::SVec3 &GetPollinateLocalForward(void) const;
    void SetPollinateLocalForward(const ursine::SVec3 &localForward);

    float GetMaxPollinateSpreadAngle(void) const;
    void SetMaxPollinateSpreadAngle(float angle);

    int GetPollinateProjectileCount(void) const;
    void SetPollinateprojectileCount(int count);

    float GetPollinateTickRate(void) const;
    void SetPollinateTickRate(float rate);

    float GetPollinateTickAmount(void) const;
    void SetPollinateTickAmount(float damage);

    const std::string &GetPollinateSmogEntityName(void) const;
    void SetPollinateSmogEntityName(const std::string &name);

    const std::string &GetPollinateGooFloorEntityName(void) const;
    void SetPollinateGooFloorEntityName(const std::string &name);

    const ursine::resources::ResourceReference &GetPollinateArchetype(void) const;
    void SetPollinateArchetype(const ursine::resources::ResourceReference &pollinateArchetype);

    const ursine::resources::ResourceReference &GetVineArchetype(void) const;
    void SetVineArchetype(const ursine::resources::ResourceReference &vineArchetype);

    float GetPhase1HealthTransitionThreshold(void) const;
    void SetPhase1HealthTransitionThreshold(float threshold);

    float GetPhase1DazedResetTimer(void) const;
    void SetPhase1DazedResetTimer(float timer);

    float GetPhase2HealthTransitionThreshold(void) const;
    void SetPhase2HealthTransitionThreshold(float threshold);

    float GetPhase2DazedResetTimer(void) const;
    void SetPhase2DazedResetTimer(float timer);

    float GetPhase3HealthTransitionThreshold(void) const;
    void SetPhase3HealthTransitionThreshold(float threshold);

    int GetPhase3NumSludgeshots(void) const;
    void SetPhase3NumSludgeshots(int num);

    float GetPhase3SludgeshotAnimScalar(void) const;
    void SetPhase3SludgeshotAnimScalar(float scalar);

    int GetPhase4NumSludgeshots(void) const;
    void SetPhase4NumSludgeshots(int num);

    float GetPhase4SeedshotDuration(void) const;
    void SetPhase4SeedshotDuration(float duration);

    const std::string &GetPollinateSafetyShieldEntityName(void) const;
    void SetPollinateSafetyShieldEntityName(const std::string &name);

    const ursine::resources::ResourceReference &GetIntroScream(void) const;
    void SetIntroScream(const ursine::resources::ResourceReference &audioEvent);

    const ursine::resources::ResourceReference &GetStageScream(void) const;
    void SetStageScream(const ursine::resources::ResourceReference &audioEvent);

    const ursine::resources::ResourceReference &GetDeathScream(void) const;
    void SetDeathScream(const ursine::resources::ResourceReference &audioEvent);

    const ursine::resources::ResourceReference &GetHurtSfx(void) const;
    void SetHurtSfx(const ursine::resources::ResourceReference &hurtSfx);

    const ursine::resources::ResourceReference &GetShieldBreakSfx(void) const;
    void SetShieldBreakSfx(const ursine::resources::ResourceReference &shieldBreakSfx);

    const ursine::resources::ResourceReference &GetShieldRebuildSfx(void) const;
    void SetShieldRebuildSfx(const ursine::resources::ResourceReference &shieldRebuildSfx);

    ursine::ecs::EntityHandle GetSeedshotEntity(void);

    ursine::ecs::EntityHandle GetSludgeshotEntity(void);

    ursine::ecs::EntityHandle GetPollinateEntity(void);

    ursine::ecs::EntityHandle GetPollinateSmogEntity(void);

    ursine::ecs::EntityHandle GetPollinateSafetyShield(void);

    ursine::ecs::EntityHandle GetPollinateGooFloor(void);

    void AddSpawnedVine(ursine::ecs::EntityHandle vine);
    const std::vector<ursine::ecs::EntityHandle> &GetVines(void) const;

    void SetVineHealthThresholdCallback(const std::function<void(VineAI*)> &callback);

    void SetHomeLocation(const ursine::SVec3 &homeLocation);
    const ursine::SVec3 &GetHomeLocation(void) const;

    void JumpToHomeLocation(void);

    void SetSpawnOrientation(const ursine::SQuat &orientation);
    const ursine::SQuat &GetSpawnOrientation(void) const;

    bool IsUnderground(void) const;
    void SetUnderground(bool flag);

private:

    void OnInitialize(void) override;
    void onHierachyConstructed(EVENT_HANDLER(ursine::ecs::Entity));

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    void onDamageTaken(EVENT_HANDLER(Health));

    void onLevelSegmentChanged(EVENT_HANDLER(LevelSegmentManager));

    void onVineDeath(EVENT_HANDLER(Health));

    void onVineHealthThresholdReached(EVENT_HANDLER(VineAI));

    void updateHealth(void);

    void updateVineCount(void);

    void setJumpToHomeLocationBools(void);

    // Seedshot variables
    float m_turnSpeed;

    float m_seedshotInterval;

    float m_seedshotCooldown;

    std::string m_seedshotEntity;

    // Sludgeshot variables
    float m_sludgeshotAnimationTime;

    std::string m_sludgeshotEntity;

    ursine::resources::ResourceReference m_sludgeshotArchetype;

    // Pollinate variables
    std::string m_pollinateEntity;

    std::string m_pollinateGooFloor;

    ursine::SVec3 m_pollinateLocalForward;

    float m_maxPollinateSpreadAngle;

    int m_pollinateProjectileCount;

    ursine::resources::ResourceReference m_pollinateArchetype;

    float m_pollinateTickRate;

    float m_pollinateTickAmount;

    std::string m_pollinateSmogEntity;

    // Invulnerable emitter
    std::string m_invulnerableEmitterEntity;

    // Vine variables
    ursine::resources::ResourceReference m_vineArchetype;

    // The number of vines alive
    int m_vineCount;

    // Vector of all vines
    std::vector<ursine::ecs::EntityHandle> m_vines;

    // This callback is called when a vine health threshold is reached
    std::function<void(VineAI*)> m_vineHealthThresholdCallback;

    // The threshold we're using for the phase 1 transition (1-100%)
    float m_phase1HealthThreshold;

    // The timer we use for resetting the boss for phase 1
    float m_phase1DazedResetTimer;

    // The threshold we're using for the phase 2 transition (1-100%)
    float m_phase2HealthThreshold;

    // The timer we use for resetting the boss for phase 2
    float m_phase2DazedResetTimer;

    // The threshold we're using for the phase 3 transition (1-100%)
    float m_phase3HealthThreshold;

    int m_phase3NumSludgeshots;

    float m_phase3SludgeshotAnimScalar;

    int m_phase4NumSludgeshots;

    float m_phase4SeedshotDuration;

    std::string m_pollinateSafetyShield;

    // Logic variables
    LevelSegments m_segment;

    typedef std::vector<BossAIStateMachine::Handle> StateMachines;
    StateMachines m_bossLogic[5];

    ursine::SVec3 m_homeLocation;
    ursine::SQuat m_spawnOrientation;

    bool m_underground;

    // Sfxs
    ursine::resources::ResourceReference m_introScream;
    ursine::resources::ResourceReference m_stageScream;
    ursine::resources::ResourceReference m_deathScream;
    ursine::resources::ResourceReference m_hurtSfx;
    ursine::resources::ResourceReference m_shieldBreakSfx;
    ursine::resources::ResourceReference m_shieldRebuildSfx;

} Meta(
    Enable,
    WhiteListMethods
) EditorMeta(
    RequiresComponents( 
        typeof( Health ),
        typeof( ursine::ecs::AudioEmitter )
    )
);
