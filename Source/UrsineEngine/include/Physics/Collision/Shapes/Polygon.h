/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Polygon.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include "BasePolygon.h"
#include "AABB.h"
#include "Shape.h"
#include "TransformComponent.h"

namespace Ursine
{
    class Edge
    {
    public:
        Edge(const Vector2 &point_0, const Vector2 &point_1, const Vector2 &normal);
        Edge(const Edge &other);

        Vector2 GetDirection();

        const Vector2 &p0, &p1;
        const Vector2 &norm;

    };

    class Polygon : public Shape
    {
    public:
        Polygon(void);
        Polygon(const Polygon &other);
        ~Polygon(void) override;

        Polygon *Clone(void) const override;

        void Set(const std::string &mesh_name);

        const std::string GetName(void) const;

        AABB TransformShape(ECS::Transform &transform) override;

        Vector2 GetSupport(const Vector2 &direction) const override;

        void ComputeMass(ECS::Transform &transform, Vector2 &center,
                            float &area, float &inertia) override;

        bool RayCast(RayCastOutput &output, const RayCastInput &input);

        // Each edge of the polygon
        std::vector<Edge> edges;

    private:
        void copy(const Polygon &other);
        void setEdges(bool reset);

        // this holds a reference to your base collider
        // in object space, NOT world!!! this is what
        // the verts and normals above are for.
        const BasePolygon *_base_polygon;

        std::vector<Vector2> _verts;   //[0][1] [1][2] [2][3] [3][0]  This is how the two arrays work together
        std::vector<Vector2> _normals; // [0]    [1]    [2]    [3]

        std::string _name;
    };
}