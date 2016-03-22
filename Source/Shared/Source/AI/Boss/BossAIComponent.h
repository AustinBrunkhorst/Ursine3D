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

    EditorMeta(InputRange(0.0f, 720.0f, 0.5f, "{{value.toFixed( 2 )}} deg/sec"))
    EditorField(
        float seedshotTurnSpeed,
        GetSeedshotTurnSpeed,
        SetSeedshotTurnSpeed
    );

    EditorField(
        std::string seedshotEntity,
        GetSeedshotEntityName,
        SetSeedshotEntityName
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        vineArchetype,
        GetVineArchetype,
        SetVineArchetype
    );

    BossAI(void);

    float GetSeedshotTurnSpeed(void) const;
    void SetSeedshotTurnSpeed(float turnSpeed);

    const std::string &GetSeedshotEntityName(void) const;
    void SetSeedshotEntityName(const std::string &entityName);

    const ursine::resources::ResourceReference &GetVineArchetype(void) const;
    void SetVineArchetype(const ursine::resources::ResourceReference &vineArchetype);

    ursine::ecs::EntityHandle GetSeedshotEntity(void);

    void AddSpawnedVine(ursine::ecs::EntityHandle vine);

private:

    void OnInitialize(void) override;
    void onHierachyConstructed(EVENT_HANDLER(ursine::ecs::Entity));

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    void onLevelSegmentChanged(EVENT_HANDLER(LevelSegmentManager));

    void onVineDeath(EVENT_HANDLER(Health));

    void updateVineCount(void);

    float m_turnSpeed;

    std::string m_seedshotEntity;

    ursine::resources::ResourceReference m_vineArchetype;

    LevelSegments m_segment;

    // The number of vines alive
    int m_vineCount;

    typedef std::vector<BossAIStateMachine::Handle> StateMachines;
    StateMachines m_bossLogic[5];

} Meta(Enable);
