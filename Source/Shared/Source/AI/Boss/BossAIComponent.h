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

#include "BossAIStateMachine.h"
#include "LevelSegmentManagerComponent.h"

class Health;

class BossAI : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        std::string seedshotEntity,
        GetSeedshotEntityName,
        SetSeedshotEntityName
    );

    EditorField(
        std::string vineArchetype,
        GetVineArchetype,
        SetVineArchetype
    );

    BossAI(void);

    const std::string &GetSeedshotEntityName(void) const;
    void SetSeedshotEntityName(const std::string &entityName);

    const std::string &GetVineArchetype(void) const;
    void SetVineArchetype(const std::string &vineArchetype);

    ursine::ecs::EntityHandle GetSeedshotEntity(void);

    void AddSpawnedVine(ursine::ecs::EntityHandle vine);

private:

    void OnInitialize(void) override;
    void onHierachyConstructed(EVENT_HANDLER(ursine::ecs::Entity));

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    void onLevelSegmentChanged(EVENT_HANDLER(LevelSegmentManager));

    void onVineDeath(EVENT_HANDLER(Health));

    std::string m_seedshotEntity;

    std::string m_vineArchetype;

    LevelSegments m_segment;

    // The number of vines alive
    int m_vineCount;

    typedef std::vector<BossAIStateMachine::Handle> StateMachines;
    StateMachines m_bossLogic[5];

} Meta(Enable);
