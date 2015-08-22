/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Manifold.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace Ursine
{
    Contact::Contact(void)
        : norm_imp(0.0f)
        , tang_imp(0.0f)
        , norm_bias_imp(0.0f)
        , mass_norm(0.0f)
        , mass_tan(0.0f)
        , bias(0.0f)
    {
            
    }

    Manifold::Manifold(ECS::RigidBody2D *A, ECS::RigidBody2D *B)
    {
        ECS::EntityUniqueID id_a = A->GetOwner()->GetUniqueID(),
                            id_b = B->GetOwner()->GetUniqueID();

        if (id_a < id_b)
        {
            a = A; b = B;
        }
        else
        {
            a = B; b = A;
        }

        Collision::Collide(contacts, a, b);

        // set the friction and restitution values
        e = Math::Max(a->restitution, b->restitution);
        f = sqrt(a->friction * b->friction);
    }

    void Manifold::Update(Contacts &new_contacts)
    {
        Contacts merged;

        // check to see if the points are the same
        for (uint i = 0; i < new_contacts.size(); ++i)
        {
            Contact &new_c = new_contacts[i];

            int k = -1;

            for (uint j = 0; j < contacts.size(); ++j)
            {
                Contact &old_c = contacts[j];

                if (new_c.info.value == old_c.info.value)
                {
                    k = j;
                    break;
                }
            }

            if (k > -1)
            {
                merged.push_back(Contact());
                Contact &c = merged.back();
                Contact &old_c = contacts[k];

                c = new_c;

                // Warm start the impulses
                c.norm_imp = old_c.norm_imp;
                c.tang_imp = old_c.tang_imp;
                c.norm_bias_imp = old_c.norm_bias_imp;                    
            }
            else
            {
                merged.push_back(new_contacts[i]);
            }
        }

        contacts = merged;
    }

    void Manifold::PreStep(float inv_dt)
    {
        static const float pen_slop = 0.01f;
        static const float bias_factor = 0.2f;
        static const float bias_threshold = 10.5f;
            
        for (int i = 0, size = contacts.size(); i < size; ++i)
        {
            Contact &c = contacts[i];

            positionalCorrection(c);

            Vec2 rel_a = c.point - a->GetCenterOfMass();
            Vec2 rel_b = c.point - b->GetCenterOfMass();

            // compute normal mass
            float rna = Vec2::Dot(rel_a, c.normal);
            float rnb = Vec2::Dot(rel_b, c.normal);
            float denom = a->_inv_mass + b->_inv_mass;
            denom += a->_inv_inertia *
                (Vec2::Dot(rel_a, rel_a) - rna * rna) +
                b->_inv_inertia *
                (Vec2::Dot(rel_b, rel_b) - rnb * rnb);
            c.mass_norm = Math::IsZero(denom) ? 0.0f : 1.0f / denom;

            // compute tangent mass
            Vec2 tan = Vec2::Cross(1.0f, c.normal);
            float rta = Vec2::Dot(rel_a, tan);
            float rtb = Vec2::Dot(rel_b, tan);
            denom = a->_inv_mass + b->_inv_mass;
            denom += a->_inv_inertia *
                (Vec2::Dot(rel_a, rel_a) - rta * rta) +
                b->_inv_inertia *
                (Vec2::Dot(rel_b, rel_b) - rtb * rtb);
            c.mass_tan = Math::IsZero(denom) ? 0.0f : 1.0f / denom;

            c.bias = -bias_factor * inv_dt * Math::Min(0.0f, (-c.pen_depth) + pen_slop);

            // clamp the bias, resolves shooting out objects when correcting overlap
            if (c.bias > bias_threshold)
                c.bias = bias_threshold;

            // apply initial accumulated impulse
            Vec2 impulse = c.norm_imp * c.normal + c.tang_imp * tan;

            impulse = impulse * e;

            a->AddLinearImpulseAtPosition(-impulse, c.point);
            b->AddLinearImpulseAtPosition(impulse, c.point);
        }
    }

    void Manifold::ApplyImpulse(void)
    {
        for (int i = 0, size = contacts.size(); i < size; ++i)
        {
            Contact &c = contacts[i];

            c.rel_a = c.point - a->GetCenterOfMass();
            c.rel_b = c.point - b->GetCenterOfMass();

            // Calculate relative velocity
            Vec2 rel_v = b->_velocity + Vec2::Cross(b->_angular_velocity, c.rel_b) -
                a->_velocity - Vec2::Cross(a->_angular_velocity, c.rel_a);

            // Calculate the velocity along the normal (normal impulse)
            float contact_vel = Vec2::Dot(rel_v, c.normal);

            // Calculate the accumulated impulse
            float impulse = c.mass_norm * (-contact_vel + c.bias);

            // Clamp the total accumulated impulse
            float prev = c.norm_imp;
            c.norm_imp = Math::Max(prev + impulse, 0.0f);

            // Apply the remainder
            impulse = c.norm_imp - prev;

            // Calculate the contact impulse
            Vec2 contact_impulse = impulse * c.normal;

            // Apply the contact impulse
            a->AddLinearImpulseAtPosition(-contact_impulse, c.point);
            b->AddLinearImpulseAtPosition(contact_impulse, c.point);

            // Relative velocity at contact
            rel_v = b->_velocity + Vec2::Cross(b->_angular_velocity, c.rel_b) -
                a->_velocity - Vec2::Cross(a->_angular_velocity, c.rel_a);

            Vec2 tang = Vec2::Cross(1.0f, c.normal);

            // Calculate the velocity along the tangent (tangent impulse)
            float tangent_vel = Vec2::Dot(rel_v, tang);

            // Calculate the accumulated impulse
            impulse = c.mass_tan * (-tangent_vel);

            // Compuate maximum friction impulse
            float max_friction = f * c.norm_imp;

            // Clamp the friction
            prev = c.tang_imp;
            c.tang_imp = Math::Clamp(prev + impulse, -max_friction, max_friction);

            // Apply the remainder
            impulse = c.tang_imp - prev;

            // Apply the impulse
            Vec2 tangent_impulse = impulse * tang;

            a->AddLinearImpulseAtPosition(-tangent_impulse, c.point);
            b->AddLinearImpulseAtPosition(tangent_impulse, c.point);
        }
    }

    void Manifold::positionalCorrection(const Contact &contact)
    {
        static const float ratio_threshold = 0.25f;
        static const float damp_factor = 0.99f;

        if (a->body_type == BODY_TYPE_DYNAMIC && a->_inv_mass != 0.0f)
        {
            Vec2 scale = Vec2(a->_transform->ScaleWC());

            float avg_scale = (scale.X() + scale.Y()) / 2.0f;

            // if we're penetrating past the threshold
            if (contact.pen_depth / avg_scale >= ratio_threshold)
            {
                if (a->_pos_locked)
                    a->_transform->AddRootPosition(SVec3(
                        -contact.normal * contact.pen_depth * damp_factor, 0
                    ));
                else
                    a->_transform->AddPositionWC(SVec3(
                        -contact.normal * contact.pen_depth * damp_factor, 0
                    ));
            }
        }
        if (b->body_type == BODY_TYPE_DYNAMIC && b->_inv_mass != 0.0f)
        {
            Vec2 scale = Vec2(b->_transform->ScaleWC());

            float avg_scale = (scale.X() + scale.Y()) / 2.0f;

            if (contact.pen_depth / avg_scale > ratio_threshold)
            {
                if (b->_pos_locked)
                    b->_transform->AddRootPosition(SVec3(
                        contact.normal * contact.pen_depth * damp_factor, 0
                    ));
                else
                    b->_transform->AddPositionWC(SVec3(
                        contact.normal * contact.pen_depth * damp_factor, 0
                    ));
            }
        }
    }

    KinematicManifold::KinematicManifold(ECS::RigidBody2D *a, ECS::RigidBody2D *b)
        : Manifold(a, b)
    {

    }

    void KinematicManifold::Solve(void)
    {
        const float DAMP_FACTOR = 0.99f;

        auto a_trans = a->GetTransform();
        auto b_trans = b->GetTransform();
        auto a_type = a->GetBodyType();
        auto b_type = b->GetBodyType();

        float RATIO = 1.0f;

        // split the pen depth in half if they are both being corrected simultaneously
        if (a_type == BODY_TYPE_KINEMATIC_SOLVE && b_type == BODY_TYPE_KINEMATIC_SOLVE)
            RATIO = 0.5f;

        // iterate over all contacts
        for (auto &contact : contacts)
        {
            // positionally correct based on the penetration depth
            if (a_type == BODY_TYPE_KINEMATIC_SOLVE)
                a_trans->AddRootPosition(
                    SVec3(-contact.normal * contact.pen_depth * DAMP_FACTOR * RATIO, 0)
                );

            if (b_type == BODY_TYPE_KINEMATIC_SOLVE)
                b_trans->AddRootPosition(
                    SVec3(contact.normal * contact.pen_depth * DAMP_FACTOR * RATIO, 0)
                ); 

        }

        // update both bodies so that they have relevant
        // information for the rest of this frame
        a->UpdateBody();
        b->UpdateBody();
    }

    ManifoldKey::ManifoldKey(ECS::EntityUniqueID a, ECS::EntityUniqueID b)
    {
        if (a < b)
        {
            this->a = a; this->b = b;
        }
        else
        {
            this->a = b; this->b = b;
        }
    }
}
