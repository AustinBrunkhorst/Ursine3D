/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Polygon.cpp
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
    Polygon::Polygon()
    {
        type = POLYGON;
    }

    Polygon::~Polygon()
    {

    }

    Polygon::Polygon(const Polygon &other)
        : _base_polygon(other._base_polygon)
    {
        type = POLYGON;
        copy(other);
    }

    Polygon *Polygon::Clone(void) const
    {
        return new Polygon(*this);
    }

    void Polygon::Set(const std::string &mesh_name)
    {
        _base_polygon = gContentManager->Load<BasePolygon>(mesh_name);

        _name = mesh_name;

        _verts = _base_polygon->vertices;
        _normals = _base_polygon->vertices;

        setEdges(true);
    }

    const std::string Polygon::GetName(void) const
    {
        return _name;
    }

    // return the encompassing AABB of the shape
    AABB Polygon::TransformShape(ECS::Transform &transform)
    {
        SMat3 normal_mat = transform.MatrixNoScalar();
        SMat3 vert_mat = transform.Matrix();

        // Find the farthest x and y value to make our box with
        AABB bounds;
        bounds.low_bound.Set(std::numeric_limits<float>().max(),
                                std::numeric_limits<float>().max());
        bounds.up_bound.Set(-std::numeric_limits<float>().max(),
                            -std::numeric_limits<float>().max());

        // transform the vertices
        for (uint i = 0; i < _verts.size(); ++i)
        {
            _verts[i] = vert_mat.TransformPoint(_base_polygon->vertices[i]);

            if (_verts[i].X() > bounds.up_bound.X())
                bounds.up_bound.X() = _verts[i].X();
            if (_verts[i].Y() > bounds.up_bound.Y())
                bounds.up_bound.Y() = _verts[i].Y();
            if (_verts[i].X() < bounds.low_bound.X())
                bounds.low_bound.X() = _verts[i].X();
            if (_verts[i].Y() < bounds.low_bound.Y())
                bounds.low_bound.Y() = _verts[i].Y();
        }

        // transform the normals
        for (Uint32 i = 0; i < _normals.size(); ++i)
            _normals[i] = normal_mat.TransformVector(_base_polygon->normals[i]);

        return bounds;
    }

    Vec2 Polygon::GetSupport(const Vec2 &direction) const
    {
        int best_vert_index = 0;
        float largest_projection = -Math::MAX_FLOAT;

        for (int i = 0, size = _verts.size(); i < size; ++i)
        {
            float projection = Vec2::Dot(_verts[i], direction);
            if (projection > largest_projection)
            {
                largest_projection = projection;
                best_vert_index = i;
            }
        }

        return _verts[best_vert_index];
    }

    void Polygon::ComputeMass(ECS::Transform &transform, Vec2 &center,
                              float &area, float &inertia)
    {
        static const float inv3 = 1.0f / 3.0f;
        std::vector<Vec2> verts;
        Vec2 scale = Vec2(transform.ScaleWC());
        SMat3 scale_mat = SMat3(scale.X(), scale.Y());

        // scale the object space vertices
        for (auto &vert : _base_polygon->vertices)
            verts.push_back(scale_mat.TransformPoint(vert));

        Vec2 origin;
        Vec2 collider_center;
        float collider_area = 0;

        // This puts the origin inside of the shape.
        // Cannot use 0, 0 because not all colliders contain the origin
        for (int i = 0, size = verts.size(); i < size; ++i)
            origin += verts[i];
        origin = origin * (1.0f / verts.size());

        // calculate the mass based verts in object space
        for (uint i = 0, size = verts.size(); i < size; ++i)
        {
            // Triangle vertices, third vertex is the center (0, 0)
            // moving the verts centered around the object's position 
            // to be centered around the origin.
            Vec2 point_1(verts[i] - origin);
            uint j = i + 1 < size ? i + 1 : 0;
            Vec2 point_2(verts[j] - origin);

            float D = Vec2::Cross(point_1, point_2);
            float A = 0.5f * D;

            collider_area += A;

            collider_center += A * inv3 * (point_1 + point_2);

            float intx2 = point_1.X() * point_1.X() + point_2.X() * point_1.X() + point_2.X() * point_2.X();
            float inty2 = point_1.Y() * point_1.Y() + point_2.Y() * point_1.Y() + point_2.Y() * point_2.Y();

            inertia += (0.25f * inv3 * D) * (intx2 + inty2);
        }

        collider_center = collider_center * (1.0f / collider_area);
        collider_center += origin;

        center += collider_center;
        area += collider_area;
    }

    void Polygon::copy(const Polygon &other)
    {
        _verts = other._verts;
        _normals = other._normals;
        setEdges(true);
    }

    void Polygon::setEdges(bool reset)
    {
        if (reset)
            edges.clear();

        for (Uint32 i = 0; i < _normals.size(); ++i)
        {
            if (i == _normals.size() - 1)
                edges.push_back(Edge(_verts[i], _verts[0], _normals[i]));
            else
                edges.push_back(Edge(_verts[i], _verts[i + 1], _normals[i]));
        }
    }

    bool Polygon::RayCast(RayCastOutput &output, const RayCastInput &input)
    {
        float t = Math::MAX_FLOAT;
        int index = -1;

        for (unsigned i = 0, size = edges.size(); i < size; ++i)
        {
            float temp = Math::MAX_FLOAT;

            if (Collision::RaySegmentIntersection(input, edges[i].p0, edges[i].p1, temp))
            {
                if (temp < t)
                {
                    t = temp;
                    index = i;
                }
            }
        }

        if (index >= 0)
        {
            output.fraction = t;
            output.normal = edges[index].norm;
            return true;
        }

        return false;
    }

    Edge::Edge(const Vec2 &point_0, const Vec2 &point_1, const Vec2 &normal)
        : p0(point_0)
        , p1(point_1)
        , norm(normal)
    {

    }

    Edge::Edge(const Edge &other)
        : p0(other.p0)
        , p1(other.p1)
        , norm(other.norm)
    {

    }

    Vec2 Edge::GetDirection()
    {
        return this->p1 - this->p0;
    }
}
