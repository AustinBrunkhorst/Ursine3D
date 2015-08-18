/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RigidBody2D.h
** - The 2D Rigidbody component
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef RIGIDBODY2D_COMPONENT_H
#define RIGIDBODY2D_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#include "Component.h"
#include "TimeSpan.h"
#include "Vec2.h"
#include "Shape.h"
#include "Collider.h"
#include "AABB.h"
#include "LayerMask.h"

namespace Ursine
{
    // The RigidBody2D's body type
    // kinematic = zero mass, non-zero velocity, moved by system
    // static = zero mass, zero velocity, manually moved only
    // dynamic = positive mass, non-zero velocity determined by forces, moved by system
    // ghost = do not resolve collision, but still send events
    // kinematic solver = Same properties as a kinematic body, except it 
    //                    resolves collisions with static and kineamatic objects
    enum BodyType
    {
        BODY_TYPE_KINEMATIC,
        BODY_TYPE_STATIC,
        BODY_TYPE_DYNAMIC,
        BODY_TYPE_GHOST,
        BODY_TYPE_KINEMATIC_SOLVE
    };
    
    // Forward declarations
    class Collision;
    class Manifold;
    class DistanceJoint;

    namespace ECS
    {
        class RigidBody2D : public Component
        {
            friend class PhysicsDebugDrawer;
            friend class Simulation;
            friend class Ursine::Collision;
            friend class Ursine::Manifold;
            friend class Ursine::DistanceJoint;

        public:
            explicit RigidBody2D(BodyType type = BODY_TYPE_DYNAMIC);
            RigidBody2D(const RigidBody2D &other);
            ~RigidBody2D(void);

            // Initializer after serialization phase
            void OnInitialize(void) override;

            // Apply force to the center of mass.
            void AddForce(const Vec2 &force);

            // Apply force at a world position.  If this isn't
            // the center of mass, this will affect angular velocity.
            void AddForceAtPosition(const Vec2 &force, const Vec2 &position);

            // Apply an impulse to the center of mass. This will
            // immediately modify the velocity.
            void AddLinearImpulse(const Vec2 &impulse);

            // Apply an impulse at a world position. This will 
            // immediately modify the velocity.  If the position isn't
            // the center of mass, this will affect angular velocity.
            void AddLinearImpulseAtPosition(const Vec2 &impulse, const Vec2 &position);

            // Apply an angular impulse
            void AddAngularImpulse(float impulse);

            // This affects the angular velocity
            void AddTorque(float torque);

            void AddAcceleration(const Vec2 &acceleration);

            void SetDensity(float density);

            void SetVelocity(const Vec2 &velocity);
            void AddVelocity(const Vec2 &velocity);

            Vec2 GetVelocity(void);

            void SetVelocityAtPosition(const Vec2 &velocity, const Vec2 &position);

            void SetAngularVelocity(float velocity);

            void SetEventable(bool eventable);

            // move the rigid body to a position (in world space) over a specified time
            void Move(const Vec2 &position, const TimeSpan &time);

            // move the rigid body to a certain rotation
            void Rotate(float radians, float time);

            // Calculate the mass of the object
            void ComputeMass(void);

            // Returns the center of mass in world coordinates
            Vec2 GetCenterOfMass(void);

            const AABB &GetAABB(void) const;

            BodyType GetBodyType(void) const;

            Transform *GetTransform(void);

            void SetBodyType(BodyType type);

            void SetPositionLocked(bool flag);

            void SetRotationLocked(bool flag);

            void SetVelocityCap(float cap);

            float GetVelocityCap(void);

            float GetInitVelocityCap(void);

            void ResetVelocityCap(void);
            
            void SetAngularVelocityCap(float cap);

            void SetCollisionLayer(LayerID layer);

            // add a kinematic child to this rigid body
            void AddKinChild(RigidBody2D *body);
            void AddKinChildAlreadyInLocal(RigidBody2D *body);

