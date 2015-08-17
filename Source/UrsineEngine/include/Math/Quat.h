/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Quat.h
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
#include "Vec4.h"

namespace Ursine
{
    class Vec3;

    class Quat : public Vec4
    {        
    public:
        // Constructors
        Quat(float angle, const Vec3 &axis);
        Quat(const Vec3 &from, const Vec3 &to);
        Quat(float z_angle, float x_angle, float y_angle);
        Quat(float X, float Y, float Z, float W);

        // Properties
        static const Quat &Identity(void);

        // Public Methods

        // Return the HALF angle between this quaternion and the other
        float GetAngle(const Quat &other) const;

        float GetAngleShortestPath(void) const;

        // Return the shortest angle between this quaternion and the other
        float GetAngleShortestPath(const Quat &other);

        // Return the axis of rotation represented by this quaternion
        Vec3 GetAxis(void) const;

        // Return the angle of rotation represented by this quaternion
        float GetAngle(void) const;

        // Get the angle and axis
        void GetAngleAxis(float &angle, Vec3 &axis) const;

        // Represent a rotation around an axis by a certain amount
        void SetAngleAxis(float angle, const Vec3 &axis);

        // Set the euler angle rotation
        void SetEulerAngles(float z_angle, float x_angle, float y_angle);

        // Represent a rotation FROM a vec -> TO a vec
        void SetFromTo(const Vec3 &from, const Vec3 &to);

        // Return the inverse of this quaternion
        Quat GetInverse(void) const;

        // Spherical linear interpolation between this and the other quaternion.
        // if t == 0, the result is this.  if t == 1, the result is other.
        Quat Slerp(const Quat &other, float t) const;
        void Slerp(const Quat &other, float t, Quat &result) const;

        // Rotate a given vector by this quaternion
        Vec3 Rotate(const Vec3 &vec);
        void Rotate(const Vec3 &vec, Vec3 &result);

        // Create a quaternion that represents the specified forward and upward directions.
        // Apply the result to this quaternion (Z axis aligned with direction, Y axis aligned
        // with up, assuming the given vectors are orthogonal).
        // TODO: void SetLookAt(const Vec3 &direction, const Vec3 &up);

        // Create a quaternion that represents the specified forward and upward directions.
        // TODO: static Quat LookAt(const Vec3 &direction, const Vec3 &up);

        // Rotates a rotation FROM -> TO by delta.  The rotation is never over shot.
        // Negative values of delta will move away from TO until the rotation is exactly
        // the opposite direction.
        // TODO: void RotateTowards(const Vec3 &from, const Vec3 &to, float delta);

        // Operators
        const Quat &operator*=(const Quat &q);
        Quat operator*(const Quat &rhs);
        Quat operator*(const Vec3 &rhs);
    };
}
