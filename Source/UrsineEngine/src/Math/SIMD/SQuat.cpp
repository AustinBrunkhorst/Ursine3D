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

namespace ursine
{
	void SQuat::set(const SMat3& m)
	{
		// The rotation matrix is of form: (Eric Lengyel's Mathematics for 3D Game Programming and Computer Graphics 2nd ed., p. 92)
		// 1 - 2y^2 - 2z^2        2xy - 2wz            2xz + 2wy
		//    2xy + 2wz        1 - 2x^2 - 2z^2         2yz - 2wx
		//    2xz - 2wy           2yz + 2wx         1 - 2x^2 - 2y^2

		float r = m(0, 0) + m(1, 1) + m(2, 2); // The element w is easiest picked up as a sum of the diagonals.
											   // Above, r == 3 - 4(x^2+y^2+z^2) == 4(1-x^2-y^2-z^2) - 1 == 4*w^2 - 1.
		if (r > 0) // In this case, |w| > 1/2.
		{
			w = sqrt(r + 1.f) * 0.5f; // We have two choices for the sign of w, arbitrarily pick the positive.
			float inv4w = 1.f / (4.f * w);
			x = (m(2, 1) - m(1, 2)) * inv4w;
			y = (m(0, 2) - m(2, 0)) * inv4w;
			z = (m(1, 0) - m(0, 1)) * inv4w;
		}
		else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2)) // If |q.x| is larger than |q.y| and |q.z|, extract it first. This gives
		{                                                // best stability, and we know below x can't be zero.
			x = sqrt(1.f + m(0, 0) - m(1, 1) - m(2, 2)) * 0.5f; // We have two choices for the sign of x, arbitrarily pick the positive.
			const float x4 = 1.f / (4.f * x);
			y = (m(0, 1) + m(1, 0)) * x4;
			z = (m(0, 2) + m(2, 0)) * x4;
			w = (m(2, 1) - m(1, 2)) * x4;
		}
		else if (m(1, 1) > m(2, 2)) // |q.y| is larger than |q.x| and |q.z|
		{
			y = sqrt(1.f + m(1, 1) - m(0, 0) - m(2, 2)) * 0.5f; // We have two choices for the sign of y, arbitrarily pick the positive.
			const float y4 = 1.f / (4.f * y);
			x = (m(0, 1) + m(1, 0)) * y4;
			z = (m(1, 2) + m(2, 1)) * y4;
			w = (m(0, 2) - m(2, 0)) * y4;
		}
		else // |q.z| is larger than |q.x| or |q.y|
		{
			z = sqrt(1.f + m(2, 2) - m(0, 0) - m(1, 1)) * 0.5f; // We have two choices for the sign of z, arbitrarily pick the positive.
			const float z4 = 1.f / (4.f * z);
			x = (m(0, 2) + m(2, 0)) * z4;
			y = (m(1, 2) + m(2, 1)) * z4;
			w = (m(1, 0) - m(0, 1)) * z4;
		}
	}

	SQuat::SQuat( )
		: SQuat(0.0f, 0.0f, 0.0f, 1.0f)	{ }

	// Constructors
    SQuat::SQuat(float degrees, const SVec3 &axis)
    {
		SetAngleAxis(degrees, axis);
    }

    SQuat::SQuat(const SVec3& from, const SVec3& to)
    {
        SetFromTo(from, to);
    }

    SQuat::SQuat(float x_degrees, float y_degrees, float z_degrees)
    {
        SetEulerAngles(SVec3(x_degrees, y_degrees, z_degrees));
    }

    SQuat::SQuat(float X, float Y, float Z, float W)
        : SVec4(X, Y, Z, W) { }

	SQuat::SQuat(const SMat3 &rotationMatrix)
	{
		set(rotationMatrix);
	}

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

        return math::RadiansToDegrees(acos(Dot(other) / scalar)) * 2.0f;
    }

    SVec3 SQuat::GetAxis() const
    {
        float scalar_2 = 1.0f - w * w;

        if (math::IsZero(scalar_2))
            return SVec3(0.0f, 1.0f, 0.0f);
        
        float scalar = 1.0f / sqrt(scalar_2);

        return SVec3(x * scalar, y * scalar, z * scalar);
    }

    float SQuat::GetAngle() const
    {
        float angle = 2.0f * acos(w);

		return math::RadiansToDegrees(angle);
    }

	void SQuat::GetAngleAxis(float &degrees, SVec3 &axis) const
    {
        // angle
		degrees = math::RadiansToDegrees(2.0f * acos(w));

        // axis
        float scalar_2 = 1.0f - w * w;

        if (math::IsZero(scalar_2))
            axis.Set(0.0f, 1.0f, 0.0f);

        float scalar = 1.0f / sqrt(scalar_2);

        axis.Set(x * scalar, y * scalar, z * scalar);
    }

    void SQuat::SetAngleAxis(float degrees, const SVec3 &axis)
    {
		auto norm = SVec3::Normalize(axis);
		float radians_2 = math::DegreesToRadians(degrees) * 0.5f;
		auto s = sin(radians_2);
		auto c = cos(radians_2);

        Set(norm.X() * s,
			norm.Y() * s,
			norm.Z() * s,
            c);
    }


    void SQuat::SetEulerAngles(const SVec3 &euler)
    {
        auto half_y = math::DegreesToRadians(euler.Y() * 0.5f);
        auto half_x = math::DegreesToRadians(euler.X() * 0.5f);
        auto half_z = math::DegreesToRadians(euler.Z() * 0.5f);
        auto cos_y = cos(half_y);
        auto sin_y = sin(half_y);
        auto cos_x = cos(half_x);
        auto sin_x = sin(half_x);
        auto cos_z = cos(half_z);
        auto sin_z = sin(half_z);

        Set(cos_z * sin_x * cos_y + sin_z * cos_x * sin_y,
            cos_z * cos_x * sin_y - sin_z * sin_x * cos_y,
            sin_z * cos_x * cos_y - cos_z * sin_x * sin_y,
            cos_z * cos_x * cos_y + sin_z * sin_x * sin_y);
    }

	SVec3 SQuat::GetEulerAngles() const
	{
		// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

		float x_radians = atan2f(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
		float y_radians = asinf(2.0f * (w * y - z * x));
		float z_radians = atan2f(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));

		return {
			math::RadiansToDegrees(x_radians),
			math::RadiansToDegrees(y_radians),
			math::RadiansToDegrees(z_radians)
		};
	}

	void SQuat::SetFromTo(const SVec3& from, const SVec3& to)
    {
        // Source: http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors

        float norm_uv = sqrt(from.Dot(from) * to.Dot(to));
        float real_part = norm_uv + from.Dot(to);
        SVec3 v;

        if (real_part < math::Epsilon * norm_uv)
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
		SQuat result;

		Slerp(other, t, result);

		return result;
    }

    void SQuat::Slerp(const SQuat& other, float t, SQuat& result) const
    {
        float mag = sqrt(LengthSquared() * other.LengthSquared());

		UAssert(mag > 0.0f, "The magnitude cannot be zero.");

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
		SVec3 result;

		Rotate(vec, result);

		return result;
    }

    void SQuat::Rotate(const SVec3& vec, SVec3& result)
    {
		SVec3 qv(x, y, z);

		result = (2.0f * w) * (SVec3::Cross(qv, vec));
		result += ((w * w) - qv.Dot(qv)) * vec;
		result += (2.0f * (qv.Dot(vec))) * qv;
    }

	void SQuat::SetLookAt(const SVec3& targetDirection, const SVec3& localForward, const SVec3& localUp, const SVec3& worldUp)
	{
		SMat3 mat;

		mat.SetLookAt(targetDirection, localForward, localUp, worldUp);

		set(mat);
	}

	void SQuat::SetLookAt( const SVec3& targetDirection, const SVec3& worldUp )
	{
		SMat3 mat;

		mat.SetLookAt(targetDirection, worldUp);

		set(mat);
	}

	SQuat SQuat::LookAt(const SVec3& targetDirection, const SVec3& localForward, const SVec3& localUp, const SVec3& worldUp)
	{
		SMat3 mat;

		mat.SetLookAt(targetDirection, localForward, localUp, worldUp);

		return SQuat(mat);
	}

	SQuat SQuat::LookAt( const SVec3& targetDirection, const SVec3& worldUp )
	{
		SMat3 mat;

		mat.SetLookAt(targetDirection, worldUp);

		return SQuat(mat);
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

    SVec3 SQuat::operator*(const SVec3& rhs)
    {
		return Rotate(rhs);
    }
}
