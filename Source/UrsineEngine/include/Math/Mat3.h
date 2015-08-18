/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Mat3.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Vec3.h"
#include "Vec2.h"

namespace Ursine
{
    // Forward Declaration
    class Mat4;

    ALIGNED16(class) Mat3
    {
        friend class Mat4;

        // Row Major
        // http://en.wikipedia.org/wiki/Row-major_order
        // Each vector represents one row of the 3x3 matrix
        Vec3 m[3];

        // helpers for column dot row
        float dotCol0(const Vec3 &row) const;
        float dotCol1(const Vec3 &row) const;
        float dotCol2(const Vec3 &row) const;

    public:
        // Generic Constructors
        Mat3(void);
        Mat3(const Mat3 &other);
        explicit Mat3(const Mat4 &mat);
        Mat3(const Vec3 &r0, const Vec3 &r1, const Vec3 &r2);
        Mat3(float m00, float m01, float m02,
             float m10, float m11, float m12,
             float m20, float m21, float m22);
        
        // 2D Homogeneous Constructors
        explicit Mat3(float radians); // Rotation
        Mat3(float x_scalar, float y_scalar); // Scalar
        explicit Mat3(const Vec2 &translation); // Translation
        Mat3(const Vec2 &translation, float radians, const Vec2 &scale); // Translation/Rotation/Scalar

        // 3D Transformations (Scale and Rotation only)
        explicit Mat3(const Vec3 &scale); // X, Y, Z Scalar
        Mat3(float z_angle, float x_angle, float y_angle); // Euler Rotation About The Z -> X -> Y axis

        // Properties
        static const Mat3 &Identity(void);

        // Public Methods
        void Clean(void); // Sets all near zero elements to zero

        void Set(float M00, float M01, float M02,
                 float M10, float M11, float M12,
                 float M20, float M21, float M22);
        
        void Translate(const Vec2 &translation);
        static void Translate(Mat3 &mat, const Vec2 &translation);

        void Rotation(float radians);
        static void Rotation(Mat3 &mat, float radians);

        // Euler Axis Rotation
        void RotationZXY(float z_angle, float x_angle, float y_angle);
        static void RotationZXY(Mat3 &mat, float z_angle, float x_angle, float y_angle);

        void Scale(const Vec2 &scale);
        static void Scale(Mat3 &mat, const Vec2 &scale);

        void Scale(const Vec3 &scale);
        static void Scale(Mat3 &mat, const Vec3 &scale);

        void TRS(const Vec2 &translation, float radians, const Vec2 &scale);
        static void TRS(Mat3 &mat, const Vec2 &translation, float radians, const Vec2 &scale);

        void Transpose(void);
        static void Transpose(Mat3 &mat);
        
        Mat3 Transpose(void) const;
        static Mat3 Transpose(const Mat3 &mat);

        // TODO: SIMD Accelerate These Functions
        void Inverse(void);
        static void Inverse(Mat3 &mat);

        Mat3 Inverse(void) const;
        static Mat3 Inverse(const Mat3 &mat);

        float Determinant(void) const;
        static float Determinant(const Mat3 &mat);

        void SetRows(const Vec3 &r0, const Vec3 &r1, const Vec3 &r2);
        void GetRows(Vec3 &r0, Vec3 &r1, Vec3 &r2) const;
        
        // i must be between 0 and 2 inclusively
        void SetRow(uint i, const Vec3 &row);
        Vec3 GetRow(uint i) const;

        void SetColumns(const Vec3 &c0, const Vec3 &c1, const Vec3 &c2);
        void GetColumns(Vec3 &c0, Vec3 &c1, Vec3 &c2) const;

        // i must be between 0 and 2 inclusively
        void SetColumn(uint i, const Vec3 &column);
        Vec3 GetColumn(uint i) const;

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
        Mat3 operator*(const Mat3 &rhs) const;
        Mat3 operator*(float rhs) const;
        Vec3 operator*(const Vec3 &rhs) const;
        Mat3 operator+(const Mat3 &rhs) const;
        Mat3 operator-(const Mat3 &rhs) const;
        const Mat3 &operator=(const Mat3 &rhs);
        const Mat3 &operator*=(const Mat3 &rhs); // this = this * rhs
        const Mat3 &operator*=(float rhs);
        const Mat3 &operator+=(const Mat3 &rhs);
        const Mat3 &operator-=(const Mat3 &rhs);
        bool operator==(const Mat3 &rhs) const;
        bool operator!=(const Mat3 &rhs) const;
    };
}

#include "Mat3.hpp"