            // remove this child
            void RemoveKinChild(RigidBody2D *body);

            // update the edges of all colliders attached to this rigid body
            void UpdateColliders(void);

            // update all of the bodies attributes (mass, COM, collider verts)
            void UpdateBody(void);

            LayerID GetLayerID(void) const;

        private:
            // Check for invalid values and assert errors if they occur
            void checkValues(void);

            // setting inverse mass/inertia to 0
            void setRotLockedValues(void);
            void setPosLockedValues(void);

            // parents the colliders after serialization
            void parentColliders(Transform *parent, Colliders &children);

            // Copies all colliders to the new vector recursively
            void copyColliders(Colliders &dest, const Colliders &source);

            // delete all dynamically allocated colliders
            void deleteColliders(Colliders &children);

            // updates the children colliders' edges
            // returns the bounding box encompassing the children
            AABB updateChildren(Colliders &children, bool reset = false);

            // compute the mass of all children
            void computeMassOfColliders(Colliders &colliders, Vec2 &center, 
                                        float &area, float &inertia, unsigned &collider_count);

            // the owner of the rigidbody's transform
            Transform *_transform;

            // the center of mass (computer in ComputeMass)
            Vec2 _c_mass;

            SerializerAllow(RigidBody2D, colliders);
            Colliders colliders;

            SerializerAllow(RigidBody2D, body_type);
            // type of rigidbody, see enum above
            BodyType body_type;

            // linear velocity
            Vec2 _velocity;
            // angular velocity in radians
            float _angular_velocity;

            // force to be applied to the velocity
            Vec2 _force;
            // torque to be applied to the angular velocity
            float _torque;

            SerializerAllow(RigidBody2D, friction);
            float friction;

            SerializerAllow(RigidBody2D, restitution);
            // This will change how bouncy this object is
            float restitution;
            
            // rotational inertia == rotational resistance
            // this is determined by the mass
            float _inv_inertia, _inertia;

            //   (1 / mass), (mass)
            float _inv_mass, _mass;

            SerializerAllow(RigidBody2D, density);
            // Density of the object (this effects the mass and rotation inertia
            float density;

            // bounding box of the shape
            AABB _aabb;

            SerializerAllow(RigidBody2D, layer);
            // collision layer of this body
            LayerID layer;

            SerializerAllow(RigidBody2D, layer_mask);
            // mask of all layers this body interacts with
            LayerMask layer_mask;

            SerializerAllow(RigidBody2D, sends_events);
            // determine whether this collider sends events or not
            bool sends_events;

            // Have physics ignore the position or rotation of this object
            bool _pos_locked;
            bool _rot_locked;

            // Velocity caps for this body
            float _vel_cap;
            float _init_vel_cap;
            float _ang_vel_cap;

            // stored so we can cancle it when a new one comes in
            TweenID _move_tween;
        };

        bool ColliderRayCast(Colliders &colliders, RayCastOutput &output, const RayCastInput &intput);
    }

    extern template
    Json JsonSerializer::Serialize(BodyType &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, BodyType &out);
}

////////////////////////////////////////////////////////////////////////////////
#endif // !RIGIDBODY2D_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS

#ifndef RIGIDBODY2D_REGISTERED
#define RIGIDBODY2D_REGISTERED

namespace Ursine
{
    namespace ECS
    {
        RegisterComponent(RigidBody2D);
        RegisterComponentData(RigidBody2D, colliders);
        RegisterComponentData(RigidBody2D, body_type);
        RegisterComponentData(RigidBody2D, friction);
        RegisterComponentData(RigidBody2D, restitution);
        RegisterComponentData(RigidBody2D, density);
        RegisterComponentData(RigidBody2D, layer);
        RegisterComponentData(RigidBody2D, layer_mask);
        RegisterComponentData(RigidBody2D, sends_events);
    }
}

#endif

#endif
