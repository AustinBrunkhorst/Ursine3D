/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Ellipse.cpp
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
    Ellipse::Ellipse(void)
        : _rotation(0.0f)
        , _c(0.0f)
        , _s(0.0f)
    {
        type = ELLIPSE;
    }

    Ellipse::~Ellipse(void)
    {

    }

    Ellipse::Ellipse(const Ellipse &other)
        : _rotation(other._rotation)
        , _dimensions(other._dimensions)
        , _position(other._position)
    {
        type = ELLIPSE;
    }

    Ellipse *Ellipse::Clone(void) const
    {
        return new Ellipse(*this);
    }

    void Ellipse::Set(float rotation,
                        const Vector2 &dimensions,
                        const Vector2 &position)
    {
        _rotation = rotation;
        _dimensions = dimensions;
        _position = position;
    }

    AABB Ellipse::TransformShape(ECS::Transform &transform)
    {
        static const float FATNESS = 1.2f;
        AABB bound;

        _position = Vector2(transform.PositionWC());
        _dimensions = Vector2(transform.ScaleWC());
        _rotation = transform.RootRotation2D();

        // store the cosine and sine of the rotation
        Math::SinCos(_rotation, _s, _c);

        if (_dimensions.x == _dimensions.y)
        {
            bound.low_bound = _position - _dimensions;
            bound.up_bound = _position + _dimensions;
        }
        else
        {
            Vector2 extent = Vector2(
                GetSupport(Vector2(1, 0)).x,
                GetSupport(Vector2(0, 1)).y
            );

            Vector2 dimensions = (extent - _position) * FATNESS;

            bound.low_bound = _position - dimensions;
            bound.up_bound = _position + dimensions;
        }

        return bound;
    }

    Vector2 Ellipse::GetSupport(const Vector2 &direction) const
    {
        Vector2 support;

        // circle case
        if (Math::IsEqual(_dimensions.x, _dimensions.y))
        {
            float theta = direction.Angle()
                , s
                , c;

            Math::SinCos(theta, s, c);

            support.x = _dimensions.x * c;
            support.y = _dimensions.x * s;
        }
        // ellipse case
        else
        {
            float theta = direction.Angle() - _rotation
                , s
                , c;

            Math::SinCos(theta, s, c);

            support.x = _dimensions.x * c * _c - _dimensions.y * s * _s;
            support.y = _dimensions.x * c * _s + _dimensions.y * s * _c;
        }

        return _position + support;
    }

    void Ellipse::ComputeMass(ECS::Transform &transform, Vector2 &center,
                              float &area, float &inertia)
    {
        float ellipse_area = Math::PI * _dimensions.x * _dimensions.y;
        center += Vector2(transform.LocalPosition());
        area += ellipse_area;
        inertia += ellipse_area * (0.5f * _dimensions.x * _dimensions.y);
    }

    bool Ellipse::RayCast(RayCastOutput &output, const RayCastInput &input)
    {
        Matrix3 rot = Matrix3(-_rotation);
        Vector2 P1 = rot.TransformPoint(input.p1 - _position);
        Vector2 P2 = rot.TransformPoint(input.p2 - _position);
        Vector2 dir = P2 - P1;
        float DX2 = _dimensions.x * _dimensions.x;
        float DY2 = _dimensions.y * _dimensions.y;
        float A = ((dir.x * dir.x) / DX2) +
                    ((dir.y * dir.y) / DY2);
        float B = ((2 * P1.x * dir.x) / DX2) +
                    ((2 * P1.y * dir.y) / DY2);
        float C = ((P1.x * P1.x) / DX2) +
                    ((P1.y * P1.y) / DY2) - 1;

        float determ = (B * B) - (4 * A * C);

        if (determ < 0.0f)
            return false;
        else if (Math::IsEqual(determ, 0.0f))
        {
            float t = -B / (2 * A);

            if (t < 0 || t > input.max_fraction)
                return false;

            output.fraction = t;
        }
        else
        {
            // Two possibilities
            float t_0 = (-B + Math::fastSqrt(determ)) / (2 * A);
            float t_1 = (-B - Math::fastSqrt(determ)) / (2 * A);

            output.fraction = Math::Min(t_0, t_1);

            if (output.fraction > input.max_fraction)
                return false;
        }

        // calculate the normal
        Vector2 hit = (input.p1 + (input.p2 - input.p1) * output.fraction);
        output.normal = Vector2::Normalize(hit - _position);
        return true;
    }
}
