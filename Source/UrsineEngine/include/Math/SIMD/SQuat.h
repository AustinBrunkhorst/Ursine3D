/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** SQuat.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SIMD.h"
#include "UrsineTypes.h"
#include <string>
#include "SVec4.h"

namespace Ursine
{
    class SVec3;
	class SMat4;

    class SQuat : public SVec4
    {        
		friend class SMat4;

	private:
		void set(const SMat3 &mat);

    public:
        // Constructors
        SQuat(float angle, const SVec3 &axis);
        SQuat(const SVec3 &from, const SVec3 &to);
        SQuat(float x_angle, float y_angle, float z_angle);
        SQuat(float X, float Y, float Z, float W);
		explicit SQuat(const SMat3 &rotationMatrix);

        // Properties
        static const SQuat &Identity(void);

        // Public Methods

        // Return the HALF angle between this quaternion and the other
        float GetAngle(const SQuat &other) const;

        float GetAngleShortestPath(void) const;

        // Return the shortest angle between this quaternion and the other
        float GetAngleShortestPath(const SQuat &other);

        // Return the axis of rotation represented by this quaternion
        SVec3 GetAxis(void) const;

        // Return the angle of rotation represented by this quaternion
        float GetAngle(void) const;

        // Get the angle and axis
        void GetAngleAxis(float &angle, SVec3 &axis) const;

        // Represent a rotation around an axis by a certain amount
        void SetAngleAxis(float angle, const SVec3 &axis);

        // Set the euler angle rotation (X, Y, Z)
		void SetEulerAngles(const SVec3 &euler);;

		// Get the euler angles this rotation represents (X, Y, Z)
		SVec3 GetEulerAngles(void) const;

        // Represent a rotation FROM a vec -> TO a vec
        void SetFromTo(const SVec3 &from, const SVec3 &to);

        // Return the inverse of this quaternion
        SQuat GetInverse(void) const;

        // Spherical linear interpolation between this and the other quaternion.
        // if t == 0, the result is this.  if t == 1, the result is other.
        SQuat Slerp(const SQuat &other, float t) const;
        void Slerp(const SQuat &other, float t, SQuat &result) const;

		// Spherical linear interpolation betweeen two given vectors
		// if t == 0, the result is from, if t == 1, the result is to.
		static SVec3 SlerpVector(const SVec3 &from, const SVec3 &to, float t);

        // Rotate a given vector by this quaternion
        SVec3 Rotate(const SVec3 &vec);
        void Rotate(const SVec3 &vec, SVec3 &result);

        // A LookAt quaternion is a quaternion that orients an object to face towards a specified target direction.
		void SetLookAt(const SVec3 &targetDirection, const SVec3 &localForward, const SVec3 &localUp, const SVec3 &worldUp);

        // Create a LookAt quaternion
        static SQuat LookAt(const SVec3 &targetDirection, const SVec3 &localForward, const SVec3 &localUp, const SVec3 &worldUp);

        // Operators
        const SQuat &operator*=(const SQuat &q);
        SQuat operator*(const SQuat &rhs);
        SVec3 operator*(const SVec3 &rhs);

		ALLOW_ALIGNED_ALLOC(16)
    };
}
