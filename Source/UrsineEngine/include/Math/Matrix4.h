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

/* Matrix elements accessed with matrix(row, column) */

namespace Ursine
{
    // Forward declarations
    class Vector3;
    class Vector4;

    class Matrix4
    {
        friend class Matrix3;
    public:
        // Properties
        const static Matrix4 &Identity();

        // Constructors/destructor
        Matrix4();
        ~Matrix4();
        Matrix4(const Matrix4 &value);
        explicit Matrix4(const Matrix3 &transform);
        Matrix4(const Matrix3 &transform, const Vector3 &translation);
        Matrix4(const Vector4 &c1, const Vector4 &c2, const Vector4 &c3, const Vector4 &c4);
        Matrix4(float radians, const Vector3 &axis); // Rotation Matrix Constructor
        Matrix4(float x_scalar, float y_scalar, float z_scalar); // Scalar Matrix Constructor
        explicit Matrix4(const Vector3 &translation); // Translation Matrix Constructor
        Matrix4(float radians, const Vector3 &axis,
            const Vector3 &scale, const Vector3 &translation); // Rotation/Translation/Scalar Constructor

        // Public methods
        void Clean(); // Sets all near zero elements to zero
        static void Rotation(Matrix4 &mat, float radians, const Vector3 &axis);
        static Matrix4 Rotation(float radians, const Vector3 &axis);
        Matrix4 &Rotation(const Matrix3 &matrix);
        static void Scale(Matrix4 &mat, float x_scalar, float y_scalar, float z_scalar);
        static Matrix4 Scale(float x_scalar, float y_scalar, float z_scalar);
        static void Translate(Matrix4 &mat, const Vector3 &translation);
        static Matrix4 Translate(const Vector3 &translation);
        Matrix4 &Transpose();
        static Matrix4 Transpose(const Matrix4 &mat);
        Matrix4 &Inverse();
        static Matrix4 Inverse(const Matrix4 &mat);
        bool IsZero() const;
        bool IsIdentity() const;
        void SetRows(const Vector4 &row1, const Vector4 &row2,
            const Vector4 &row3, const Vector4 &row4);
        void GetRows(Vector4 &row1, Vector4 &row2, Vector4 &row3, Vector4 &row4);
        void SetColumns(const Vector4 &col1, const Vector4 &col2,
            const Vector4 &col3, const Vector4 &col4);
        void GetColumns(Vector4 &col1, Vector4 &col2, Vector4 &col3, Vector4 &col4);
        Vector3 TransformVector(const Vector3 &vector) const;
        Vector3 TransformPoint(const Vector3 &point) const;

        // Text output
        friend std::ostream &operator<<(std::ostream &out, const Matrix4 &mat);

        // Accessors
        float &operator()(unsigned int row, unsigned int column);
        float operator()(unsigned int row, unsigned int column) const;
        const float *GetFloatPtr(void) const;

        // Operators
        Matrix4 operator*(const Matrix4 &rhs) const;
        Matrix4 operator*(float rhs) const;
        Vector4 operator*(const Vector4 &rhs) const;
        Matrix4 operator+(const Matrix4 &rhs) const;
        Matrix4 operator-(const Matrix4 &rhs) const;
        const Matrix4 &operator=(const Matrix4 &rhs);
        const Matrix4 &operator*=(const Matrix4 &rhs);
        const Matrix4 &operator*=(float rhs);
        const Matrix4 &operator+=(const Matrix4 &rhs);
        const Matrix4 &operator-=(const Matrix4 &rhs);
        bool operator==(const Matrix4 &rhs) const;
        bool operator!=(const Matrix4 &rhs) const;


    protected:
        float _m[16];
    };

    /**
    * Storage class for compile-time fixed size matrices
    */
    template<int num_rows, int num_columns>
    class TMatrix
    {
    public:
        float m[num_rows][num_columns];

        // Constructors
        TMatrix();
        TMatrix(const TMatrix &mat);
    };

    class BasisVectorMatrix4 : public Matrix4
    {
        // Create Basis matrix for 3 axis vectors and the origin
        BasisVectorMatrix4(const Vector3 &x_axis, const Vector3 &y_axis,
            const Vector3 &z_axis, const Vector3 &origin);
    };

    class LookAtMatrix : Matrix4
    {
        // Creates a view matrix given an eye position,
        // a position to look at, and an up vector
        LookAtMatrix(const Vector3 &eye, const Vector3 &look_at, const Vector3 &up);
    };
}