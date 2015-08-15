#include "UrsinePrecompiled.h"

namespace Ursine
{
    DistanceJoint::DistanceJoint(ECS::RigidBody2D *body_0, ECS::RigidBody2D *body_1,
        float distance, bool max_distance_only)
        : _distance(distance)
        , _max_distance_only(max_distance_only)
        , _spring_constant(1.0f)
    {
        _body[0] = body_0;
        _body[1] = body_1;

        UAssert(_body[0]->body_type == BODY_TYPE_DYNAMIC ||
                _body[1]->body_type == BODY_TYPE_DYNAMIC,
                "One of the bodies has to be dynamic.");

        for (int i = 0; i < 2; ++i)
        {
            _trans[i] = _body[i]->GetOwner()->GetTransform();
            _rot_locked[i] = false;
            _rot_anchor[i] = 0.0f;
        }
    }

    DistanceJoint::~DistanceJoint(void)
    {

    }

    void DistanceJoint::PreStep(void)
    {
        _trans_anchor[0] = _trans[0]->ToWorld(_anchor[0]);
        _trans_anchor[1] = _trans[1]->ToWorld(_anchor[1]);
    }

    void DistanceJoint::Solve(float dt)
    {
        static const float damp_factor = 0.1f;

        Vector2 dir = _trans_anchor[1] - _trans_anchor[0];
        float distance = dir.Length();
        dir.Normalize();

        float rel_vel = Vector2::Dot(
            Vector2(_body[1]->GetVelocity() - _body[0]->GetVelocity()),
            dir
        );

        float rel_dist = distance - _distance;


        if (!_max_distance_only || distance >= _distance)
        {
            float remove = rel_vel + rel_dist / dt;

            float impulse = remove / (_body[0]->_inv_mass + _body[1]->_inv_mass);

            Vector2 I = dir * impulse * damp_factor * _spring_constant;
                
            if (_rot_locked[0])
                _body[0]->AddLinearImpulse(I);
            else
                _body[0]->AddLinearImpulseAtPosition(I, _trans_anchor[0]);

            if (_rot_locked[1])
                _body[1]->AddLinearImpulse(-I);
            else
                _body[1]->AddLinearImpulseAtPosition(-I, _trans_anchor[1]);
        }
    }

    void DistanceJoint::PostStep()
    {
        for (unsigned i = 0; i < 2; ++i)
        {
            if (_rot_locked[i])
            {
                _trans_rot_anchor[i] = _rot_anchor[i] +
                    (_trans_anchor[1] - _trans_anchor[0]).Angle();

                _body[i]->Rotate(_trans_rot_anchor[i], 0.016f);
            }
        }
    }

    DistanceJoint* DistanceJoint::SetDistance(float distance)
    {
        _distance = distance;
            
        return this;
    }

    DistanceJoint* DistanceJoint::SetAnchor(uint body_index, const Vector2 &anchor)
    {
        UAssert(body_index < 2, "Must provide a valid index (0, 1)");
            
        _anchor[body_index] = anchor;

        return this;
    }

    DistanceJoint* DistanceJoint::SetMaxDistanceOnly(bool flag)
    {
        _max_distance_only = flag;

        return this;
    }

    DistanceJoint* DistanceJoint::SetSpring(const float constant)
    {
        _spring_constant = constant;

        return this;
    }

    DistanceJoint* DistanceJoint::SetRotationLock(uint body_index, float world_rotation)
    {
        UAssert(body_index < 2, "Must provide a valid index (0, 1)");

        _rot_locked[body_index] = true;

        _rot_anchor[body_index] = Math::Wrap(world_rotation, 0.0f, Math::PI_2);

        return this;
    }
}
