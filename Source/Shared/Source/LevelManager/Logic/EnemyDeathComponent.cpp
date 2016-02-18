/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EnemyDeathComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "EnemyDeathComponent.h"
#include "ComponentIncludes.h"

NATIVE_COMPONENT_DEFINITION( EnemyDeathComp ) ;

using namespace ursine;
using namespace ecs;

EnemyDeathComp::EnemyDeathComp(void) :
    BaseComponent( )
{
}

EnemyDeathComp::~EnemyDeathComp(void)
{
    GetOwner( )->Listener( this )
         .Off( ENTITY_REMOVED, &EnemyDeathComp::OnDeath );
}

void EnemyDeathComp::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ENTITY_REMOVED, &EnemyDeathComp::OnDeath );
}

void EnemyDeathComp::OnDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED))
{
    GetOwner( )->Dispatch( game::ENEMY_DEATH, EntityEventArgs::Empty );
}



