/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** SMat3.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SVec3.h"
#include "Vec2.h"

namespace Ursine
{
	// Forward Declaration
    class SMat4;
	class Mat4;
	class Mat3;
	class SQuat;

    ALIGNED16(class) SMat3
    {
        friend class SMat4;
		friend class Mat4;
		friend class Mat3;

        // Row Major
        // http://en.wikipedia.org/wiki/Row-major_order
        // Each vector represents one row of the 3x3 matrix
        SVec3 m[3];

        // helpers for column dot row
        float dotCol0(const SVec3 &row) const;
        float dotCol1(const SVec3 &row) const;
        float dotCol2(const SVec3 &row) const;

    public:
        // Generic Constructors
        SMat3(void);
        SMat3(const SMat3 &other);
		explicit SMat3(const Mat3 &other);
        explicit SMat3(const SMat4 &mat);
		explicit SMat3(const Mat4 &mat);
		explicit SMat3(const SQuat &quat);
        SMat3(const SVec3 &r0, const SVec3 &r1, const SVec3 &r2);
        SMat3(float m00, float m01, float m02,
             float m10, float m11, float m12,
             float m20, float m21, float m22);
        
        // 2D Homogeneous Constructors
        explicit SMat3(float radians); // Rotation
        SMat3(float x_scalar, float y_scalar); // Scalar
        explicit SMat3(const Vec2 &translation); // Translation
        SMat3(const Vec2 &translation, float radians, const Vec2 &scale); // Translation/Rotation/Scalar

        // 3D Transformations (Scale and Rotation only)
        explicit SMat3(const SVec3 &scale); // X, Y, Z Scalar
        SMat3(float z_angle, float x_angle, float y_angle); // Euler Rotation About The Z -> X -> Y axis

        // Properties
        static const SMat3 &Identity(void);

        // Public Methods
        void Clean(void); // Sets all near zero elements to zero

        void Set(float M00, float M01, float M02,
                 float M10, float M11, float M12,
                 float M20, float M21, float M22);
        
        void Translate(const Vec2 &translation);
        static void Translate(SMat3 &mat, const Vec2 &translation);

        void Rotation(float radians);
        static void Rotation(SMat3 &mat, float radians);

        // Euler Axis Rotation
        void RotationZXY(float z_angle, float x_angle, float y_angle);
        static void RotationZXY(SMat3 &mat, float z_angle, float x_angle, float y_angle);

		SVec3 GetRotationXYZ(void) const;

        void Scale(const Vec2 &scale);
        static void Scale(SMat3 &mat, const Vec2 &scale);

        void Scale(const SVec3 &scale);
        static void Scale(SMat3 &mat, const SVec3 &scale);

        void TRS(const Vec2 &translation, float radians, const Vec2 &scale);
        static void TRS(SMat3 &mat, const Vec2 &translation, float radians, const Vec2 &scale);

		void SetLookAt(const SVec3& targetDirection, const SVec3& localForward, const SVec3& localUp, const SVec3& worldUp);

        void Transpose(void);
        static void Transpose(SMat3 &mat);
        
        SMat3 Transpose(void) const;
        static SMat3 Transpose(const SMat3 &mat);

        // TODO: SIMD Accelerate These Functions
        void Inverse(void);
        static void Inverse(SMat3 &mat);

        SMat3 Inverse(void) const;
        static SMat3 Inverse(const SMat3 &mat);

        float Determinant(void) const;
        static float Determinant(const SMat3 &mat);

		// Normalize all columns and make them orthogonal to eachother
		void Orthonormalize(void);

        void SetRows(const SVec3 &r0, const SVec3 &r1, const SVec3 &r2);
        void GetRows(SVec3 &r0, SVec3 &r1, SVec3 &r2) const;
        
        // i must be between 0 and 2 inclusively
        void SetRow(uint i, const SVec3 &row);
        SVec3 GetRow(uint i) const;

        void SetColumns(const SVec3 &c0, const SVec3 &c1, const SVec3 &c2);
        void GetColumns(SVec3 &c0, SVec3 &c1, SVec3 &c2) const;

        // i must be between 0 and 2 inclusively
        void SetColumn(uint i, const SVec3 &column);
        SVec3 GetColumn(uint i) const;

        // (fast) Homogeneous Transformations Only! Bottom row neglected.
        Vec2 TransformVector(const Vec2 &vector) const;
        void TransformVectorInplace(Vec2 &vector) const;
        Vec2 TransformPoint(const Vec2 &point) const;
        void TransformPointInplace(Vec2 &point) const;

        // Transforms a point and accounts for perspective divide
        Vec2 TransformPointAndDiv(const Vec2 &point) const;
        void TransformPointAndDivInplace(Vec2 &point) const;

		// 2D camera matrix
		void SetWorldToCamera(float width, float height, float rotation, const Vec2 &trans);

        std::string ToString(void) const;

        // Accessors
        float &operator()(uint row, uint column);
        float operator()(uint row, uint column) const;
        const float *GetFloatPtr(void) const;

        // Operators
        SMat3 operator*(const SMat3 &rhs) const;
        SMat3 operator*(float rhs) const;
        SVec3 operator*(const SVec3 &rhs) const;
        SMat3 operator+(const SMat3 &rhs) const;
        SMat3 operator-(const SMat3 &rhs) const;
        const SMat3 &operator=(const SMat3 &rhs);
        const SMat3 &operator*=(const SMat3 &rhs); // this = this * rhs
        const SMat3 &operator*=(float rhs);
        const SMat3 &operator+=(const SMat3 &rhs);
        const SMat3 &operator-=(const SMat3 &rhs);
        bool operator==(const SMat3 &rhs) const;
        bool operator!=(const SMat3 &rhs) const;

		ALLOW_ALIGNED_ALLOC(16)
    };
}

#include "SMat3.hpp"
