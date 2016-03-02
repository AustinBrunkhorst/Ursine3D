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

class BossAI : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        std::string seedshotEntity,
        GetSeedshotEntityName,
        SetSeedshotEntityName
    );

    BossAI(void);

    const std::string &GetSeedshotEntityName(void) const;
    void SetSeedshotEntityName(const std::string &entityName);

    ursine::ecs::Entity *GetSeedshotEntity(void);

private:

    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    std::string m_seedshotEntity;

    BossAIStateMachine m_stateMachine;

} Meta(Enable);
