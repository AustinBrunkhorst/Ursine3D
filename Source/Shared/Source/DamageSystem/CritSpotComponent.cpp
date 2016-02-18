/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CritSpotComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "CritSpotComponent.h"
#include "DamageOnCollideComponent.h"
#include <EntityEvent.h>
#include <CollisionEventArgs.h>
#include <GameEvents.h>

NATIVE_COMPONENT_DEFINITION( CritSpot ) ;


CritSpot::CritSpot(void) :
    BaseComponent( )
{
}

CritSpot::~CritSpot(void)
{
    GetOwner( )->Listener(this)
        .Off(ursine::ecs::ENTITY_COLLISION_PERSISTED, &CritSpot::OnCollide);
}

void CritSpot::OnInitialize(void)
{
    GetOwner( )->Listener(this)
        .Off(ursine::ecs::ENTITY_COLLISION_PERSISTED, &CritSpot::OnCollide);
}

void CritSpot::OnCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED))
{
    EVENT_ATTRS(ursine::ecs::Entity, ursine::physics::CollisionEventArgs);

    DamageOnCollide* damageComp = args->otherEntity->GetComponent<DamageOnCollide>( );

    if ( damageComp )
    {
        // calculate damage with crit and if cr
        float damage = damageComp->GetDamageToApply( );
        damage *= damageComp->GetCritModifier( );

        game::DamageEventArgs damageArgs(damage, damageComp);

        // dispatch event for health comp
        GetOwner( )->Dispatch(game::DAMAGE_EVENT, &damageArgs);

        damageComp->AddEntityToIntervals( GetOwner( )->GetUniqueID( ) );
    }

}