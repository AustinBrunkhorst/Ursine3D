
#include <string.h>

namespace ursine
{
    // Constructors
    INLINE Mat3::Mat3(void)
        : Mat3(Identity()) { }

    INLINE Mat3::Mat3(const Mat3 &other)
    {
        memcpy(m, other.m, sizeof(m));
    }

    INLINE Mat3::Mat3(const Vec3 &r0, const Vec3 &r1, const Vec3 &r2)
    {
        m[0] = r0;
        m[1] = r1;
        m[2] = r2;
    }

    INLINE Mat3::Mat3(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22)
    {
        Set(
            m00, m01, m02,
            m10, m11, m12,
            m20, m21, m22
            );
    }

    INLINE Mat3::Mat3(float radians)
    {
        Rotation(*this, radians);
    }

    INLINE Mat3::Mat3(float x_scalar, float y_scalar)
    {
        Set(
            x_scalar, 0.0f, 0.0f,
            0.0f, y_scalar, 0.0f,
            0.0f, 0.0f, 1.0f
            );
    }

    INLINE Mat3::Mat3(const Vec2 &translation)
    {
        Set(
            1.0f, 0.0f, translation.X(),
            0.0f, 1.0f, translation.Y(),
            0.0f, 0.0f, 1.0f
            );
    }

    INLINE Mat3::Mat3(const Vec2 &translation, float radians, const Vec2 &scale)
    {
        TRS(*this, translation, radians, scale);
    }

    INLINE Mat3::Mat3(const Vec3 &scale)
    {
        Set(
            scale.X(), 0.0f, 0.0f,
            0.0f, scale.Y(), 0.0f,
            0.0f, 0.0f, scale.Z()
            );
    }

    INLINE Mat3::Mat3(float z_angle, float x_angle, float y_angle)
    {
        RotationZXY(*this, z_angle, x_angle, y_angle);
    }

    // Properties
    INLINE const Mat3 &Mat3::Identity(void)
    {
        static const Mat3 identity(1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f);

        return identity;
    }

    // Private Methods
    INLINE float Mat3::dotCol0(const Vec3 &row) const
    {
        return row.Dot(Vec3(m[0][0], m[1][0], m[2][0]));
    }

    INLINE float Mat3::dotCol1(const Vec3 &row) const
    {
        return row.Dot(Vec3(m[0][1], m[1][1], m[2][1]));
    }

    INLINE float Mat3::dotCol2(const Vec3 &row) const
    {
        return row.Dot(Vec3(m[0][2], m[1][2], m[2][2]));
    }

    // Public Methods
    INLINE void Mat3::Clean(void)
    {
        m[0].Clean();
        m[1].Clean();
        m[2].Clean();
    }

    INLINE void Mat3::Set(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22)
    {
        m[0].Set(m00, m01, m02);
        m[1].Set(m10, m11, m12);
        m[2].Set(m20, m21, m22);
    }

    INLINE void Mat3::Translate(const Vec2 &translation)
    {
        Translate(*this, translation);
    }

    INLINE void Mat3::Translate(Mat3 &mat, const Vec2 &translation)
    {
        mat.Set(
            1.0f, 0.0f, translation.X(),
            0.0f, 1.0f, translation.Y(),
            0.0f, 0.0f, 1.0f
            );
    }

    INLINE void Mat3::Rotation(float radians)
    {
        Rotation(*this, radians);
    }

    INLINE void Mat3::Rotation(Mat3 &mat, float radians)
    {
        float s, c;
        math::SinCos(radians, s, c);

        mat.Set(
            c, -s, 0.0f,
            s, c, 0.0f,
            0.0f, 0.0f, 1.0f
            );
    }

    INLINE void Mat3::RotationZXY(float z_angle, float x_angle, float y_angle)
    {
        RotationZXY(*this, z_angle, x_angle, y_angle);
    }

    INLINE void Mat3::RotationZXY(Mat3 &mat, float z_angle, float x_angle, float y_angle)
    {
        float cx, sx, cy, sy, cz, sz;

        math::SinCos(x_angle, sx, cx);
        math::SinCos(y_angle, sy, cy);
        math::SinCos(z_angle, sz, cz);

        float cycz = cy * cz;
        float sxsy = sx * sy;
        float szcy = sz * cy;

        mat.Set(
            cycz + sxsy * sz, cz * sxsy - szcy, cx * sx,
            cx * sz, cx * cz, -sx,
            szcy * sx - cz * sy, cycz * sx + sy * sz, cx * cy
            );
    }

