/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Quat.cpp
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
    // Constructors
    SQuat::SQuat(float angle, const SVec3 &axis)
    {
        SetAngleAxis(angle, axis);
    }

    SQuat::SQuat(const SVec3& from, const SVec3& to)
    {
        SetFromTo(from, to);
    }

    SQuat::SQuat(float z_angle, float x_angle, float y_angle)
    {
        SetEulerAngles(z_angle, x_angle, y_angle);
    }

    SQuat::SQuat(float X, float Y, float Z, float W)
        : SVec4(X, Y, Z, W) { }

    // Properties
    const SQuat &SQuat::Identity(void)
    {
        static const SQuat identity(0.0f, 0.0f, 0.0f, 1.0f);

        return identity;
    }

    // Public Methods
    float SQuat::GetAngle(const SQuat &other) const
    {
        float scalar = sqrt(LengthSquared() * other.LengthSquared());

        // TODO: UAssert(scalar != 0.0f)

        return acos(Dot(other) / scalar);
    }

    float SQuat::GetAngleShortestPath(void) const
    {
        float angle;

        if (LengthSquared() < 0.0f)
            angle = 2.0f * acos(w);
        else
            angle = 2.0f * acos(-w);

        return angle;
    }

    float SQuat::GetAngleShortestPath(const SQuat& other)
    {
        float angle = sqrt(LengthSquared() * other.LengthSquared());

        // TODO: UAssert(angle != 0.0f);

        if (Dot(other) < 0)
            return acos(Dot(-other) / angle) * 2.0f;
        else
            return acos(Dot(other) / angle) * 2.0f;
    }

    SVec3 SQuat::GetAxis() const
    {
        float scalar_2 = 1.0f - w * w;

        if (Math::IsZero(scalar_2))
            return SVec3(0.0f, 1.0f, 0.0f);
        
        float scalar = 1.0f / sqrt(scalar_2);

        return SVec3(x * scalar, y * scalar, z * scalar);
    }

    float SQuat::GetAngle() const
    {
        float angle = 2.0f * acos(w);

        return angle;
    }

    void SQuat::GetAngleAxis(float &angle, SVec3 &axis) const
    {
        // angle
        angle = 2.0f * acos(w);

        // axis
        float scalar_2 = 1.0f - w * w;

        if (Math::IsZero(scalar_2))
            axis.Set(0.0f, 1.0f, 0.0f);

        float scalar = 1.0f / sqrt(scalar_2);

        axis.Set(x * scalar, y * scalar, z * scalar);
    }

    void SQuat::SetAngleAxis(float angle, const SVec3 &axis)
    {
        float s = sin(0.5f * angle);
        float c = cos(0.5f * angle);

        Set(axis.X() * s,
            axis.Y() * s,
            axis.Z() * s,
            c);
    }


    void SQuat::SetEulerAngles(float z_angle, float x_angle, float y_angle)
    {
        float half_y = y_angle * 0.5f;
        float half_x = x_angle * 0.5f;
        float half_z = z_angle * 0.5f;
        float cos_y = cos(half_y);
        float sin_y = sin(half_y);
        float cos_x = cos(half_x);
        float sin_x = sin(half_x);
        float cos_z = cos(half_z);
        float sin_z = sin(half_z);

        Set(cos_z * sin_x * cos_y + sin_z * cos_x * sin_y,
            cos_z * cos_x * sin_y - sin_z * sin_x * cos_y,
            sin_z * cos_x * cos_y - cos_z * sin_x * sin_y,
            cos_z * cos_x * cos_y + sin_z * sin_x * sin_y);
    }

    void SQuat::SetFromTo(const SVec3& from, const SVec3& to)
    {
        // Source: http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors

        float norm_uv = sqrt(from.Dot(from) * to.Dot(to));
        float real_part = norm_uv + from.Dot(to);
        SVec3 v;

        if (real_part < Math::Epsilon * norm_uv)
        {
            // If from and to are exactly opposite, rotate 180 degrees
            // around an arbitrary orthogonal axis.  Normalization happens later.
            real_part = 0.0f;
            v = abs(from.X()) > abs(from.Z()) ? SVec3(-from.Y(), from.X(), 0.0f)
                                              : SVec3(0.0f, -from.Z(), from.Y());
        }
        else
        {
            v = SVec3::Cross(from, to);
        }

        Set(v.X(), v.Y(), v.Z(), real_part);
        Normalize();
    }

    SQuat SQuat::GetInverse() const
    {
        return SQuat(-x, -y, -z, w);
    }

    SQuat SQuat::Slerp(const SQuat& other, float t) const
    {
        float mag = sqrt(LengthSquared() * other.LengthSquared());

        // TODO: UAssert(magnitude > 0.0f);

        float product = Dot(other) / mag;

        if (fabsf(product) < 1.0f)
        {
            // take care of long angle case: http://en.wikipedia.org/wiki/Slerp
            float sign = (product < 0.0f) ? -1.0f : 1.0f;

            float theta = acos(sign * product);
            float s1 = sin(sign * t * theta);
            float d = 1.0f / sin(theta);
            float s0 = sin((1.0f - t) * theta);

            return SQuat(
                (x * s0 + other.X() * s1) * d,
                (y * s0 + other.Y() * s1) * d,
                (z * s0 + other.Z() * s1) * d,
                (w * s0 + other.W() * s1) * d
            );
        }
        else
            return *this;
    }

    void SQuat::Slerp(const SQuat& other, float t, SQuat& result) const
    {
        float mag = sqrt(LengthSquared() * other.LengthSquared());

        // TODO: UAssert(magnitude > 0.0f);

        float product = Dot(other) / mag;

        if (fabsf(product) < 1.0f)
        {
            // take care of long angle case: http://en.wikipedia.org/wiki/Slerp
            float sign = (product < 0.0f) ? -1.0f : 1.0f;

            float theta = acos(sign * product);
            float s1 = sin(sign * t * theta);
            float d = 1.0f / sin(theta);
            float s0 = sin((1.0f - t) * theta);

            result.Set(
                (x * s0 + other.X() * s1) * d,
                (y * s0 + other.Y() * s1) * d,
                (z * s0 + other.Z() * s1) * d,
                (w * s0 + other.W() * s1) * d
            );
        }
        else
            result = *this;
    }

    SVec3 SQuat::Rotate(const SVec3 &vec)
    {
        SQuat q = *this * vec;

        q *= GetInverse();

        return SVec3(q.X(), q.Y(), q.Z());
    }

    void SQuat::Rotate(const SVec3& vec, SVec3& result)
    {
        SQuat q = *this * vec;

        q *= GetInverse();

        result.Set(q.X(), q.Y(), q.Z());
    }

    // Operators
    const SQuat &SQuat::operator*=(const SQuat &q)
    {
        Set(
            w * q.X() + x * q.W() + y * q.Z() - z * q.Y(),
            w * q.Y() + y * q.W() + z * q.X() - x * q.Z(),
            w * q.Z() + z * q.W() + x * q.Y() - y * q.X(),
            w * q.W() - x * q.X() - y * q.Y() - z * q.Z()
        );

        return *this;
    }

    SQuat SQuat::operator*(const SQuat &rhs)
    {
        return SQuat(
            w * rhs.X() + x * rhs.W() + y * rhs.Z() - z * rhs.Y(),
            w * rhs.Y() + y * rhs.W() + z * rhs.X() - x * rhs.Z(),
            w * rhs.Z() + z * rhs.W() + x * rhs.Y() - y * rhs.X(),
            w * rhs.W() - x * rhs.X() - y * rhs.Y() - z * rhs.Z()
        );
    }

    SQuat SQuat::operator*(const SVec3& rhs)
    {
        return SQuat(
            w * rhs.X() + y * rhs.Z() - z * rhs.Y(),
            w * rhs.Y() + z * rhs.X() - x * rhs.Z(),
            w * rhs.Z() + x * rhs.Y() - y * rhs.X(),
           -x * rhs.X() - y * rhs.Y() - z * rhs.Z()
        );
    }

}