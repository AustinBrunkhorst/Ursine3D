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

#include "SIMD.h"
#include "UrsineTypes.h"
#include <string>
#include "SVec4.h"

namespace Ursine
{
    class SVec3;

    class SQuat : public SVec4
    {        
    public:
        // Constructors
        SQuat(float angle, const SVec3 &axis);
        SQuat(const SVec3 &from, const SVec3 &to);
        SQuat(float z_angle, float x_angle, float y_angle);
        SQuat(float X, float Y, float Z, float W);

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

        // Set the euler angle rotation
        void SetEulerAngles(float z_angle, float x_angle, float y_angle);

        // Represent a rotation FROM a vec -> TO a vec
        void SetFromTo(const SVec3 &from, const SVec3 &to);

        // Return the inverse of this quaternion
        SQuat GetInverse(void) const;

        // Spherical linear interpolation between this and the other quaternion.
        // if t == 0, the result is this.  if t == 1, the result is other.
        SQuat Slerp(const SQuat &other, float t) const;
        void Slerp(const SQuat &other, float t, SQuat &result) const;

        // Rotate a given vector by this quaternion
        SVec3 Rotate(const SVec3 &vec);
        void Rotate(const SVec3 &vec, SVec3 &result);

        // Create a quaternion that represents the specified forward and upward directions.
        // Apply the result to this quaternion (Z axis aligned with direction, Y axis aligned
        // with up, assuming the given vectors are orthogonal).
        // TODO: void SetLookAt(const SVec3 &direction, const SVec3 &up);

        // Create a quaternion that represents the specified forward and upward directions.
        // TODO: static SQuat LookAt(const SVec3 &direction, const SVec3 &up);

        // Rotates a rotation FROM -> TO by delta.  The rotation is never over shot.
        // Negative values of delta will move away from TO until the rotation is exactly
        // the opposite direction.
        // TODO: void RotateTowards(const SVec3 &from, const SVec3 &to, float delta);

        // Operators
        const SQuat &operator*=(const SQuat &q);
        SQuat operator*(const SQuat &rhs);
        SQuat operator*(const SVec3 &rhs);
    };
}
