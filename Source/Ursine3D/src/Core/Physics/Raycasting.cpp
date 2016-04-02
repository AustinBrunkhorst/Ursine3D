/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Raycasting.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Raycasting.h"
#include "Entity.h"
#include "World.h"
#include "GhostComponent.h"

namespace ursine
{
    namespace physics
    {
        ClosestNonGhostRayResultCallback::ClosestNonGhostRayResultCallback(const btVector3&	rayFromWorld, const btVector3&	rayToWorld, ursine::ecs::World* world)
            : ClosestRayResultCallback(rayFromWorld, rayToWorld)
            , m_world(world)
        {
        }

        btScalar ClosestNonGhostRayResultCallback::addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
        {
            //caller already does the filter on the m_closestHitFraction
            btAssert(rayResult.m_hitFraction <= m_closestHitFraction);

            ursine::ecs::EntityHandle objHit = m_world->GetEntity(rayResult.m_collisionObject->getUserIndex( ));

            if ( objHit->HasComponent<ursine::ecs::Ghost>( ) && 
                !objHit->GetComponent<ursine::ecs::Ghost>( )->GetEnableRaycast( ) )
            {
                return rayResult.m_hitFraction;
            }

            m_closestHitFraction = rayResult.m_hitFraction;
            m_collisionObject = rayResult.m_collisionObject;
            if ( normalInWorldSpace )
            {
                m_hitNormalWorld = rayResult.m_hitNormalLocal;
            }
            else
            {
                ///need to transform normal into worldspace
                m_hitNormalWorld = m_collisionObject->getWorldTransform( ).getBasis( )*rayResult.m_hitNormalLocal;
            }
            m_hitPointWorld.setInterpolate3(m_rayFromWorld, m_rayToWorld, rayResult.m_hitFraction);
            return rayResult.m_hitFraction;
        }

    }
}