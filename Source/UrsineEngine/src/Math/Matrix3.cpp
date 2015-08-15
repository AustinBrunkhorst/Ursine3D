/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Matrix3.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace Ursine
{
    // Constructors/destructor
    Matrix3::Matrix3()
        : Matrix3(Identity()) {}

    Matrix3::~Matrix3() {}

    Matrix3::Matrix3(const Matrix3 &value)
    {
        memcpy(_m, value._m, sizeof(_m));
    }

    Matrix3::Matrix3(const Matrix4 &transform)
    {
        memcpy(_m, transform._m, sizeof(_m));
    }

    Matrix3::Matrix3(const Vector3 &c1, const Vector3 &c2, const Vector3 &c3)
    {
        (*this)(0, 0) = c1.x;
        (*this)(1, 0) = c1.y;
        (*this)(2, 0) = c1.z;
        (*this)(0, 1) = c2.x;
        (*this)(1, 1) = c2.y;
        (*this)(2, 1) = c2.z;
        (*this)(0, 2) = c3.x;
        (*this)(1, 2) = c3.y;
        (*this)(2, 2) = c3.z;
    }

    Matrix3::Matrix3(float radians) // Rotation Matrix Constructor
    {
        float s, c;
        radians = Math::Wrap(radians, 0.0f, Math::PI_2);
        Math::SinCos(radians, s, c);
        s = Math::IsZero(s) ? 0 : s;
        c = Math::IsZero(c) ? 0 : c;
        (*this)(0, 0) = c;
        (*this)(1, 0) = s;
        (*this)(2, 0) = 0;
        (*this)(0, 1) = -s;
        (*this)(1, 1) = c;
        (*this)(2, 1) = 0;
        (*this)(0, 2) = 0;
        (*this)(1, 2) = 0;
        (*this)(2, 2) = 1;
    }

    Matrix3::Matrix3(float x_scalar, float y_scalar) // Scalar Matrix Constructor
    {
        (*this)(0, 0) = x_scalar;
        (*this)(1, 0) = 0;
        (*this)(2, 0) = 0;
        (*this)(0, 1) = 0;
        (*this)(1, 1) = y_scalar;
        (*this)(2, 1) = 0;
        (*this)(0, 2) = 0;
        (*this)(1, 2) = 0;
        (*this)(2, 2) = 1;
    }

    Matrix3::Matrix3(const Vector2 &translation) // Translation Matrix Constructor
    {
        (*this)(0, 0) = 1;
        (*this)(1, 0) = 0;
        (*this)(2, 0) = 0;
        (*this)(0, 1) = 0;
        (*this)(1, 1) = 1;
        (*this)(2, 1) = 0;
        (*this)(0, 2) = translation.x;
        (*this)(1, 2) = translation.y;
        (*this)(2, 2) = 1;
    }

    Matrix3::Matrix3(float radians, const Vector2 &scale, const Vector2 &translation) // Rotation/Translation/Scalar Constructor
        : Matrix3(scale.x, scale.y)
    {
        (*this) = Matrix3(radians) * (*this);
        (*this)(0, 2) = translation.x;
        (*this)(1, 2) = translation.y;
    }

    void Matrix3::Reset()
    {
        (*this) = Identity();
    }

    // Public methods
    void Matrix3::Clean()
    {
        for (int i = 0; i < 9; ++i)
            _m[i] = Math::IsZero(_m[i]) ? 0 : _m[i];
    }

    void Matrix3::Rotation(Matrix3 &mat, float radians)
    {
        mat = Matrix3(radians);
    }

    Matrix3 Matrix3::Rotation(float radians)
    {
        return Matrix3(radians);
    }

    void Matrix3::Scale(Matrix3 &mat, float x_scalar, float y_scalar)
    {
        mat = Matrix3(x_scalar, y_scalar);
    }

    Matrix3 Matrix3::Scale(float x_scalar, float y_scalar)
    {
        return Matrix3(x_scalar, y_scalar);
    }

    void Matrix3::Translate(Matrix3 &mat, const Vector2 &translation)
    {
        mat = Matrix3(translation);
    }

    Matrix3 Matrix3::Translate(const Vector2 &translation)
    {
        return Matrix3(translation);
    }

    Matrix3 &Matrix3::Transpose()
    {
        float temp = _m[1];
        _m[1] = _m[3];
        _m[3] = temp;

        temp = _m[2];
        _m[2] = _m[6];
        _m[6] = temp;

        temp = _m[5];
        _m[5] = _m[7];
        _m[7] = temp;

        return (*this);
    }

    Matrix3 Matrix3::Transpose(const Matrix3 &mat)
    {
        Matrix3 new_mat = mat;
        return new_mat.Transpose();
    }

    Matrix3 &Matrix3::Inverse()
    {
        (*this) = Inverse(*this);
        return (*this);
    }

    Matrix3 Matrix3::Inverse(const Matrix3 &mat)
    {
        Matrix3 new_mat;

        // compute determinant
        float cofactor0 = mat._m[4] * mat._m[8] - mat._m[5] * mat._m[7];
        float cofactor3 = mat._m[2] * mat._m[7] - mat._m[1] * mat._m[8];
        float cofactor6 = mat._m[1] * mat._m[5] - mat._m[2] * mat._m[4];
        float det = mat._m[0] * cofactor0 + mat._m[3] * cofactor3 + mat._m[6] * cofactor6;

        UAssert(det != 0, "Inverse of 0 determinant.");

        // create adjoint matrix and multiply by 1/det to get inverse
        float invDet = 1.0f / det;
        new_mat._m[0] = invDet * cofactor0;
        new_mat._m[1] = invDet * cofactor3;
        new_mat._m[2] = invDet * cofactor6;

        new_mat._m[3] = invDet * (mat._m[5] * mat._m[6] - mat._m[3] * mat._m[8]);
        new_mat._m[4] = invDet * (mat._m[0] * mat._m[8] - mat._m[2] * mat._m[6]);
        new_mat._m[5] = invDet * (mat._m[2] * mat._m[3] - mat._m[0] * mat._m[5]);

        new_mat._m[6] = invDet * (mat._m[3] * mat._m[7] - mat._m[4] * mat._m[6]);
        new_mat._m[7] = invDet * (mat._m[1] * mat._m[6] - mat._m[0] * mat._m[7]);
        new_mat._m[8] = invDet * (mat._m[0] * mat._m[4] - mat._m[1] * mat._m[3]);

        return new_mat;
    }

    float Matrix3::Determinant() const
    {
        return _m[0] * (_m[4] * _m[8] - _m[5] * _m[7])
            + _m[3] * (_m[2] * _m[7] - _m[1] * _m[8])
            + _m[6] * (_m[1] * _m[5] - _m[2] * _m[4]);
    }

    bool Matrix3::IsZero() const
    {
        for (int i = 0; i < 9; ++i)
            if (!Math::IsZero(_m[i]))
                return false;
        return true;
    }

    bool Matrix3::IsIdentity() const
    {
        return (*this) == Identity();
    }

    void Matrix3::SetRows(const Vector3 &row1, const Vector3 &row2, const Vector3 &row3)
    {
        for (int i = 0; i < 3; ++i)
            (*this)(0, i) = row1[i];
        for (int i = 0; i < 3; ++i)
            (*this)(1, i) = row2[i];
        for (int i = 0; i < 3; ++i)
            (*this)(2, i) = row3[i];
    }

    void Matrix3::GetRows(Vector3 &row1, Vector3 &row2, Vector3 &row3)
    {
        for (int i = 0; i < 3; ++i)
            row1[i] = (*this)(0, i);
        for (int i = 0; i < 3; ++i)
            row2[i] = (*this)(1, i);
        for (int i = 0; i < 3; ++i)
            row2[i] = (*this)(2, i);
    }

    void Matrix3::SetColumns(const Vector3 &col1, const Vector3 &col2, const Vector3 &col3)
    {
        for (int i = 0; i < 3; ++i)
            (*this)(i, 0) = col1[i];
        for (int i = 0; i < 3; ++i)
            (*this)(i, 1) = col2[i];
        for (int i = 0; i < 3; ++i)
            (*this)(i, 2) = col3[i];
    }

    void Matrix3::GetColumns(Vector3 &col1, Vector3 &col2, Vector3 &col3)
    {
        for (int i = 0; i < 3; ++i)
            col1[i] = (*this)(i, 0);
        for (int i = 0; i < 3; ++i)
            col2[i] = (*this)(i, 1);
        for (int i = 0; i < 3; ++i)
            col2[i] = (*this)(i, 2);
    }

    Vector2 Matrix3::TransformVector(const Vector2 &vector) const
    {
        Vector3 result = (*this) * Vector3(vector, 0.0f);
        return Vector2(result.x, result.y);
    }

    Vector2 Matrix3::TransformPoint(const Vector2 &point) const
    {
        Vector3 result = (*this) * Vector3(point, 1.0f);
        return Vector2(result.x, result.y);
    }

    // Text output
    std::ostream &operator<<(std::ostream &out, const Matrix3 &mat)
    {
        for (int row = 0; row < 3; ++row)
        {
            out << "[";
            for (int col = 0; col < 3; ++col)
            {
                out << mat(row, col) << (col == 2 ? "" : ",");
            }
            out << "]";
        }

        return out;
    }

    // Operators
    Matrix3 Matrix3::operator*(const Matrix3 &rhs) const
    {
        Matrix3 new_mat;

        new_mat._m[0] = _m[0] * rhs._m[0] + _m[3] * rhs._m[1] + _m[6] * rhs._m[2];
        new_mat._m[1] = _m[1] * rhs._m[0] + _m[4] * rhs._m[1] + _m[7] * rhs._m[2];
        new_mat._m[2] = _m[2] * rhs._m[0] + _m[5] * rhs._m[1] + _m[8] * rhs._m[2];
        new_mat._m[3] = _m[0] * rhs._m[3] + _m[3] * rhs._m[4] + _m[6] * rhs._m[5];
        new_mat._m[4] = _m[1] * rhs._m[3] + _m[4] * rhs._m[4] + _m[7] * rhs._m[5];
        new_mat._m[5] = _m[2] * rhs._m[3] + _m[5] * rhs._m[4] + _m[8] * rhs._m[5];
        new_mat._m[6] = _m[0] * rhs._m[6] + _m[3] * rhs._m[7] + _m[6] * rhs._m[8];
        new_mat._m[7] = _m[1] * rhs._m[6] + _m[4] * rhs._m[7] + _m[7] * rhs._m[8];
        new_mat._m[8] = _m[2] * rhs._m[6] + _m[5] * rhs._m[7] + _m[8] * rhs._m[8];

        return new_mat;
    }

    Matrix3 Matrix3::operator*(float rhs) const
    {
        Matrix3 new_mat = rhs;

        for (int i = 0; i < 9; ++i)
            new_mat._m[i] *= rhs;

        return new_mat;
    }

    Vector3 Matrix3::operator*(const Vector3 &rhs) const
    {
        Vector3 result;

        result.x = _m[0] * rhs.x + _m[3] * rhs.y + _m[6] * rhs.z;
        result.y = _m[1] * rhs.x + _m[4] * rhs.y + _m[7] * rhs.z;
        result.z = _m[2] * rhs.x + _m[5] * rhs.y + _m[8] * rhs.z;

        return result;
    }

    Matrix3 Matrix3::operator+(const Matrix3 &rhs) const
    {
        Matrix3 new_mat = (*this);

        for (int i = 0; i < 9; ++i)
            new_mat._m[i] += rhs._m[i];

        return new_mat;
    }

    Matrix3 Matrix3::operator-(const Matrix3 &rhs) const
    {
        Matrix3 new_mat = (*this);

        for (int i = 0; i < 9; ++i)
            new_mat._m[i] -= rhs._m[i];

        return new_mat;
    }

    const Matrix3 &Matrix3::operator=(const Matrix3 &rhs)
    {
        memcpy(_m, rhs._m, sizeof(_m));
        return (*this);
    }

    const Matrix3 &Matrix3::operator*=(const Matrix3 &rhs)
    {
        (*this) = rhs * (*this);
        return (*this);
    }

    const Matrix3 &Matrix3::operator*=(float rhs)
    {
        for (int i = 0; i < 9; ++i)
            _m[i] *= rhs;
        return (*this);
    }

    const Matrix3 &Matrix3::operator+=(const Matrix3 &rhs)
    {
        for (int i = 0; i < 9; ++i)
            _m[i] += rhs._m[i];
        return (*this);
    }

    const Matrix3 &Matrix3::operator-=(const Matrix3 &rhs)
    {
        for (int i = 0; i < 9; ++i)
            _m[i] -= rhs._m[i];
        return (*this);
    }

    bool Matrix3::operator==(const Matrix3 &rhs) const
    {
        for (int i = 0; i < 9; ++i)
            if (!Math::IsEqual(_m[i], rhs._m[i]))
                return false;
        return true;
    }

    bool Matrix3::operator!=(const Matrix3 &rhs) const
    {
        return !((*this) == rhs);
    }

    // Properties
    const Matrix3 &Matrix3::Identity()
    {
        const static Matrix3 identity = Matrix3(Vector3(1, 0, 0),
            Vector3(0, 1, 0),
            Vector3(0, 0, 1));
        return identity;
    }

    // Accessors
    float &Matrix3::operator()(unsigned int row, unsigned int column)
    {
        return _m[row + 3 * column];
    }

    float Matrix3::operator()(unsigned int row, unsigned int column) const
    {
        return _m[row + 3 * column];
    }

    const float *Matrix3::GetFloatPtr(void) const
    {
        return _m;
    }

    // Create Basis matrix for 3 axis vectors and the origin
    BasisVectorMatrix3::BasisVectorMatrix3(const Vector2 &x_axis, const Vector2 &y_axis, const Vector2 &origin)
    {
        for (int i = 0; i < 2; ++i)
        {
            (*this)(0, i) = x_axis[i];
            (*this)(1, i) = y_axis[i];
            (*this)(2, i) = 0.0f;
        }

        (*this)(0, 2) = Vector2::Dot(origin, x_axis);
        (*this)(1, 2) = Vector2::Dot(origin, y_axis);
        (*this)(2, 2) = 1.0f;
    }
}