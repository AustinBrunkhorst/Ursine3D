/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Simplex.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Vec2.h"
#include "Utils.h"

namespace Ursine
{
    struct SimplexEdge
    {
        float distance;
        Vec2 normal;
            
        // index into the EPA simplex (vector of simplexverts)
        size_t index_0, index_1;

        SimplexEdge()
            : distance(Math::MAX_FLOAT)
            , normal(Vec2::Zero())
        { }
    };

    class Simplex
    {
    public:
        class Vert
        {
        public:
            Vec2 parent_p0, parent_p1, vert;
        };

        int count;
        // store all simplex vertices
        Vert vertices[3];

        Simplex()
        {
            count = 0;
        }

        void Add(const Vert &vert)
        {
            vertices[count++] = vert;
        }

        const Vert &A()
        {
            return vertices[count - 1];
        }

        const Vert &B()
        {
            return vertices[count - 2];
        }

        const Vert &C()
        {
            return vertices[0];
        }

        void Reset(const Vert &A, const Vert &B)
        {
            vertices[0] = B;
            vertices[1] = A;
            count = 2;
        }

        void Reset(const Vert &A)
        {
            vertices[0] = A;
            count = 1;
        }

        void Reset()
        {
            count = 0;
        }

        bool ContainsOrigin(Vec2 &direction)
        {
            // Triangle case
            if (count == 3)
            {
                // compute the edges
                Vec2 AO = -A().vert;
                Vec2 AB = B().vert - A().vert;
                Vec2 AC = C().vert - A().vert;

                // Adjust the direction to be perpendicular to AB, pointing away from C
                direction.Set(-AB.Y(), AB.X());
                if (Vec2::SameDirection(direction, C().vert))
                    direction.Set(-direction.X(), -direction.Y());

                // If the perpendicular vector from the edge AB is heading towards the origin,
                // then we know that C is furthest from the origin and we can safely remove to
                // create a new simplex away from C
                if (Vec2::SameDirection(direction, AO))
                {
                    Reset(A(), B());
                    return false;
                }

                // Adjust the direction to be perpendicular to AC, pointing away from B
                direction.Set(-AC.Y(), AC.X());
                if (Vec2::SameDirection(direction, B().vert))
                    direction.Set(-direction.X(), -direction.Y());

                // if the perpendicular vector from the edge AC is heading towards the origin,
                // then we know that B is furthest from the origin and we can safely remove
                // to create a new simplex away from B
                if (Vec2::SameDirection(direction, AO))
                {
                    Reset(A(), C());
                    return false;
                }

                // If the perpendicular vectors generated from the edges of the triangle
                // do not point in the direction of the origin, then the origin must be
                // contained inside the triangle
                return true;
            }
            // Line case
            else
            {
                Vec2 AO = -A().vert;
                Vec2 AB = B().vert - A().vert;

                if (Vec2::SameDirection(AB, AO))
                {
                    direction = Vec2::TripleProduct(AB, AO, AB);
                }
                else
                {
                    Reset(A());
                    direction = AO;
                }

                // continue building the simplex
                return false;
            }
        }
    };
}
