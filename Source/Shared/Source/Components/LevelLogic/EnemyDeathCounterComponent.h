#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EnemyDeathCounterComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>


struct EnemyDeathCounter : ursine::ecs::Component
{
    NATIVE_COMPONENT
public:

    EnemyDeathCounter(void);
    ~EnemyDeathCounter(void);

    void OnInitialize(void) override;

    void OnEnemyDeath(EVENT_HANDLER(game::ENEMY_DEATH));

    int m_numToListenFor;

private:

    int m_numOfDeaths;

} Meta(Enable, DisplayName("EnemyDeathCounter"));

