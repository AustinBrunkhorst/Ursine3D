/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PolygonCollisionSystem.cpp
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
    void Support(const Collider &collider_A, const Collider &collider_B, const Vector2 &direction,
                 Vector2 &support, Vector2 &support_A, Vector2 &support_B)
    {
        const Shape &polygon_a = *collider_A.shape;
        const Shape &polygon_b = *collider_B.shape;

        support_A = polygon_a.GetSupport(direction);
        support_B = polygon_b.GetSupport(-direction);

        // perform the minkowski difference
        support = support_A - support_B;
    }

    Simplex::Vert Support(const Collider &collider_A, const Collider &collider_B,
        const Vector2 &direction)
    {
        Vector2 support_A = collider_A.shape->GetSupport(direction);
        Vector2 support_B = collider_B.shape->GetSupport(-direction);
        Vector2 vert = support_A - support_B;

        return{ support_A, support_B, vert };
    }

    SimplexEdge EPAFindClosestEdge(const std::vector<Simplex::Vert> &epa_simplex)
    {
        SimplexEdge closest;

        for (uint i = 0; i < epa_simplex.size(); ++i)
        {
            // i is the current point in the simplex, and j is the next
            // this is so we can connect back to the starting point
            size_t j = (i + 1 == epa_simplex.size() ? 0 : i + 1);

            const Vector2 &A = epa_simplex[i].vert;
            const Vector2 &B = epa_simplex[j].vert;

            // find our edge
            Vector2 direction = B - A;

            // find the normal of the edge
            Vector2 N(-direction.y, direction.x);
            N.Normalize();

            // distance between the origin and the normal
            // (a is the vector between origin and a since: a - origin = a - [0,0] = a
            float distance = Vector2::Dot(A, N);

            // if d is less than the previously closest
            if (distance < closest.distance)
            {
                closest.distance = distance;
                closest.normal = N;
                closest.index_0 = i;
                closest.index_1 = j;
            }
        }

        return closest;
    }

    void EPAHandle(const Collider &collider_a, const Collider &collider_b,
        const Simplex &simplex, Contacts &contacts)
    {
        std::vector<Simplex::Vert> epa_simplex
            = { simplex.vertices[0], simplex.vertices[1], simplex.vertices[2] };

        // 100 is for security purposes, preventing an infinite loop
        for (int i = 0; i < 100; ++i)
        {
            // find the edge closest to the origin in the simplex
            SimplexEdge edge = EPAFindClosestEdge(epa_simplex);

            // find the furthest minkowski difference point in the direction of the normal
            Vector2 support, support_A, support_B;
            Support(collider_a, collider_b, edge.normal,
                support, support_A, support_B);

            // find the distance between the point and the edge
            float dist = Vector2::Dot(support, edge.normal);

            // if we've hit the border of the minkowski difference
            if (dist - edge.distance < 0.01f)
            {
                contacts.push_back(Contact());
                auto &contact = contacts.back();

                contact.normal = edge.normal;
                contact.pen_depth = dist;

                // For contact points
                // Find the repeating point, if there is no repeating point,
                // project onto the edge, and use the homogenous coords to find the contact point.

                Simplex::Vert &vert_0 = epa_simplex[edge.index_0],
                    &vert_1 = epa_simplex[edge.index_1];

                // if there are no repitions, we project the origin onto the edge
                // to find the contact point
                float t;

                // get the interval from the x coordinates
                if (vert_0.vert.x > 0 && vert_1.vert.x < 0 ||
                    vert_0.vert.x < 0 && vert_1.vert.x > 0)
                {
                    t = (-vert_0.vert.x) / (vert_1.vert.x - vert_0.vert.x);
                }
                // get the interval from the y coordinates
                else
                {
                    t = (-vert_0.vert.y) / (vert_1.vert.y - vert_0.vert.y);
                }

                contact.point = vert_0.parent_p0 + t * (vert_1.parent_p0 - vert_0.parent_p0);

                return;
            }
            else
            {
                // add the point inbetween the points where it was found
                epa_simplex.insert(epa_simplex.begin() + edge.index_1,
                { support_A, support_B, support });
            }
        }
    }

    bool IsColliding(const Collider &collider_a, const Collider &collider_b, Contacts &contacts)
    {
        // quick check with the individual collider AABBs for a quick out
        if (!collider_a.aabb.Overlap(collider_b.aabb))
            return false;

        // our simplex for this collision test
        Simplex simplex;

        // Set initial search direction to the difference of centers
        Vector2 d = Vector2(1, -1);//Vector2(collider_b.root_trans.PositionWC() - collider_a.root_trans.PositionWC());

        // get the first minkowski difference point
        simplex.Add(Support(collider_a, collider_b, d));

        // negate the support point, giving us a vector in the direction of the origin
        d = -simplex.A().vert;

        int count = 0;
        // start looping
        while (count < 100)
        {
            // add a new point to the simplex because we haven't terminated yet
            simplex.Add(Support(collider_a, collider_b, d));

            // see if the simplex is on the correct side of the origin
            if (Vector2::OppositeDirection(simplex.A().vert, d))
            {
                // if the point added last was not past the origin in the direction of d
                // then the Minkowski Sum cannot possibly contain the origin since
                // the last point added is on the edge of the Minkowski Difference
                return false;
            }
            else
            {
                // oterwise we need to determine if the origin is in the current simplex
                // this function will set the next search direction for us if it fails.
                if (simplex.ContainsOrigin(d))
                {
                    // if it does then we know there is a collision

                    // handle the collision with the EPA algorithm
                    EPAHandle(collider_a, collider_b, simplex, contacts);

                    // find the incident edge.
                    if (contacts.size())
                    {
                        auto &it = contacts.back();
                        it.info.e.edge_a = collider_a.FindIndex(it.normal);
                        it.info.e.edge_b = collider_b.FindIndex(-it.normal);
                    }

                    return true;
                }
            }

            ++count;
        }

        return false;
    }

    void Collision::SetCollisionFalse(Colliders &colliders)
    {
        // recursively set all collider's collision flags to false
        for (auto &collider : colliders)
        {
            SetCollisionFalse(collider->children);
            collider->is_colliding = false;
        }
    }

    void Collision::compareColliders(Collider *collider_a, ECS::RigidBody2D *a,
                                     Collider *collider_b, ECS::RigidBody2D *b,
                                     Contacts &contacts)
    {
        if (IsColliding(*collider_a, *collider_b, contacts))
        {
            collider_a->is_colliding = collider_b->is_colliding = true;
        }
    }

    void Collision::compareColliderAndTree(Collider *collider, ECS::RigidBody2D *a,
                                           Colliders &tree, ECS::RigidBody2D *b,
                                           Contacts &contacts)
    {
        // recursively check the collider with each branch in the given tree
        for (auto &node : tree)
        {
            compareColliderAndTree(collider, a, node->children, b, contacts);
            compareColliders(collider, a, node, b, contacts);
        }
    }

    void Collision::handleNarrowPhase(Colliders &tree_a, ECS::RigidBody2D *a, 
                                        Colliders &tree_b, ECS::RigidBody2D *b,
                                        Contacts &contacts)
    {
        // recursively check each branch with every other branch
        for (auto &node : tree_a)
        {
            handleNarrowPhase(node->children, a, tree_b, b, contacts);
            compareColliderAndTree(node, a, tree_b, b, contacts);
        }
    }

    void Collision::Collide(Contacts &contacts, ECS::RigidBody2D *a, ECS::RigidBody2D *b)
    {
        // check the broad phase of both rigid bodies
        if (a->_aabb.Overlap(b->_aabb))
        {
            handleNarrowPhase(a->colliders, a, b->colliders, b, contacts);
        }
    }

    bool Collision::RaySegmentIntersection(const RayCastInput &input,
                                           const Vector2 &p_0, const Vector2 &p_1,
                                           float &t)
    {
        Vector2 r_d = input.p2 - input.p1;
        Vector2 r_p = input.p1;
        Vector2 s_d = p_1 - p_0;
        Vector2 s_p = p_0;

        float t_2 = (r_d.x * (s_p.y - r_p.y) + r_d.y * (r_p.x - s_p.x)) / (s_d.x * r_d.y - s_d.y * r_d.x);

        if (t_2 <= 0.0f || t_2 >= 1.0f)
            return false;

        float t_1 = (s_p.x + s_d.x * t_2 - r_p.x) / r_d.x;

        if (t_1 > 0.0f && t_1 <= input.max_fraction)
        {
            t = t_1;
            return true;
        }
        else
            return false;
    }

    void Collision::SendEvents(ECS::RigidBody2D *a, ECS::RigidBody2D *b,
                               Manifold &manifold, CollisionEvents event)
    {
        CollisionEventArgs e;
            
        if (event != COLLISION_ENDED)
            e.at = manifold.contacts[0].point;

        ECS::Entity *e_a = a->GetOwner();
        ECS::Entity *e_b = b->GetOwner();

        if (e_a && e_b && e_a->IsAvailable() && e_b->IsAvailable() &&
            a->sends_events && b->sends_events)
        {
            e.other = e_b;
            e_a->Dispatch(event, &e);

            e.other = e_a;
            e_b->Dispatch(event, &e);
        }
    }
}