    INLINE void Mat3::Scale(const Vec2 &scale)
    {
        Scale(*this, scale);
    }

    INLINE void Mat3::Scale(Mat3 &mat, const Vec2 &scale)
    {
        mat.Set(
            scale.X(), 0.0f, 0.0f,
            0.0f, scale.Y(), 0.0f,
            0.0f, 0.0f, 1.0f
            );
    }

    INLINE void Mat3::Scale(const Vec3 &scale)
    {
        Scale(*this, scale);
    }

    INLINE void Mat3::Scale(Mat3 &mat, const Vec3 &scale)
    {
        mat.Set(
            scale.X(), 0.0f, 0.0f,
            0.0f, scale.Y(), 0.0f,
            0.0f, 0.0f, scale.Z()
            );
    }

    INLINE void Mat3::TRS(const Vec2 &translation, float radians, const Vec2 &scale)
    {
        TRS(*this, translation, radians, scale);
    }

    INLINE void Mat3::TRS(Mat3 &mat, const Vec2 &translation, float radians, const Vec2 &scale)
    {
        float scale_x = scale.X(), scale_y = scale.Y();
        float s, c;
        math::SinCos(radians, s, c);

        mat.Set(
            scale_x * c, -scale_y * s, translation.X(),
            scale_x * s, scale_y * c, translation.Y(),
            0.0f, 0.0f, 1.0f
            );
    }

    INLINE void Mat3::Transpose(void)
    {
        Transpose(*this);
    }

    INLINE void Mat3::Transpose(Mat3 &mat)
    {
        // swap(1, 3)
        float temp = mat.m[0][1];
        mat.m[0][1] = mat.m[1][0];
        mat.m[1][0] = temp;

        // swap (2, 6)
        temp = mat.m[0][2];
        mat.m[0][2] = mat.m[2][0];
        mat.m[2][0] = temp;

        // swap(5, 7)
        temp = mat.m[1][2];
        mat.m[1][2] = mat.m[2][1];
        mat.m[2][1] = temp;
    }

    INLINE Mat3 Mat3::Transpose(void) const
    {
        return Transpose(*this);
    }

    INLINE Mat3 Mat3::Transpose(const Mat3 &mat)
    {
        return{
            mat.m[0][0], mat.m[1][0], mat.m[2][0],
            mat.m[0][1], mat.m[1][1], mat.m[2][1],
            mat.m[0][2], mat.m[1][2], mat.m[2][2]
        };
    }

    INLINE void Mat3::Inverse(void)
    {
        Inverse(*this);
    }

    INLINE void Mat3::Inverse(Mat3 &mat)
    {
        // compute determinant
        float cofactor0 = mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1];
        float cofactor3 = mat.m[0][2] * mat.m[2][1] - mat.m[0][1] * mat.m[2][2];
        float cofactor6 = mat.m[0][1] * mat.m[1][2] - mat.m[0][2] * mat.m[1][1];

        float det = mat.m[0][0] * cofactor0 + mat.m[1][0] * cofactor3 + mat.m[2][0] * cofactor6;

        if (det == 0.0f)
            return;

