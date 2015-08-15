/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Matrix4.cpp
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
    Matrix4::Matrix4()
        : Matrix4(Identity()) {}

    Matrix4::~Matrix4() {}

    Matrix4::Matrix4(const Matrix4 &value)
    {
        memcpy(_m, value._m, sizeof(_m));
    }

    Matrix4::Matrix4(const Matrix3 &transform)
    {
        (*this)(0, 0) = transform(0, 0);
        (*this)(1, 0) = transform(1, 0);
        (*this)(2, 0) = transform(2, 0);
        (*this)(3, 0) = 0.0f;
        (*this)(0, 1) = transform(0, 1);
        (*this)(1, 1) = transform(1, 1);
        (*this)(2, 1) = transform(2, 1);
        (*this)(3, 1) = 0.0f;
        (*this)(0, 2) = transform(0, 2);
        (*this)(1, 2) = transform(1, 2);
        (*this)(2, 2) = transform(2, 2);
        (*this)(3, 2) = 0.0f;
        (*this)(0, 3) = (*this)(1, 3) = (*this)(2, 3) = 0.0f;
        (*this)(3, 3) = 1.0f;
    }

    Matrix4::Matrix4(const Matrix3 &transform, const Vector3 &translation)
        : Matrix4(transform)
    {
        (*this)(0, 3) = translation.x;
        (*this)(1, 3) = translation.y;
        (*this)(2, 3) = translation.z;
    }

    Matrix4::Matrix4(const Vector4 &c1, const Vector4 &c2, const Vector4 &c3, const Vector4 &c4)
    {
        (*this)(0, 0) = c1.x;
        (*this)(1, 0) = c1.y;
        (*this)(2, 0) = c1.z;
        (*this)(3, 0) = c1.w;
        (*this)(0, 1) = c2.x;
        (*this)(1, 1) = c2.y;
        (*this)(2, 1) = c2.z;
        (*this)(3, 1) = c2.w;
        (*this)(0, 2) = c3.x;
        (*this)(1, 2) = c3.y;
        (*this)(2, 2) = c3.z;
        (*this)(3, 2) = c3.w;
        (*this)(0, 3) = c4.x;
        (*this)(1, 3) = c4.y;
        (*this)(2, 3) = c4.z;
        (*this)(3, 3) = c4.w;
    }

    Matrix4::Matrix4(float radians, const Vector3 &axis) // Rotation Matrix Constructor
    {
        Rotation((*this), radians, axis);
    }

    Matrix4::Matrix4(float x_scalar, float y_scalar, float z_scalar) // Scalar Matrix Constructor
        : Matrix4(Vector4(x_scalar, 0, 0, 0), Vector4(0, y_scalar, 0, 0), Vector4(0, 0, z_scalar, 0), Vector4(0, 0, 0, 1)) {}

    Matrix4::Matrix4(const Vector3 &translation) // Translation Matrix Constructor
        : Matrix4(Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), Vector4(0, 0, 1, 0), Vector4(translation, 1)) {}

    Matrix4::Matrix4(float radians, const Vector3 &axis,
        const Vector3 &scale, const Vector3 &translation) // Rotation/Translation/Scalar Constructor
        : Matrix4(scale.x, scale.y, scale.z)
    {
        (*this) *= Matrix4(radians, axis);
        _m[12] = translation.x;
        _m[13] = translation.y;
        _m[14] = translation.z;
    }

    // Public methods
    void Matrix4::Clean()
    {
        for (int i = 0; i < 16; ++i)
            _m[i] = Math::IsZero(_m[i]) ? 0.0f : _m[i];
    }

    void Matrix4::Rotation(Matrix4 &mat, float radians, const Vector3 &axis)
    {
        float s, c;
        Math::SinCos(radians, s, c);
        float t = 1.0f - c;

        Vector3 nAxis = Vector3::Normalize(axis);

        // intermediate values
        float tx = t * nAxis.x;
        float ty = t * nAxis.y;
        float tz = t * nAxis.z;
        float sx = s * nAxis.x;
        float sy = s * nAxis.y;
        float sz = s * nAxis.z;
        float txy = tx * nAxis.y;
        float tyz = ty * nAxis.z;
        float txz = tx * nAxis.z;

        // set matrix
        mat._m[0] = tx * nAxis.x + c;
        mat._m[4] = txy - sz;
        mat._m[8] = txz + sy;
        mat._m[12] = 0.0f;

        mat._m[1] = txy + sz;
        mat._m[5] = ty * nAxis.y + c;
        mat._m[9] = tyz - sx;
        mat._m[13] = 0.0f;

        mat._m[2] = txz - sy;
        mat._m[6] = tyz + sx;
        mat._m[10] = tz * nAxis.z + c;
        mat._m[14] = 0.0f;

        mat._m[3] = 0.0f;
        mat._m[7] = 0.0f;
        mat._m[11] = 0.0f;
        mat._m[15] = 1.0f;
    }

    Matrix4 Matrix4::Rotation(float radians, const Vector3 &axis)
    {
        Matrix4 new_mat;
        Rotation(new_mat, radians, axis);
        return new_mat;
    }

    Matrix4 &Matrix4::Rotation(const Matrix3 &matrix)
    {
        _m[0] = matrix._m[0];
        _m[1] = matrix._m[1];
        _m[2] = matrix._m[2];
        _m[3] = 0;
        _m[4] = matrix._m[3];
        _m[5] = matrix._m[4];
        _m[6] = matrix._m[5];
        _m[7] = 0;
        _m[8] = matrix._m[6];
        _m[9] = matrix._m[7];
        _m[10] = matrix._m[8];
        _m[11] = 0;
        _m[12] = 0;
        _m[13] = 0;
        _m[14] = 0;
        _m[15] = 1;

        return *this;
    }

    void Matrix4::Scale(Matrix4 &mat, float x_scalar, float y_scalar, float z_scalar)
    {
        mat = Matrix4(x_scalar, y_scalar, z_scalar);
    }

    Matrix4 Matrix4::Scale(float x_scalar, float y_scalar, float z_scalar)
    {
        return Matrix4(x_scalar, y_scalar, z_scalar);
    }

    void Matrix4::Translate(Matrix4 &mat, const Vector3 &translation)
    {
        mat = Matrix4(translation);
    }

    Matrix4 Matrix4::Translate(const Vector3 &translation)
    {
        return Matrix4(translation);
    }

    Matrix4 &Matrix4::Transpose()
    {
        float temp = _m[1];
        _m[1] = _m[4];
        _m[4] = temp;

        temp = _m[2];
        _m[2] = _m[8];
        _m[8] = temp;

        temp = _m[3];
        _m[2] = _m[12];
        _m[12] = temp;

        temp = _m[6];
        _m[6] = _m[9];
        _m[9] = temp;

        temp = _m[7];
        _m[7] = _m[13];
        _m[13] = temp;

        temp = _m[11];
        _m[11] = _m[14];
        _m[14] = temp;

        return *this;
    }

    Matrix4 Matrix4::Transpose(const Matrix4 &mat)
    {
        Matrix4 new_mat = mat;
        return new_mat.Transpose();
    }

    Matrix4 &Matrix4::Inverse()
    {
        (*this) = Matrix4::Inverse((*this));
        return (*this);
    }

    Matrix4 Matrix4::Inverse(const Matrix4 &mat)
    {
        Matrix4 result;

        // compute upper left 3x3 matrix determinant
        float cofactor0 = mat._m[5] * mat._m[10] - mat._m[6] * mat._m[9];
        float cofactor4 = mat._m[2] * mat._m[9] - mat._m[1] * mat._m[10];
        float cofactor8 = mat._m[1] * mat._m[6] - mat._m[2] * mat._m[5];
        float det = mat._m[0] * cofactor0 + mat._m[4] * cofactor4 + mat._m[8] * cofactor8;

        UAssert(!Math::IsZero(det), "Inverse of 0 determinant.");

        // create adjunct matrix and multiply by 1/det to get upper 3x3
        float invDet = 1.0f / det;
        result._m[0] = invDet * cofactor0;
        result._m[1] = invDet * cofactor4;
        result._m[2] = invDet * cofactor8;

        result._m[4] = invDet * (mat._m[6] * mat._m[8] - mat._m[4] * mat._m[10]);
        result._m[5] = invDet * (mat._m[0] * mat._m[10] - mat._m[2] * mat._m[8]);
        result._m[6] = invDet * (mat._m[2] * mat._m[4] - mat._m[0] * mat._m[6]);

        result._m[8] = invDet * (mat._m[4] * mat._m[9] - mat._m[5] * mat._m[8]);
        result._m[9] = invDet * (mat._m[1] * mat._m[8] - mat._m[0] * mat._m[9]);
        result._m[10] = invDet * (mat._m[0] * mat._m[5] - mat._m[1] * mat._m[4]);

        // multiply -translation by inverted 3x3 to get its inverse

        result._m[12] = -result._m[0] * mat._m[12] - result._m[4] * mat._m[13] - result._m[8] * mat._m[14];
        result._m[13] = -result._m[1] * mat._m[12] - result._m[5] * mat._m[13] - result._m[9] * mat._m[14];
        result._m[14] = -result._m[2] * mat._m[12] - result._m[6] * mat._m[13] - result._m[10] * mat._m[14];

        return result;
    }

    bool Matrix4::IsZero() const
    {
        for (int i = 0; i < 16; ++i)
            if (!Math::IsZero(_m[i]))
                return false;

        return true;
    }

    bool Matrix4::IsIdentity() const
    {
        return (*this) == Identity();
    }

    void Matrix4::SetRows(const Vector4 &row1, const Vector4 &row2,
        const Vector4 &row3, const Vector4 &row4)
    {
        for (int i = 0; i < 4; ++i)
            (*this)(0, i) = row1[i];
        for (int i = 0; i < 4; ++i)
            (*this)(1, i) = row2[i];
        for (int i = 0; i < 4; ++i)
            (*this)(2, i) = row3[i];
        for (int i = 0; i < 4; ++i)
            (*this)(3, i) = row4[i];
    }

    void Matrix4::GetRows(Vector4 &row1, Vector4 &row2, Vector4 &row3, Vector4 &row4)
    {
        for (int i = 0; i < 4; ++i)
            row1[i] = (*this)(0, i);
        for (int i = 0; i < 4; ++i)
            row2[i] = (*this)(1, i);
        for (int i = 0; i < 4; ++i)
            row3[i] = (*this)(2, i);
        for (int i = 0; i < 4; ++i)
            row4[i] = (*this)(3, i);
    }

    void Matrix4::SetColumns(const Vector4 &col1, const Vector4 &col2,
        const Vector4 &col3, const Vector4 &col4)
    {
        for (int i = 0; i < 4; ++i)
            (*this)(i, 0) = col1[i];
        for (int i = 0; i < 4; ++i)
            (*this)(i, 1) = col2[i];
        for (int i = 0; i < 4; ++i)
            (*this)(i, 2) = col3[i];
        for (int i = 0; i < 4; ++i)
            (*this)(i, 3) = col4[i];
    }

    void Matrix4::GetColumns(Vector4 &col1, Vector4 &col2, Vector4 &col3, Vector4 &col4)
    {
        for (int i = 0; i < 4; ++i)
            col1[i] = (*this)(i, 0);
        for (int i = 0; i < 4; ++i)
            col2[i] = (*this)(i, 1);
        for (int i = 0; i < 4; ++i)
            col3[i] = (*this)(i, 2);
        for (int i = 0; i < 4; ++i)
            col4[i] = (*this)(i, 3);
    }

    Vector3 Matrix4::TransformVector(const Vector3 &vector) const
    {
        Vector4 result = (*this) * Vector4(vector, 0);
        return Vector3(result.x, result.y, result.z);
    }

    Vector3 Matrix4::TransformPoint(const Vector3 &vector) const
    {
        Vector4 result = (*this) * Vector4(vector, 1);
        return Vector3(result.x, result.y, result.z);
    }

    // Text output
    std::ostream &operator<<(std::ostream &out, const Matrix4 &mat)
    {
        for (int row = 0; row < 4; ++row)
        {
            out << "[";
            for (int col = 0; col < 4; ++col)
            {
                out << mat(row, col) << (col == 3 ? "" : ",");
            }
            out << "]" << std::endl;
        }
        return out;
    }

    // Operators
    Matrix4 Matrix4::operator*(const Matrix4 &rhs) const
    {
        Matrix4 new_mat;

        new_mat._m[0] = _m[0] * rhs._m[0] + _m[4] * rhs._m[1] + _m[8] * rhs._m[2]
            + _m[12] * rhs._m[3];
        new_mat._m[1] = _m[1] * rhs._m[0] + _m[5] * rhs._m[1] + _m[9] * rhs._m[2]
            + _m[13] * rhs._m[3];
        new_mat._m[2] = _m[2] * rhs._m[0] + _m[6] * rhs._m[1] + _m[10] * rhs._m[2]
            + _m[14] * rhs._m[3];
        new_mat._m[3] = _m[3] * rhs._m[0] + _m[7] * rhs._m[1] + _m[11] * rhs._m[2]
            + _m[15] * rhs._m[3];

        new_mat._m[4] = _m[0] * rhs._m[4] + _m[4] * rhs._m[5] + _m[8] * rhs._m[6]
            + _m[12] * rhs._m[7];
        new_mat._m[5] = _m[1] * rhs._m[4] + _m[5] * rhs._m[5] + _m[9] * rhs._m[6]
            + _m[13] * rhs._m[7];
        new_mat._m[6] = _m[2] * rhs._m[4] + _m[6] * rhs._m[5] + _m[10] * rhs._m[6]
            + _m[14] * rhs._m[7];
        new_mat._m[7] = _m[3] * rhs._m[4] + _m[7] * rhs._m[5] + _m[11] * rhs._m[6]
            + _m[15] * rhs._m[7];

        new_mat._m[8] = _m[0] * rhs._m[8] + _m[4] * rhs._m[9] + _m[8] * rhs._m[10]
            + _m[12] * rhs._m[11];
        new_mat._m[9] = _m[1] * rhs._m[8] + _m[5] * rhs._m[9] + _m[9] * rhs._m[10]
            + _m[13] * rhs._m[11];
        new_mat._m[10] = _m[2] * rhs._m[8] + _m[6] * rhs._m[9] + _m[10] * rhs._m[10]
            + _m[14] * rhs._m[11];
        new_mat._m[11] = _m[3] * rhs._m[8] + _m[7] * rhs._m[9] + _m[11] * rhs._m[10]
            + _m[15] * rhs._m[11];

        new_mat._m[12] = _m[0] * rhs._m[12] + _m[4] * rhs._m[13] + _m[8] * rhs._m[14]
            + _m[12] * rhs._m[15];
        new_mat._m[13] = _m[1] * rhs._m[12] + _m[5] * rhs._m[13] + _m[9] * rhs._m[14]
            + _m[13] * rhs._m[15];
        new_mat._m[14] = _m[2] * rhs._m[12] + _m[6] * rhs._m[13] + _m[10] * rhs._m[14]
            + _m[14] * rhs._m[15];
        new_mat._m[15] = _m[3] * rhs._m[12] + _m[7] * rhs._m[13] + _m[11] * rhs._m[14]
            + _m[15] * rhs._m[15];

        return new_mat;
    }

    Matrix4 Matrix4::operator*(float rhs) const
    {
        Matrix4 new_mat = (*this);

        for (int i = 0; i < 16; ++i)
            new_mat._m[i] *= rhs;

        return new_mat;
    }

    Vector4 Matrix4::operator*(const Vector4 &rhs) const
    {
        Vector4 result;

        result.x = _m[0] * rhs.x + _m[4] * rhs.y + _m[8] * rhs.z + _m[12] * rhs.w;
        result.y = _m[1] * rhs.x + _m[5] * rhs.y + _m[9] * rhs.z + _m[13] * rhs.w;
        result.z = _m[2] * rhs.x + _m[6] * rhs.y + _m[10] * rhs.z + _m[14] * rhs.w;
        result.w = _m[3] * rhs.x + _m[7] * rhs.y + _m[11] * rhs.z + _m[15] * rhs.w;

        return result;
    }

    Matrix4 Matrix4::operator+(const Matrix4 &rhs) const
    {
        Matrix4 new_mat = (*this);

        for (int i = 0; i < 16; ++i)
            new_mat._m[i] += rhs._m[i];

        return new_mat;
    }

    Matrix4 Matrix4::operator-(const Matrix4 &rhs) const
    {
        Matrix4 new_mat = (*this);

        for (int i = 0; i < 16; ++i)
            new_mat._m[i] -= rhs._m[i];

        return new_mat;
    }

    const Matrix4 &Matrix4::operator=(const Matrix4 &rhs)
    {
        memcpy(_m, rhs._m, sizeof(_m));
        return (*this);
    }

    const Matrix4 &Matrix4::operator*=(const Matrix4 &rhs)
    {
        (*this) = rhs * (*this);
        return (*this);
    }

    const Matrix4 &Matrix4::operator*=(float rhs)
    {
        for (int i = 0; i < 16; ++i)
            _m[i] *= rhs;
        return (*this);
    }

    const Matrix4 &Matrix4::operator+=(const Matrix4 &rhs)
    {
        for (int i = 0; i < 16; ++i)
            _m[i] += rhs._m[i];
        return (*this);
    }

    const Matrix4 &Matrix4::operator-=(const Matrix4 &rhs)
    {
        for (int i = 0; i < 16; ++i)
            _m[i] -= rhs._m[i];
        return (*this);
    }

    bool Matrix4::operator==(const Matrix4 &rhs) const
    {
        for (int i = 0; i < 16; ++i)
            if (!Math::IsEqual(_m[i], rhs._m[i]))
                return false;

        return true;
    }

    bool Matrix4::operator!=(const Matrix4 &rhs) const
    {
        return !((*this) == rhs);
    }

    const Matrix4 &Matrix4::Identity()
    {
        const static Matrix4 identity = Matrix4(Vector4(1, 0, 0, 0),
            Vector4(0, 1, 0, 0),
            Vector4(0, 0, 1, 0),
            Vector4(0, 0, 0, 1));
        return identity;
    }

    float &Matrix4::operator()(unsigned int row, unsigned int column)
    {
        return _m[row + 4 * column];
    }

    float Matrix4::operator()(unsigned int row, unsigned int column) const
    {
        return _m[row + 4 * column];
    }

    const float *Matrix4::GetFloatPtr(void) const
    {
        return _m;
    }

    template<int num_rows, int num_columns>
    TMatrix<num_rows, num_columns>::TMatrix() {}

    template<int num_rows, int num_columns>
    TMatrix<num_rows, num_columns>::TMatrix(const TMatrix &mat)
    {
        for (int i = 0; (i < num_rows); ++i)
            for (int j = 0; (j < num_columns); ++i)
                m[i][j] = mat.m[i][j];
    }

    BasisVectorMatrix4::BasisVectorMatrix4(const Vector3 &x_axis, const Vector3 &y_axis, const Vector3 &z_axis, const Vector3 &origin)
    {
        for (int i = 0; i < 3; ++i)
        {
            (*this)(0, i) = x_axis[i];
            (*this)(1, i) = y_axis[i];
            (*this)(2, i) = z_axis[i];
            (*this)(3, i) = 0.0f;
        }

        (*this)(0, 3) = Vector3::Dot(origin, x_axis);
        (*this)(1, 3) = Vector3::Dot(origin, y_axis);
        (*this)(2, 3) = Vector3::Dot(origin, z_axis);
        (*this)(3, 3) = 1.0f;
    }

    LookAtMatrix::LookAtMatrix(const Vector3 &eye, const Vector3 &look_at, const Vector3 &up)
    {
        const Vector3 z_axis = Vector3::Normalize(look_at - eye);
        const Vector3 x_axis = Vector3::Normalize(Vector3::Cross(up, z_axis));
        const Vector3 y_axis = Vector3::Cross(z_axis, x_axis);

        for (int i = 0; i < 3; ++i)
        {
            (*this)(0, i) = x_axis[i];
            (*this)(1, i) = y_axis[i];
            (*this)(2, i) = z_axis[i];
            (*this)(3, i) = 0.0f;
        }

        (*this)(0, 3) = Vector3::Dot(-eye, x_axis);
        (*this)(1, 3) = Vector3::Dot(-eye, y_axis);
        (*this)(2, 3) = Vector3::Dot(-eye, z_axis);
        (*this)(3, 3) = 1.0f;
    }
}