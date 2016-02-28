/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsInteropConfig.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#define BULLET_PHYSICS

#ifdef BULLET_PHYSICS

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletWorldImporter/btWorldImporter.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

namespace ursine
{
    namespace physics
    {
        // Debug drawer
        typedef btIDebugDraw DebugDrawerBase;

        // Collision shapes
        typedef btCollisionShape ColliderBase;
        typedef btSphereShape SphereColliderBase;
        typedef btBoxShape BoxColliderBase;
        typedef btCylinderShape CylinderColliderBase;
        typedef btCapsuleShape CapsuleColliderBase;
        typedef btConeShape ConeColliderBase;
        typedef btEmptyShape EmptyColliderBase;
        typedef btConvexHullShape ConvexHullColliderBase;
        typedef btCompoundShape ConvexDecompColliderBase;
        typedef btScaledBvhTriangleMeshShape BvhTriangleMeshColliderBase;

        // Collision Object
        typedef btCollisionObject BodyBase;

        // Rigidbodies
        typedef btRigidBody RigidbodyBase;
        typedef btRigidBody::btRigidBodyConstructionInfo RigidbodyConstructionInfo;

        // Ghost Object
        typedef btPairCachingGhostObject GhostBase;

        // Motion state
        typedef btDefaultMotionState MotionStateBase;

        // Manifold
        typedef btPersistentManifold PersistentManifold;

        enum BodyType
        {
            BT_BODY = btCollisionObject::CO_COLLISION_OBJECT,
            BT_RIGID_BODY = btCollisionObject::CO_RIGID_BODY,
            BT_GHOST = btCollisionObject::CO_GHOST_OBJECT,
            BT_SOFT_BODY = btCollisionObject::CO_SOFT_BODY
        };

        enum BodyFlag
        {
            BF_DYNAMIC = 0,
            BF_KINEMATIC = btRigidBody::CF_KINEMATIC_OBJECT,
            BF_STATIC = btRigidBody::CF_STATIC_OBJECT
        };
    }
}

#endif
