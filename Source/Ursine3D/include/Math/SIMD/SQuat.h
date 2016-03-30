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
#include <string>
#include "SVec4.h"
#include "SVec3.h"

namespace ursine
{
    class SVec3;
    class SMat4;
    class SMat3;

    class SQuat : public SVec4
    {
        friend class SMat4;

    public:
        Meta(ExplicitGetter( "(float (ursine::SVec4::*)(void) const) &ursine::SVec4::X" ))
        EditorField(float x, X, SetX);

        Meta(ExplicitGetter( "(float (ursine::SVec4::*)(void) const) &ursine::SVec4::Y" ))
        EditorField(float y, Y, SetY);

        Meta(ExplicitGetter( "(float (ursine::SVec4::*)(void) const) &ursine::SVec4::Z" ))
        EditorField(float z, Z, SetZ);

        Meta(ExplicitGetter( "(float (ursine::SVec4::*)(void) const) &ursine::SVec4::W" ))
        EditorField(float w, W, SetW);

        // Constructors
        Meta(Enable)
        SQuat(void);
        SQuat(float degrees, const SVec3 &axis);
        SQuat(const SVec3 &from, const SVec3 &to);
        SQuat(float x_degrees, float y_degrees, float z_degrees);
        SQuat(float X, float Y, float Z, float W);
        explicit SQuat(const SMat3 &rotationMatrix);

        // Properties
        static const SQuat &Identity(void);

        // Public Methods

        // Return the angle between this quaternion and the other in degrees
        float GetAngle(const SQuat &other) const;

        // Return the axis of rotation represented by this quaternion
        SVec3 GetAxis(void) const;

        // Return the angle of rotation represented by this quaternion in degrees
        float GetAngle(void) const;

        // Get the angle and axis
        void GetAngleAxis(float &degrees, SVec3 &axis) const;

        // Represent a rotation around an axis by a certain amount in degrees
        void SetAngleAxis(float degrees, const SVec3 &axis);

        // Set the euler angle rotation (X, Y, Z) in degrees
        void SetEulerAngles(const SVec3 &euler);

        // Get the euler angles this rotation represents (X, Y, Z) in degrees
        SVec3 GetEulerAngles(void) const;

        // Represent a rotation FROM a vec -> TO a vec
        void SetFromTo(const SVec3 &from, const SVec3 &to);

        // Return the inverse of this quaternion
        SQuat GetInverse(void) const;

        // Spherical linear interpolation between this and the other quaternion.
        // if t == 0, the result is this.  if t == 1, the result is other.
        SQuat Slerp(const SQuat &other, float t) const;
        void Slerp(const SQuat &other, float t, SQuat &result) const;

        // Rotate a given vector by this quaternion
        SVec3 Rotate(const SVec3 &vec) const;
        void Rotate(const SVec3 &vec, SVec3 &result) const;

        // A LookAt quaternion is a quaternion that orients an object to face towards a specified target direction.
        void SetLookAt(const SVec3 &targetDirection, const SVec3 &localForward, const SVec3 &localUp, const SVec3 &worldUp);
        void SetLookAt(const SVec3 &targetDirection, const SVec3 &worldUp = SVec3::UnitY( ));

        // Create a LookAt quaternion
        static SQuat LookAt(const SVec3 &targetDirection, const SVec3 &localForward, const SVec3 &localUp, const SVec3 &worldUp);
        static SQuat LookAt(const SVec3 &targetDirection, const SVec3 &worldUp = SVec3::UnitY( ));

        // Operators
        const SQuat &operator*=(const SQuat &q);
        SQuat operator*(const SQuat &rhs) const;
        SQuat operator*(float rhs) const;
        SVec3 operator*(const SVec3 &rhs) const;
        SQuat operator+(const SQuat &rhs) const;

        ALLOW_ALIGNED_ALLOC(16)

    private:
        void set(const SMat3 &mat);
    } Meta(Enable, WhiteListMethods);
}
