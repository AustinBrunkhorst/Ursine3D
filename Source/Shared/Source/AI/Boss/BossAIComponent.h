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

class Health;

class BossAI : public ursine::ecs::Component
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

    Meta(InputRange(0.0f, 90.0f, 0.1f, "{{value.toFixed( 2 )}} deg"))
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

    EditorField(
        float pollinateGravity,
        GetPollinateGravity,
        SetPollinateGravity
    );

    EditorField(
        float pollinateSpreadDistance,
        GetPollinateSpreadDistance,
        SetPollinateSpreadDistance
    );

    EditorField(
        float pollinateSpreadTime,
        GetPollinateSpreadTime,
        SetPollinateSpreadTime
    );

    EditorField(
        float pollinateProjectileLifeTime,
        GetPollinateProjectileLifeTime,
        SetPollinateProjectileLifeTime
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        pollinateArchetype,
        GetPollinateArchetype,
        SetPollinateArchetype
    );

    EditorField(
        std::string invulnerableEmitterEntity,
        GetInvulnerableEmitterEntityName,
        SetInvulnerableEmitterEntityName
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        vineArchetype,
        GetVineArchetype,
        SetVineArchetype
    );

    BossAI(void);

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

    float GetPollinateGravity(void) const;
    void SetPollinateGravity(float gravity);

    float GetPollinateSpreadDistance(void) const;
    void SetPollinateSpreadDistance(float distance);

    float GetPollinateSpreadTime(void) const;
    void SetPollinateSpreadTime(float time);

    float GetPollinateProjectileLifeTime(void) const;
    void SetPollinateProjectileLifeTime(float lifetime);

    const ursine::resources::ResourceReference &GetPollinateArchetype(void) const;
    void SetPollinateArchetype(const ursine::resources::ResourceReference &pollinateArchetype);

    const std::string &GetInvulnerableEmitterEntityName(void) const;
    void SetInvulnerableEmitterEntityName(const std::string &entityName);

    const ursine::resources::ResourceReference &GetVineArchetype(void) const;
    void SetVineArchetype(const ursine::resources::ResourceReference &vineArchetype);

    ursine::ecs::EntityHandle GetSeedshotEntity(void);

    ursine::ecs::EntityHandle GetSludgeshotEntity(void);

    ursine::ecs::EntityHandle GetPollinateEntity(void);

    ursine::ecs::EntityHandle GetInvulnerableEmitterEntity(void);

    void AddSpawnedVine(ursine::ecs::EntityHandle vine);

private:

    void OnInitialize(void) override;
    void onHierachyConstructed(EVENT_HANDLER(ursine::ecs::Entity));

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    void onLevelSegmentChanged(EVENT_HANDLER(LevelSegmentManager));

    void onVineDeath(EVENT_HANDLER(Health));

    void updateVineCount(void);

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

    ursine::SVec3 m_pollinateLocalForward;

    float m_maxPollinateSpreadAngle;

    int m_pollinateProjectileCount;

    float m_pollinateGravity;

    float m_pollinateSpreadDistance;

    float m_pollinateSpreadTime;

    float m_pollinateProjectileLifeTime;

    ursine::resources::ResourceReference m_pollinateArchetype;

    // Invulnerable emitter
    std::string m_invulnerableEmitterEntity;

    // Vine variables
    ursine::resources::ResourceReference m_vineArchetype;

    // The number of vines alive
    int m_vineCount;

    // Logic variables
    LevelSegments m_segment;

    typedef std::vector<BossAIStateMachine::Handle> StateMachines;
    StateMachines m_bossLogic[5];

} Meta(Enable);