        // create adjoint matrix and multiply by 1/det to get inverse
        float invDet = 1.0f / det;
        float N10 = mat.m[1][2] * mat.m[2][0] - mat.m[1][0] * mat.m[2][2],
            N11 = mat.m[0][0] * mat.m[2][2] - mat.m[0][2] * mat.m[2][0],
            N12 = mat.m[0][2] * mat.m[1][0] - mat.m[0][0] * mat.m[1][2],
            N20 = mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0],
            N21 = mat.m[0][1] * mat.m[2][0] - mat.m[0][0] * mat.m[2][1],
            N22 = mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0];

        mat.Set(
            invDet * cofactor0, invDet * cofactor3, invDet * cofactor6,
            invDet * N10, invDet * N11, invDet * N12,
            invDet * N20, invDet * N21, invDet * N22
            );
    }

    INLINE Mat3 Mat3::Inverse(void) const
    {
        return Inverse(*this);
    }

    INLINE Mat3 Mat3::Inverse(const Mat3 &mat)
    {
        Mat3 new_mat = mat;

        new_mat.Inverse();

        return new_mat;
    }

    INLINE float Mat3::Determinant(void) const
    {
        return Determinant(*this);
    }

    INLINE float Mat3::Determinant(const Mat3 &mat)
    {
        float cofactor0 = mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1];
        float cofactor3 = mat.m[0][2] * mat.m[2][1] - mat.m[0][1] * mat.m[2][2];
        float cofactor6 = mat.m[0][1] * mat.m[1][2] - mat.m[0][2] * mat.m[1][1];

        return mat.m[0][0] * cofactor0 + mat.m[1][0] * cofactor3 + mat.m[2][0] * cofactor6;
    }

    INLINE void Mat3::SetRows(const Vec3 &r0, const Vec3 &r1, const Vec3 &r2)
    {
        m[0] = r0;
        m[1] = r1;
        m[2] = r2;
    }

    INLINE void Mat3::GetRows(Vec3 &r0, Vec3 &r1, Vec3 &r2) const
    {
        r0 = m[0];
        r1 = m[1];
        r2 = m[2];
    }

    INLINE void Mat3::SetRow(uint i, const Vec3 &row)
    {
        m[i] = row;
    }

    INLINE Vec3 Mat3::GetRow(uint i) const
    {
        return m[i];
    }

    INLINE void Mat3::SetColumns(const Vec3 &c0, const Vec3 &c1, const Vec3 &c2)
    {
        Set(
            c0.X(), c1.X(), c2.X(),
            c0.Y(), c1.Y(), c2.Y(),
            c0.Z(), c1.Z(), c2.Z()
            );
    }

    INLINE void Mat3::GetColumns(Vec3 &c0, Vec3 &c1, Vec3 &c2) const
    {
        c0.Set(m[0][0], m[1][0], m[2][0]);
        c1.Set(m[0][1], m[1][1], m[2][1]);
        c2.Set(m[0][2], m[1][2], m[2][2]);
    }

    INLINE void Mat3::SetColumn(uint i, const Vec3 &column)
    {
        auto &mat = *this;

        mat(0, i) = column.X();
        mat(1, i) = column.Y();
        mat(2, i) = column.Z();
    }

    INLINE Vec3 Mat3::GetColumn(uint i) const
    {
        auto &mat = *this;

        return{ mat(0, i), mat(1, i), mat(2, i) };
    }

    INLINE Vec2 Mat3::TransformVector(const Vec2 &vector) const
    {
        float x = vector.X();
        float y = vector.Y();

        return{
            m[0][0] * x + m[0][1] * y,
            m[1][0] * x + m[1][1] * y
        };
    }

    INLINE void Mat3::TransformVectorInplace(Vec2 &vector) const
    {
        float x = vector.X();
        float y = vector.Y();

        vector.X() = m[0][0] * x + m[0][1] * y;
        vector.Y() = m[1][0] * x + m[1][1] * y;
    }

    INLINE Vec2 Mat3::TransformPoint(const Vec2 &point) const
    {
        float x = point.X();
        float y = point.Y();

        return{
            m[0][0] * x + m[0][1] * y + m[0][2],
            m[1][0] * x + m[1][1] * y + m[1][2]
        };
    }

    INLINE void Mat3::TransformPointInplace(Vec2 &point) const
    {
        float x = point.X();
        float y = point.Y();

        point.X() = m[0][0] * x + m[0][1] * y + m[0][2];
        point.Y() = m[1][0] * x + m[1][1] * y + m[1][2];
    }

    INLINE Vec2 Mat3::TransformPointAndDiv(const Vec2 &point) const
    {
        float x = point.X();
        float y = point.Y();
        float z = m[2][0] * x + m[2][1] * y + m[2][2];
        float inv_div = math::IsZero(z) ? 1.0f : 1.0f / z;

        return{
            inv_div * (m[0][0] * x + m[0][1] * y + m[0][2]),
            inv_div * (m[1][0] * x + m[1][1] * y + m[1][2])
        };
    }

    INLINE void Mat3::TransformPointAndDivInplace(Vec2 &point) const
    {
        float x = point.X();
        float y = point.Y();
        float z = m[2][0] * x + m[2][1] * y + m[2][2];
        float inv_div = math::IsZero(z) ? 1.0f : 1.0f / z;

        point.X() = inv_div * (m[0][0] * x + m[0][1] * y + m[0][2]);
        point.Y() = inv_div * (m[1][0] * x + m[1][1] * y + m[1][2]);
    }

	INLINE void Mat3::SetWorldToCamera(float width, float height, float rotation, const Vec2 &trans)
	{
		float c, s;
		math::SinCos(-rotation, s, c);

		float c_2 = c * c, s_2 = s * s
			, A = c / (c_2 + s_2)
			, B = s / (c_2 + s_2);

		float inv_w = 1.0f / width
			, inv_h = 1.0f / height;

		SetColumns(
			Vec3(A * inv_w, -B * inv_h, 0),
			Vec3(B * inv_w, A * inv_h, 0),
			Vec3(((-A * trans.X()) - B * trans.Y()) * inv_w,
				 (B * trans.X() - A * trans.Y()) * inv_h, 1)
		);
	}

    // Accessors
    INLINE float &Mat3::operator()(uint row, uint column)
    {
        return m[row][column];
    }

    INLINE float Mat3::operator()(uint row, uint column) const
    {
        return m[row][column];
    }

    INLINE const float* Mat3::GetFloatPtr(void) const
    {
        return m[0].GetFloatPtr();
    }

    // Operators
    INLINE Mat3 Mat3::operator*(const Mat3 &rhs) const
    {
        return{
            rhs.dotCol0(m[0]), rhs.dotCol1(m[0]), rhs.dotCol2(m[0]),
            rhs.dotCol0(m[1]), rhs.dotCol1(m[1]), rhs.dotCol2(m[1]),
            rhs.dotCol0(m[2]), rhs.dotCol1(m[2]), rhs.dotCol2(m[2])
        };
    }

    INLINE Mat3 Mat3::operator*(float rhs) const
    {
        return{
            m[0] * rhs,
            m[1] * rhs,
            m[2] * rhs
        };
    }

    INLINE Vec3 Mat3::operator*(const Vec3 &rhs) const
    {
        return{
            m[0].Dot(rhs),
            m[1].Dot(rhs),
            m[2].Dot(rhs)
        };
    }

    INLINE Mat3 Mat3::operator+(const Mat3 &rhs) const
    {
        auto &rhs_m = rhs.m;

        return{
            m[0] + rhs_m[0],
            m[1] + rhs_m[1],
            m[2] + rhs_m[2]
        };
    }

    INLINE Mat3 Mat3::operator-(const Mat3 &rhs) const
    {
        auto &rhs_m = rhs.m;

        return{
            m[0] - rhs_m[0],
            m[1] - rhs_m[1],
            m[2] - rhs_m[2]
        };
    }

    INLINE const Mat3 &Mat3::operator=(const Mat3 &rhs)
    {
        auto &rhs_m = rhs.m;

        m[0] = rhs_m[0];
        m[1] = rhs_m[1];
        m[2] = rhs_m[2];

        return *this;
    }

    INLINE const Mat3 &Mat3::operator*=(const Mat3 &rhs)
    {
        Set(
            rhs.dotCol0(m[0]), rhs.dotCol1(m[0]), rhs.dotCol2(m[0]),
            rhs.dotCol0(m[1]), rhs.dotCol1(m[1]), rhs.dotCol2(m[1]),
            rhs.dotCol0(m[2]), rhs.dotCol1(m[2]), rhs.dotCol2(m[2])
            );

        return *this;
    }

    INLINE const Mat3 &Mat3::operator*=(float rhs)
    {
        m[0] *= rhs;
        m[1] *= rhs;
        m[2] *= rhs;

        return *this;
    }

    INLINE const Mat3 &Mat3::operator+=(const Mat3 &rhs)
    {
        auto &rhs_m = rhs.m;

        m[0] += rhs_m[0];
        m[1] += rhs_m[1];
        m[2] += rhs_m[2];

        return *this;
    }

    INLINE const Mat3 &Mat3::operator-=(const Mat3 &rhs)
    {
        auto &rhs_m = rhs.m;

        m[0] -= rhs_m[0];
        m[1] -= rhs_m[1];
        m[2] -= rhs_m[2];

        return *this;
    }

    INLINE bool Mat3::operator==(const Mat3 &rhs) const
    {
        bool result = true;
        auto &rhs_m = rhs.m;

        result = result && m[0] == rhs_m[0];
        result = result && m[1] == rhs_m[1];
        result = result && m[2] == rhs_m[2];

        return result;
    }

    INLINE bool Mat3::operator!=(const Mat3 &rhs) const
    {
        return !(*this == rhs);
    }
}
