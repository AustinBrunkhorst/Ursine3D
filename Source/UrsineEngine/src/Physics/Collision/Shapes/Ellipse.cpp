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
                        const Vec2 &dimensions,
                        const Vec2 &position)
    {
        _rotation = rotation;
        _dimensions = dimensions;
        _position = position;
    }

    AABB Ellipse::TransformShape(ECS::Transform &transform)
    {
        static const float FATNESS = 1.2f;
        AABB bound;

        _position = Vec2(transform.PositionWC());
        _dimensions = Vec2(transform.ScaleWC());
        _rotation = transform.RootRotation2D();

        // store the cosine and sine of the rotation
        Math::SinCos(_rotation, _s, _c);

        if (_dimensions.X() == _dimensions.Y())
        {
            bound.low_bound = _position - _dimensions;
            bound.up_bound = _position + _dimensions;
        }
        else
        {
            Vec2 extent = Vec2(
                GetSupport(Vec2(1, 0)).X(),
                GetSupport(Vec2(0, 1)).Y()
            );

            Vec2 dimensions = (extent - _position) * FATNESS;

            bound.low_bound = _position - dimensions;
            bound.up_bound = _position + dimensions;
        }

        return bound;
    }

    Vec2 Ellipse::GetSupport(const Vec2 &direction) const
    {
        Vec2 support;

        // circle case
        if (Math::IsEqual(_dimensions.X(), _dimensions.Y()))
        {
            float theta = direction.Angle()
                , s
                , c;

            Math::SinCos(theta, s, c);

            support.X() = _dimensions.X() * c;
            support.Y() = _dimensions.X() * s;
        }
        // ellipse case
        else
        {
            float theta = direction.Angle() - _rotation
                , s
                , c;

            Math::SinCos(theta, s, c);

            support.X() = _dimensions.X() * c * _c - _dimensions.Y() * s * _s;
            support.Y() = _dimensions.X() * c * _s + _dimensions.Y() * s * _c;
        }

        return _position + support;
    }

    void Ellipse::ComputeMass(ECS::Transform &transform, Vec2 &center,
                              float &area, float &inertia)
    {
        float ellipse_area = Math::PI * _dimensions.X() * _dimensions.Y();
        center += Vec2(transform.LocalPosition());
        area += ellipse_area;
        inertia += ellipse_area * (0.5f * _dimensions.X() * _dimensions.Y());
    }

    bool Ellipse::RayCast(RayCastOutput &output, const RayCastInput &input)
    {
        SMat3 rot = SMat3(-_rotation);
        Vec2 P1 = rot.TransformPoint(input.p1 - _position);
        Vec2 P2 = rot.TransformPoint(input.p2 - _position);
        Vec2 dir = P2 - P1;
        float DX2 = _dimensions.X() * _dimensions.X();
        float DY2 = _dimensions.Y() * _dimensions.Y();
        float A = ((dir.X() * dir.X()) / DX2) +
                    ((dir.Y() * dir.Y()) / DY2);
        float B = ((2 * P1.X() * dir.X()) / DX2) +
                    ((2 * P1.Y() * dir.Y()) / DY2);
        float C = ((P1.X() * P1.X()) / DX2) +
                    ((P1.Y() * P1.Y()) / DY2) - 1;

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
        Vec2 hit = (input.p1 + (input.p2 - input.p1) * output.fraction);
        output.normal = Vec2::Normalize(hit - _position);
        return true;
    }
}
