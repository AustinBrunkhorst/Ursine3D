/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TeleportPlayerToSpawnOnHitComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "TeleportPlayerToSpawnOnHitComponent.h"

#include "PlayerIDComponent.h"
#include "PlayerSpawnPointComponent.h"
#include "LevelSegmentManagerComponent.h"

#include <EntityEvent.h>
#include <CollisionEventArgs.h>

NATIVE_COMPONENT_DEFINITION( TeleportPlayerToSpawnOnHit );

using namespace ursine;
using namespace ecs;

TeleportPlayerToSpawnOnHit::TeleportPlayerToSpawnOnHit(void)
    : BaseComponent( ) { }

TeleportPlayerToSpawnOnHit::~TeleportPlayerToSpawnOnHit(void)
{
    GetOwner( )->Listener( this )
        .Off( ENTITY_COLLISION_PERSISTED, &TeleportPlayerToSpawnOnHit::onCollide );
}

void TeleportPlayerToSpawnOnHit::OnSceneReady(Scene *scene)
{
    GetOwner( )->Listener( this )
        .On( ENTITY_COLLISION_PERSISTED, &TeleportPlayerToSpawnOnHit::onCollide );
}

void TeleportPlayerToSpawnOnHit::onCollide(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, physics::CollisionEventArgs);

    if (args->otherEntity->HasComponent<PlayerID>( ))
    {
        auto world = GetOwner( )->GetWorld( );
        auto sm = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );
        auto spawns = world->GetEntitiesFromFilter( Filter( ).All<PlayerSpawnPoint>( ) );

        if (!sm.size( ))
            return;

        auto segment = sm[ 0 ]->GetComponent<LevelSegmentManager>( )->GetCurrentSegment( );
        auto spawnP = spawns[ 0 ];
        auto spawnSeg = spawnP->GetComponent<PlayerSpawnPoint>( )->GetSpawnSegment( );

        for (auto &spawn : spawns)
        {
            auto newSeg = spawn->GetComponent<PlayerSpawnPoint>( )->GetSpawnSegment( );
            auto dif = segment - newSeg;

            if (dif >= 0 && dif < segment - spawnSeg)
            {
                spawnSeg = newSeg;
                spawnP = spawn;
            }
        }

        auto otherTrans = args->otherEntity->GetTransform( );
        auto spawnTrans = spawnP->GetTransform( );

        otherTrans->SetWorldPosition( spawnTrans->GetWorldPosition( ) );
        otherTrans->SetWorldRotation( spawnTrans->GetWorldRotation( ) );
    }
}
