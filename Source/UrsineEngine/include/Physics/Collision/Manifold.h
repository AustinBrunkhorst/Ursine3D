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

#pragma once

#include "SimulationConfig.h"
#include "Vector2.h"
#include "Entity.h"

#include <vector>

namespace Ursine
{
    // Forward declaration
    namespace ECS
    {
        class RigidBody2D;
    }

    // This is used to cache the incident edges.
    // Needed for warm starting of the accumulated impulses.
    union ContactInfo
    {
        struct Edges
        {
            uint edge_a, edge_b;
        } e;

        double value;
    };

    struct Contact
    {
        Contact(void);

        // Points Of Contact world and model coordinates
        Vector2 point;

        // Information reguarding the incident edges
        ContactInfo info;

        // Contact normal (from A to B)
        Vector2 normal;

        // BodyA and bodyB's relative vectors
        Vector2 rel_a, rel_b;

        // Depth of penetration
        float pen_depth;

        // Accumulated normal impulse
        float norm_imp;

        // Accumulated tangent impulse
        float tang_imp;

        // Accumulated normal positional bias impulse
        float norm_bias_imp;

        float mass_norm, mass_tan;

        float bias;
    };

    typedef std::vector<Contact> Contacts;

    class Manifold
    {
    public:
        Manifold(ECS::RigidBody2D *a, ECS::RigidBody2D *b);

        // Merge the new contacts with the old
        void Update(Contacts &new_contacts);
            
        void PreStep(float inv_dt);

        void ApplyImpulse(void);

        Contacts contacts;

        ECS::RigidBody2D *a, *b;

        // Average restitution
        float e;

        // Friction
        float f;

    private:
        void positionalCorrection(const Contact &contact);
    };

    // This class is used specifically for kinematic solving non-iteratively
    class KinematicManifold : public Manifold
    {
    public:
        KinematicManifold(ECS::RigidBody2D *a, ECS::RigidBody2D *b);

        // Solves onces for the intersection of the two bodies.
        void Solve(void);
    };

    class ManifoldKey
    {
    public:
        ManifoldKey(ECS::EntityUniqueID a, ECS::EntityUniqueID b);
        inline bool operator<(const ManifoldKey &rhs) const
        {
            if (a < rhs.a)
                return true;

            if (a == rhs.a && b < rhs.b)
                return true;

            return false;
        }

        ECS::EntityUniqueID a, b;
    };

    typedef std::pair<ManifoldKey, Manifold> ManifoldPair;
}