#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EnemyDeathComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include <GameEvents.h>

struct EnemyDeathComp : ursine::ecs::Component
{
    NATIVE_COMPONENT
public:

    EnemyDeathComp(void);
    ~EnemyDeathComp(void);

    void OnInitialize(void) override;
    void OnDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED));


} Meta(Enable, DisplayName("EnemyDeathComp"));
