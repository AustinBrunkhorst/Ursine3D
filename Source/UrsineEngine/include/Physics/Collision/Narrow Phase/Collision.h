/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Collision.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "CollisionEventArgs.h"
#include "Manifold.h"

namespace Ursine
{
    // Forward declarations
    namespace ECS
    {
        class RigidBody2D;
    }

    // Collision detection utility class
    class Collision
    {
        // Run narrow phase detection on two colliders
        static void compareColliders(Collider *collider_a, ECS::RigidBody2D *a,
                                     Collider *collider_b, ECS::RigidBody2D *b,
                                     Contacts &contacts);

        // Run narrow phase detection with collider and tree
        static void compareColliderAndTree(Collider *collider, ECS::RigidBody2D *a,
                                           Colliders &tree, ECS::RigidBody2D *b,
                                           Contacts &contacts);

        // Run narrow phase detection with both collider trees
        static void handleNarrowPhase(Colliders &tree_a, ECS::RigidBody2D *a,
                                      Colliders &tree_b, ECS::RigidBody2D *b, 
                                      Contacts &contacts);

    public:
        static void Collide(Contacts &contacts, ECS::RigidBody2D *a, ECS::RigidBody2D *b);

        static bool RaySegmentIntersection(const RayCastInput &input, 
                                           const Vector2 &p_0, const Vector2 &p_1, 
                                           float &t);

        // Send collision events for both interacting bodies
        static void SendEvents(ECS::RigidBody2D *a, ECS::RigidBody2D *b,
                               Manifold &manifold, CollisionEvents event);

        // Set all colliders in the tree to false
        static void SetCollisionFalse(Colliders &colliders);
    };
}
