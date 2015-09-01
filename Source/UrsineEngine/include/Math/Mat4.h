/* --h-------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Mat4.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Vec4.h"
#include "Vec3.h"

namespace ursine
{
	// Forward Declaration
	class Mat3;
	class SMat3;
	class SMat4;

    class Mat4
    {
        friend class Mat3;
		friend class SMat3;
		friend class SMat4;

        // Row Major
        // http://en.wikipedia.org/wiki/Row-major_order
        // Each vector represents one row of the 4x4 matrix
        Vec4 m[4];

        // helpers for column dot row
        float dotCol0(const Vec4 &row) const;
        float dotCol1(const Vec4 &row) const;
        float dotCol2(const Vec4 &row) const;
        float dotCol3(const Vec4 &row) const;

        // helper for inverse (eleminating duplicate code)
        static void invert(const float *src, float *dest);
        // TODO: static void invertFast(const float *src, float *dest);

    public:
        // Generic Constructors
        Mat4(void);
        Mat4(const Mat4 &other);
		explicit Mat4(const SMat4 &other);
        explicit Mat4(const Mat3 &mat);
		explicit Mat4(const SMat3 &mat);
        Mat4(const Vec4 &r0, const Vec4 &r1, const Vec4 &r2, const Vec4 &r3);
        Mat4(float m00, float m01, float m02, float m03,
             float m10, float m11, float m12, float m13,
             float m20, float m21, float m22, float m23,
             float m30, float m31, float m32, float m33);

        // 3D Homogenous Constructors
        Mat4(float x_scalar, float y_scalar, float z_scalar); // Scalar
        explicit Mat4(const Vec3 &translation); // Translation
        // TODO: Add rotation of 3D points and vectors (Euler and quat?)
        // TODO: Add TRS constructor

        // Non Homogenous Transformations
        // TODO: Perspective Projection
        // TODO: Orthogonal Projection

        // Properties
        static const Mat4 &Identity(void);

        // Public Methods
        void Clean(void); // Sets all near zero elements to zero

        void Set(float M00, float M01, float M02, float M03,
                 float M10, float M11, float M12, float M13,
                 float M20, float M21, float M22, float M23,
                 float M30, float M31, float M32, float M33);

        void Translate(const Vec3 &translation);
        static void Translate(Mat4 &mat, const Vec3 &translation);

        // TODO: Rotation(SQuat &quat)
        // TODO: static Rotation(SQuat &quat)

        void RotationZXY(float z_angle, float x_angle, float y_angle);
        static void RotationZXY(Mat4 &mat, float z_angle, float x_angle, float y_angle);

        void Scale(const Vec3 &scale);
        static void Scale(Mat4 &mat, const Vec3 &scale);

        void Scale(const Vec4 &scale);
        static void Scale(Mat4 &mat, const Vec4 &scale);

        // TODO: void TRS(const Vec3 &translation, SQuat &rotation, const Vec3 &scale);
        // TODO: static void TRS(const Vec3 &translation, SQuat &rotation, const Vec3 &scale);

        void Transpose(void);
        static void Transpose(Mat4 &mat);

        Mat4 Transpose(void) const;
        static Mat4 Transpose(const Mat4 &mat);

        void Inverse(void);
        static void Inverse(Mat4 &mat);

        Mat4 Inverse(void) const;
        static Mat4 Inverse(const Mat4 &mat);

        // Homogeneous Matrices Only!
        // TODO: void InverseFast(void);
        // TODO: static void InverseFast(Mat4 &mat);

        // Homogeneous Matrices Only!
        // TODO: Mat4 InverseFast(void) const;
        // TODO: static Mat4 InverseFast(const Mat4 &mat);

        float Determinant(void) const;
        static float Determinant(const Mat4 &mat);

        void SetRows(const Vec4 &r0, const Vec4 &r1, const Vec4 &r2, const Vec4 &r3);
        void GetRows(Vec4 &r0, Vec4 &r1, Vec4 &r2, Vec4 &r3) const;

        // i must be between 0 and 3 inclusively
        void SetRow(uint i, const Vec4 &row);
        Vec4 GetRow(uint i) const;

        void SetColumns(const Vec4 &c0, const Vec4 &c1, const Vec4 &c2, const Vec4 &c3);
        void GetColumns(Vec4 &c0, Vec4 &c1, Vec4 &c2, Vec4 &c3) const;

        // i must be between 0 and 3 inclusively
        void SetColumn(uint i, const Vec4 &column);
        Vec4 GetColumn(uint i) const;

        // (fast) Homogeneous Transformations Only! Bottom row neglected.
        Vec3 TransformVector(const Vec3 &vector) const;
        void TransformVectorInplace(Vec3 &vector) const;
        Vec3 TransformPoint(const Vec3 &point) const;
        void TransformPointInplace(Vec3 &point) const;

        // Transforms a point and accounts for perspective divide
        Vec3 TransformPointAndDiv(const Vec3 &point) const;
        void TransformPointAndDivInplace(Vec3 &point) const;

        std::string ToString(void) const;

        // Accessors
        float &operator()(uint row, uint column);
        float operator()(uint row, uint column) const;
        const float *GetFloatPtr(void) const;

        // Operators
        Mat4 operator*(const Mat4 &rhs) const;
        Mat4 operator*(float rhs) const;
        Vec4 operator*(const Vec4 &rhs) const;
        Mat4 operator+(const Mat4 &rhs) const;
        Mat4 operator-(const Mat4 &rhs) const;
        const Mat4 &operator=(const Mat4 &rhs);
        const Mat4 &operator*=(const Mat4 &rhs); // this = this * rhs
        const Mat4 &operator*=(float rhs);
        const Mat4 &operator+=(const Mat4 &rhs);
        const Mat4 &operator-=(const Mat4 &rhs);
        bool operator==(const Mat4 &rhs) const;
        bool operator!=(const Mat4 &rhs) const;
    };
}

#include "Mat4.hpp"
