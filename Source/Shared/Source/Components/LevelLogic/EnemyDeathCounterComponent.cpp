/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EnemyDeathCounterComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "EnemyDeathCounterComponent.h"
#include "ComponentIncludes.h"

NATIVE_COMPONENT_DEFINITION( EnemyDeathCounter ) ;

using namespace ursine;


EnemyDeathCounter::EnemyDeathCounter(void) :
    BaseComponent( ),
    m_numToListenFor( 10 ),
    m_numOfDeaths( 0 )
{
}

EnemyDeathCounter::~EnemyDeathCounter(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
         .Off( game::ENEMY_DEATH, &EnemyDeathCounter::OnEnemyDeath );
}

void EnemyDeathCounter::OnInitialize(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( game::ENEMY_DEATH, &EnemyDeathCounter::OnEnemyDeath );
}

void EnemyDeathCounter::OnEnemyDeath(EVENT_HANDLER(game::ENEMY_DEATH))
{
    ++m_numOfDeaths;

    if ( m_numOfDeaths == m_numToListenFor )
    {
        GetOwner( )->GetWorld( )->Dispatch( game::AREA_CLEAR, ursine::ecs::EntityEventArgs::Empty );
    }
}




