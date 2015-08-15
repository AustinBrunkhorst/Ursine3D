/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Matrix3.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include "UrsineMath.h"

/* Matrix elements are accessed with matrix(row, column) */

namespace Ursine
{
    class Matrix3
    {
        // Forward declarations
        friend class Matrix4;

    public:
        // Properties
        const static Matrix3 &Identity();

        // Constructors/destructor
        Matrix3();
        ~Matrix3();
        Matrix3(const Matrix3 &other);
        explicit Matrix3(const Matrix4 &transform);
        Matrix3(const Vector3 &c1, const Vector3 &c2, const Vector3 &c3);
        Matrix3(float radians); // Rotation Matrix Constructor
        Matrix3(float x_scalar, float y_scalar); // Scalar Matrix Constructor
        explicit Matrix3(const Vector2 &translation); // Translation Matrix Constructor
        Matrix3(float radians, const Vector2 &scale, const Vector2 &translation); // Rotation/Translation/Scalar Constructor

        // Poolable reset functionality
        void Reset();

        // Public methods
        void Clean(); // Sets all near zero elements to zero
        static void Rotation(Matrix3 &mat, float radians);
        static Matrix3 Rotation(float radians);
        static void Scale(Matrix3 &mat, float x_scalar, float y_scalar);
        static Matrix3 Scale(float x_scalar, float y_scalar);
        static void Translate(Matrix3 &mat, const Vector2 &translation);
        static Matrix3 Translate(const Vector2 &translation);
        Matrix3 &Transpose();
        static Matrix3 Transpose(const Matrix3 &mat);
        Matrix3 &Inverse();
        static Matrix3 Inverse(const Matrix3 &mat);
        float Determinant() const;
        bool IsZero() const;
        bool IsIdentity() const;
        void SetRows(const Vector3 &row1, const Vector3 &row2, const Vector3 &row3);
        void GetRows(Vector3 &row1, Vector3 &row2, Vector3 &row3);
        void SetColumns(const Vector3 &col1, const Vector3 &col2, const Vector3 &col3);
        void GetColumns(Vector3 &col1, Vector3 &col2, Vector3 &col3);
        Vector2 TransformVector(const Vector2 &vector) const;
        Vector2 TransformPoint(const Vector2 &point) const;

        // Text output
        friend std::ostream &operator<<(std::ostream &out, const Matrix3 &mat);

        // Accessors
        float &operator()(unsigned int row, unsigned int column);
        float operator()(unsigned int row, unsigned int column) const;
        const float *GetFloatPtr(void) const;

        // Operators
        Matrix3 operator*(const Matrix3 &rhs) const;
        Matrix3 operator*(float rhs) const;
        Vector3 operator*(const Vector3 &rhs) const;
        Matrix3 operator+(const Matrix3 &rhs) const;
        Matrix3 operator-(const Matrix3 &rhs) const;
        const Matrix3 &operator=(const Matrix3 &rhs);
        const Matrix3 &operator*=(const Matrix3 &rhs);
        const Matrix3 &operator*=(float rhs);
        const Matrix3 &operator+=(const Matrix3 &rhs);
        const Matrix3 &operator-=(const Matrix3 &rhs);
        bool operator==(const Matrix3 &rhs) const;
        bool operator!=(const Matrix3 &rhs) const;

        // Helpers
        inline void SetWorldToCamera(float width, float height,
                                     float rotation, const Vector2 &translation);
    protected:
        float _m[9];
    };

    class BasisVectorMatrix3 : public Matrix3
    {
        // Create Basis matrix for 3 axis vectors and the origin
        BasisVectorMatrix3(const Vector2 &x_axis, const Vector2 &y_axis, const Vector2 &origin);
    };
}

#include "Matrix3.hpp"
