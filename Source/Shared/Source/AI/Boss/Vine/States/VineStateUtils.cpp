/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineStateUtils.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineStateUtils.h"

#include <PhysicsSystem.h>

using namespace ursine;
using namespace ecs;

float VineStateUtils::FindYPosition(VineAI *ai, const SVec3 &aiPosition)
{
    auto aiOwner = ai->GetOwner( );
    auto world = aiOwner->GetWorld( );
    auto physics = world->GetEntitySystem<PhysicsSystem>( );

    auto startPos = aiPosition;

    startPos.Y( ) = ai->GetDigRayStartY( );

    auto dir = SVec3::UnitY( ) * abs( ai->GetDigRayStartY( ) * 2.0f );

    physics::RaycastInput input( startPos, dir );
    physics::RaycastOutput output;

    if (!physics->Raycast( input, output, physics::RAYCAST_ALL_HITS, true ))
        return aiPosition.Y( );

    auto lowestY = std::numeric_limits<float>( ).max( );
    auto found = false;

    // iterate through all output and find the closest hit (that isn't the AI)
    for (int i = 0; i < output.entity.size( ); ++i)
    {
        auto hitEntity = world->GetEntity( output.entity[ i ] );

        // make sure we're not checking for ourselves, and entities
        // we don't care about
        auto collisionItr = std::find(
            ai->collisionList.begin( ), 
            ai->collisionList.end( ),
            hitEntity->GetRoot( )->GetName( ) 
        );

        auto ignoreItr = std::find(
            ai->ignoreList.begin( ),
            ai->ignoreList.end( ),
            hitEntity->GetName( )
        );

        if (hitEntity == aiOwner->GetRoot( ) ||
            (ai->collisionList.Size( ) && collisionItr == ai->collisionList.end( )) ||
            (ai->ignoreList.Size( ) && ignoreItr != ai->ignoreList.end( )))
            continue;

        auto hit = output.hit[ i ];

        if (hit.Y( ) < lowestY)
        {
            lowestY = hit.Y( );
            found = true;
        }
    }

    if (found)
        return lowestY;
    else
        return aiPosition.Y( );
}

bool VineStateUtils::AtTarget(VineAI *ai, float distance)
{
    auto targetPos = ai->GetTarget( )->GetTransform( )->GetWorldPosition( );
    auto aiPos = ai->GetOwner( )->GetTransform( )->GetWorldPosition( );

    targetPos.Y( ) = 0.0f;
    aiPos.Y( ) = 0.0f;

    float dist = SVec3::Distance( targetPos, aiPos );

    return dist <= distance;
}

bool VineStateUtils::AtHome(VineAI *ai, float distance)
{
    auto homePos = ai->GetHomeLocation( );
    auto aiPos = ai->GetOwner( )->GetTransform( )->GetWorldPosition( );

    aiPos.Y( ) = 0.0f;
    homePos.Y( ) = 0.0f;

    float dist = SVec3::Distance( homePos, aiPos );

    return dist <= distance;
}
