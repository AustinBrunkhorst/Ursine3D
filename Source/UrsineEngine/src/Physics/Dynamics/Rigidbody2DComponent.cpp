/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RigidBody2DComponent.hpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
**
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace ursine
{
    namespace ecs
    {
        RigidBody2D::RigidBody2D(BodyType type)
            : BaseComponent()
            , _transform(nullptr)
            , body_type(type)
            , _velocity(0)
            , _angular_velocity(0)
            , _force(0)
            , _torque(0)
            , friction(0)
            , restitution(0.2f)
            , _inv_inertia(1)
            , _inertia(1)
            , _inv_mass(1)
            , _mass(1)
            , density(1)
            , layer(0)
            , layer_mask()
            , sends_events(true)
            , _rot_locked(false)
            , _pos_locked(false)
            , _vel_cap(1000)
            , _ang_vel_cap(1000)
        {
            if (type == BODY_TYPE_KINEMATIC || type == BODY_TYPE_STATIC || type == BODY_TYPE_KINEMATIC_SOLVE)
            {
                setPosLockedValues();
                setRotLockedValues();
            }
        }

        RigidBody2D::RigidBody2D(const RigidBody2D &other)
            : BaseComponent()
            , _c_mass(other._c_mass)
            , body_type(other.body_type)
            , _velocity(other._velocity)
            , _angular_velocity(other._angular_velocity)
            , _force(other._force)
            , _torque(other._torque)
            , friction(other.friction)
            , restitution(other.restitution)
            , _inertia(other._inertia)
            , _inv_inertia(other._inv_inertia)
            , _mass(other._mass)
            , _inv_mass(other._inv_mass)
            , density(other.density)
            , _aabb(other._aabb)
            , layer(other.layer)
            , layer_mask(other.layer_mask)
            , sends_events(other.sends_events)
            , _rot_locked(other._rot_locked)
            , _pos_locked(other._pos_locked)
            , _vel_cap(other._vel_cap)
            , _ang_vel_cap(other._ang_vel_cap)
        {
            // copy over the colliders
            copyColliders(colliders, other.colliders);
        }

        void RigidBody2D::OnInitialize()
        {
            checkValues();

            _transform = GetOwner()->GetTransform();

            // parent the colliders after serialization
            parentColliders(_transform, colliders);

            UpdateBody();
            _init_vel_cap = _vel_cap;
        }

        RigidBody2D::~RigidBody2D()
        {
            deleteColliders(colliders);
        }

        void RigidBody2D::deleteColliders(Colliders &children)
        {
            for (auto &child : children)
            {
                deleteColliders(child->children);

                delete child;
            }
        }

        AABB RigidBody2D::updateChildren(Colliders &children, bool reset)
        {
            static AABB bounds;

            if (reset)
            {
                bounds.low_bound.Set(std::numeric_limits<float>().max(),
                                    std::numeric_limits<float>().max());
                bounds.up_bound.Set(-std::numeric_limits<float>().max(),
                                    -std::numeric_limits<float>().max());
            }

            for (auto &child : children)
            {
                updateChildren(child->children);

                AABB child_bounds = child->shape->TransformShape(child->transform);
                child->aabb = child_bounds;
                bounds.Combine(child_bounds);
            }

            return bounds;
        }

        void RigidBody2D::UpdateColliders()
        {       
            AABB bounds = updateChildren(colliders, true);

            _aabb = bounds;
        }

        void RigidBody2D::UpdateBody(void)
        {
            UpdateColliders();
            ComputeMass();
        }

        void RigidBody2D::checkValues()
        {
            UAssert(!(density == 0.0f), "Density cannot be zero.");
        }

        void RigidBody2D::parentColliders(Transform *parent, Colliders &children)
        {
            // recursively parent all children through the transform
            // assuming that their values are set in local space via serialization
            for (auto &child : children)
            {
                parent->AddChildAlreadyInLocal(&child->transform);
                parentColliders(&child->transform, child->children);
            }
        }

        void RigidBody2D::copyColliders(Colliders &dest, const Colliders &source)
        {
            for (int i = 0, size = source.size(); i < size; ++i)
            {
                dest.push_back(new Collider(*source[i]));

                copyColliders(dest[i]->children, source[i]->children);
            }
        }

        void RigidBody2D::computeMassOfColliders(Colliders &colliders,
            Vec2 &center, float &area, float &inertia, unsigned &collider_count)
        {
            for (auto &collider : colliders)
            {
                computeMassOfColliders(collider->children, center, area, inertia, collider_count);

                collider->shape->ComputeMass(collider->transform, center, area, inertia);

                ++collider_count;
            }
        }

        void RigidBody2D::ComputeMass()
        {
            // Setting mass if object is of type kinematic or static
            if (body_type == BODY_TYPE_KINEMATIC || body_type == BODY_TYPE_STATIC || body_type == BODY_TYPE_KINEMATIC_SOLVE)
            {
                setRotLockedValues();
                setPosLockedValues();
                return;
            }

            // calculate the centroid and moment of inertia
            Vec2 center(0.0f, 0.0f); // centroid
            float area = 0.0f;
            float inertia = 0.0f;
            unsigned collider_count = 0;

            // recursively calculate all mass of all colliders in the tree
            computeMassOfColliders(colliders, center, area, inertia, collider_count);

            _mass = density * area;
            _inv_mass = _mass == 0.0f ? 0.0f : 1.0f / _mass;
            _inertia = density * inertia;
            _inv_inertia = _inertia == 0.0f ? 0.0f : 1.0f / _inertia;

            _c_mass = center / static_cast<float>(collider_count);
            
            if (_rot_locked)
            {
                setRotLockedValues();
            }
            else if (_pos_locked)
            {
                setPosLockedValues();
            }
        }

        Vec2 RigidBody2D::GetCenterOfMass()
        {
            return _transform->Root()->MatrixNoScalar().TransformPoint(_c_mass);
        }

        void RigidBody2D::AddForce(const Vec2 &force)
        {
            if (body_type != BODY_TYPE_DYNAMIC || _pos_locked)
                return;

            // accumulate all forces
            _force += force;

            // check for infinite values
            if (!Math::IsFiniteNumber(_force.X()))
                _force.X() = 0.0f;
            if (!Math::IsFiniteNumber(_force.Y()))
                _force.Y() = 0.0f;
        }

        void RigidBody2D::AddForceAtPosition(const Vec2 &force, const Vec2 &position)
        {
            if (body_type != BODY_TYPE_DYNAMIC)
                return;

            AddForce(force);

            Vec2 com = GetCenterOfMass();

            // create torque based on the given position
            AddTorque(Vec2::Cross(position - com, force));
        }

        void RigidBody2D::AddLinearImpulse(const Vec2 &impulse)
        {
            if (body_type != BODY_TYPE_DYNAMIC || _pos_locked)
                return;

            // apply the impulse directly to the body's velocity
            SetVelocity(_velocity + _inv_mass * impulse);
        }

        void RigidBody2D::AddLinearImpulseAtPosition(const Vec2 &impulse, const Vec2 &position)
        {
            if (body_type != BODY_TYPE_DYNAMIC)
                return;

            SetVelocity(_velocity + _inv_mass * impulse);

            Vec2 com = GetCenterOfMass();

            SetAngularVelocity(
                _angular_velocity + _inv_inertia * Vec2::Cross(position - com, impulse)
            );
        }

        void RigidBody2D::AddAngularImpulse(float impulse)
        {
            if (body_type != BODY_TYPE_DYNAMIC || _rot_locked)
                return;

            SetAngularVelocity(_angular_velocity + _inv_inertia * impulse);
        }

        void RigidBody2D::AddTorque(float torque)
        {
            if (body_type != BODY_TYPE_DYNAMIC || _rot_locked)
                return;

            _torque += torque;

            // check for infinite values
            if (!Math::IsFiniteNumber(_torque))
                _torque = 0.0f;
        }

        void RigidBody2D::AddAcceleration(const Vec2 &acceleration)
        {
            if (body_type != BODY_TYPE_DYNAMIC || _pos_locked)
                return;

            if (_inv_mass == 0.0f)
                return;

            AddForce(acceleration * _mass);
        }

        void RigidBody2D::SetDensity(float Density)
        {
            density = Density;

            UAssert(density > 0.0f, "Density must be greater then zero.");

            ComputeMass();
        }

        void RigidBody2D::SetVelocity(const Vec2 &velocity)
        {
            if (body_type == BODY_TYPE_STATIC || _pos_locked)
                return;

            _velocity = {
                Math::Clamp(velocity.X(), -_vel_cap, _vel_cap),
                Math::Clamp(velocity.Y(), -_vel_cap, _vel_cap)
            };
        }

        void RigidBody2D::AddVelocity(const Vec2 &velocity)
        {
            if (body_type == BODY_TYPE_STATIC || _pos_locked)
                return;

            _velocity = {
                Math::Clamp(_velocity.X() + velocity.X(), -_vel_cap, _vel_cap),
                Math::Clamp(_velocity.Y() + velocity.Y(), -_vel_cap, _vel_cap)
            };
        }

        Vec2 RigidBody2D::GetVelocity(void)
        {
            return _velocity;
        }

        void RigidBody2D::SetVelocityAtPosition(const Vec2 &velocity, const Vec2 &Position)
        {
            if (body_type == BODY_TYPE_STATIC)
                return;

            SetVelocity(velocity);

            Vec2 com = GetCenterOfMass();

            SetAngularVelocity(
                _inv_inertia * Vec2::Cross(Position - com, velocity)
            );
        }

        void RigidBody2D::SetAngularVelocity(float velocity)
        {
            if (body_type == BODY_TYPE_STATIC || _rot_locked)
                return;

            _angular_velocity = Math::Clamp(velocity, -_ang_vel_cap, _ang_vel_cap);
        }

        void RigidBody2D::SetEventable(bool eventable)
        {
            sends_events = eventable;
        }

        void RigidBody2D::Move(const Vec2 &position, const TimeSpan &time)
        {
            Vec2 delta = position - Vec2(_transform->PositionWC());
            Vec2 start;
            float seconds = static_cast<float>(time.Milliseconds()) / 1000.0f;
            Vec2 target = 2 * delta * (1.0f / seconds);

            // reset the velocity
            SetVelocity({ 0, 0 });

            // cancle the previous tween
            _move_tween.Cancel();

            _move_tween = GetOwner()->GetTweens().Create()
                .BeginGroup()
                    .Setter(this, &RigidBody2D::SetVelocity, start, target, time, Ease::CubicOut)
                .EndGroup();
        }

        void RigidBody2D::Rotate(float radians, float time)
        {
            // This movement is based on the follow kinematic equation
            // delta theta = angular_velocity * time + 0.5 * angular_accel * time * time
            // solving the equation in terms of angular_accel
            // a = (-2(angular_velocity * time - theta))/(time * time)

            if (body_type != BODY_TYPE_DYNAMIC || _rot_locked)
                return;

            radians = Math::Wrap(radians, 0.0f, Math::PI_2);

            float theta = radians - _transform->RootRotation2D();
            float vel = _angular_velocity;

            theta = Math::Wrap(theta, -Math::PI, Math::PI);

            // reset the angular velocity
            _angular_velocity = 0;

            AddTorque((-2 * (vel * time * time - theta) / (time * time)) * _inertia);
        }

        const AABB &RigidBody2D::GetAABB() const
        {
            return _aabb;
        }

        BodyType RigidBody2D::GetBodyType() const
        {
            return body_type;
        }

        void RigidBody2D::SetBodyType(BodyType type)
        {
            body_type = type;

            ComputeMass();
        }

        void RigidBody2D::SetPositionLocked(bool flag)
        {
            _pos_locked = flag;

            if (_pos_locked)
                setPosLockedValues();
            else
                ComputeMass();
        }

        void RigidBody2D::SetRotationLocked(bool flag)
        {
            _rot_locked = flag;

            if (_rot_locked)
                setRotLockedValues();
            else
                ComputeMass();
        }

        void RigidBody2D::SetVelocityCap(float cap)
        {
            _vel_cap = cap;
        }

        float RigidBody2D::GetVelocityCap(void)
        {
            return _vel_cap;
        }

        float RigidBody2D::GetInitVelocityCap(void)
        {
            return _init_vel_cap;
        }

        void RigidBody2D::ResetVelocityCap(void)
        {
            _vel_cap = _init_vel_cap;
        }


        void RigidBody2D::SetAngularVelocityCap(float cap)
        {
            _ang_vel_cap = cap;
        }

        void RigidBody2D::SetCollisionLayer(LayerID Layer)
        {
            layer = Layer;
        }

        LayerID RigidBody2D::GetLayerID(void) const
        {
            return layer;
        }

        void RigidBody2D::AddKinChild(RigidBody2D *body)
        {
            auto entity = body->GetOwner();

            // add the kinematic child component
            if (!entity->GetComponent<KinematicChild>())
                entity->AddComponent<KinematicChild>();

            // Add the transform to our transform's hierarchy
            _transform->AddChild(entity->GetTransform());
        }

        void RigidBody2D::AddKinChildAlreadyInLocal(RigidBody2D *body)
        {
            auto entity = body->GetOwner();

            // add the kinematic child component
            if (!entity->GetComponent<KinematicChild>())
                entity->AddComponent<KinematicChild>();

            // Add the transform to our transform's hierarchy
            _transform->AddChildAlreadyInLocal(entity->GetTransform());
        }

        void RigidBody2D::RemoveKinChild(RigidBody2D *body)
        {
            auto entity = body->GetOwner();

            UAssert(entity->GetComponent<KinematicChild>(), "This entity must have the KinematicChild component");
            
            // Remove the component
            entity->RemoveComponent<KinematicChild>();

            // Remove the transform from our transform's hierarchy
            _transform->RemoveChild(entity->GetTransform());
        }

        Transform * RigidBody2D::GetTransform()
        {
            return _transform;
        }

        void RigidBody2D::setRotLockedValues(void)
        {
            _inertia = 1.0f;
            _inv_inertia = 0.0f;
        }

        void RigidBody2D::setPosLockedValues(void)
        {
            _mass = 1.0f;
            _inv_mass = 0.0f;
        }

        static void recursRayCast(Colliders &colliders, RayCastOutput &output, 
                            const RayCastInput &input, bool *result)
        {
            RayCastOutput temp_output;

            for (auto &collider : colliders)
            {
                recursRayCast(collider->children, output, input, result);

                if (collider->aabb.RayCast(temp_output, input) && 
                    collider->shape->RayCast(temp_output, input))
                {
                    if (temp_output.fraction < output.fraction)
                    {
                        *result = true;
                        output.fraction = temp_output.fraction;
                        output.normal = temp_output.normal;
                        
                        Transform *parent = collider->transform.Parent();

                        // find the first transform in the hierarch who has an entity
                        // associated with it
                        while (!parent->GetOwner())
                            parent = parent->Parent();

                        output.entity = parent->GetOwner();
                    }
                }
            }
        }

        bool ColliderRayCast(Colliders &colliders, RayCastOutput &output, const RayCastInput &input)
        {
            bool result = false;
            output.fraction = input.max_fraction;

            recursRayCast(colliders, output, input, &result);

            return result;
        }
    }

    template<>
    Json JsonSerializer::Serialize(BodyType &instance)
    {
        return Json(static_cast<int>(instance));
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, BodyType &out)
    {
        out = static_cast<BodyType>(data.int_value());
    }
}