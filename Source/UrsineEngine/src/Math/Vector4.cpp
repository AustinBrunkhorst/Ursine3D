/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Vector4.cpp
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
    Vector4::Vector4(float X, float Y, float Z, float W)
        : x(X)
        , y(Y)
        , z(Z)
        , w(W) {}

    Vector4::Vector4(float value)
        : x(value)
        , y(value)
        , z(value)
        , w(value) {}

    Vector4::Vector4(const Vector3 &value, float W)
        : x(value.x)
        , y(value.y)
        , z(value.z)
        , w(W) {}

    Vector4::Vector4(const Vector4 &value)
        : x(value.x)
        , y(value.y)
        , z(value.z)
        , w(value.w) {}

    Vector4::Vector4()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(0.0f) {}

    const Vector4 &Vector4::Zero()
    {
        const static Vector4 zero = Vector4();
        return zero;
    }

    const Vector4 &Vector4::One()
    {
        const static Vector4 one = Vector4(1);
        return one;
    }

    const Vector4 &Vector4::UnitX()
    {
        const static Vector4 unit_x = Vector4(1, 0, 0, 0);
        return unit_x;
    }

    const Vector4 &Vector4::UnitY()
    {
        const static Vector4 unit_y = Vector4(0, 1, 0, 0);
        return unit_y;
    }

    const Vector4 &Vector4::UnitZ()
    {
        const static Vector4 unit_z = Vector4(0, 0, 1, 0);
        return unit_z;
    }

    const Vector4 &Vector4::UnitW()
    {
        const static Vector4 unit_w = Vector4(0, 0, 0, 1);
        return unit_w;
    }

    float &Vector4::operator[](unsigned int index)
    {
        return (&x)[index];
    }

    const float &Vector4::operator[](unsigned int index) const
    {
        return (&x)[index];
    }

    // Public Methods
    void Vector4::Clean()
    {
        x = Math::IsZero(x) ? 0.0f : x;
        y = Math::IsZero(y) ? 0.0f : y;
        z = Math::IsZero(z) ? 0.0f : z;
        w = Math::IsZero(w) ? 0.0f : w;
    }

    void Vector4::Set(int x, int y, int z, int w)
    {
        this->x = static_cast<float>(x);
        this->y = static_cast<float>(y);
        this->z = static_cast<float>(z);
        this->w = static_cast<float>(w);
    }

    void Vector4::Set(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vector4 Vector4::Clamp(const Vector4 &value, const Vector4 &min, const Vector4 &max)
    {
        return Vector4(
            Math::Clamp(value.x, min.x, max.x),
            Math::Clamp(value.y, min.y, max.y),
            Math::Clamp(value.z, min.z, max.z),
            Math::Clamp(value.w, min.w, max.w));
    }

    void Vector4::Clamp(const Vector4 &value, const Vector4 &min, const Vector4 &max, Vector4 &result)
    {
        result.x = Math::Clamp(value.x, min.x, max.x);
        result.y = Math::Clamp(value.y, min.y, max.y);
        result.z = Math::Clamp(value.z, min.z, max.z);
        result.w = Math::Clamp(value.w, min.w, max.w);
    }

    float Vector4::Distance(const Vector4 &vector1, const Vector4 &vector2)
    {
        return Math::fastSqrt(DistanceSquared(vector1, vector2));
    }

    void Vector4::Distance(const Vector4 &vector1, const Vector4 &vector2, float &result)
    {
        result = Math::fastSqrt(DistanceSquared(vector1, vector2));
    }

    float Vector4::DistanceSquared(const Vector4 &vector1, const Vector4 &vector2)
    {
        float a = vector1.x - vector2.x, b = vector1.y - vector2.y,
            c = vector1.z - vector2.z, d = vector1.w - vector2.w;

        return a * a + b * b + c * c + d * d;
    }

    void Vector4::DistanceSquared(const Vector4 &vector1, const Vector4 &vector2, float &result)
    {
        result = DistanceSquared(vector1, vector2);
    }

    float Vector4::Dot(const Vector4 &vector1, const Vector4 &vector2)
    {
        return vector1.x * vector2.x + vector1.y * vector2.y +
            vector1.z * vector2.z + vector1.w * vector2.w;
    }

    void Vector4::Dot(const Vector4 &vector1, const Vector4 &vector2, float &result)
    {
        result = Dot(vector1, vector2);
    }

    float Vector4::Length() const
    {
        return Math::fastSqrt(LengthSquared());
    }

    float Vector4::LengthSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    Vector4 Vector4::Max(const Vector4 &vector1, const Vector4 &vector2)
    {
        return Vector4(
            Math::Max(vector1.x, vector2.x),
            Math::Max(vector1.y, vector2.y),
            Math::Max(vector1.z, vector2.z),
            Math::Max(vector1.w, vector2.w));
    }

    void Vector4::Max(const Vector4 &vector1, const Vector4 &vector2, Vector4 &result)
    {
        result.x = Math::Max(vector1.x, vector2.x);
        result.y = Math::Max(vector1.y, vector2.y);
        result.z = Math::Max(vector1.z, vector2.z);
        result.w = Math::Max(vector1.w, vector2.w);
    }

    Vector4 Vector4::Min(const Vector4 &vector1, const Vector4 &vector2)
    {
        return Vector4(
            Math::Min(vector1.x, vector2.x),
            Math::Min(vector1.y, vector2.y),
            Math::Min(vector1.z, vector2.z),
            Math::Min(vector1.w, vector2.w));
    }

    void Vector4::Min(const Vector4 &vector1, const Vector4 &vector2, Vector4 &result)
    {
        result.x = Math::Min(vector1.x, vector2.x);
        result.y = Math::Min(vector1.y, vector2.y);
        result.z = Math::Min(vector1.z, vector2.z);
        result.w = Math::Min(vector1.w, vector2.w);
    }

    Vector4 &Vector4::Normalize()
    {
        (*this) /= Length();
        return (*this);
    }

    Vector4 Vector4::Normalize(const Vector4 &vector)
    {
        return vector / vector.Length();
    }

    void Vector4::Normalize(const Vector4 &vector, Vector4 &result)
    {
        result = vector / vector.Length();
    }

    Vector4 Vector4::Reflect(const Vector4 &vector, const Vector4 &normal)
    {
        return Vector4(vector - (((2 * Dot(vector, normal)) / normal.LengthSquared()) * normal));
    }

    void Vector4::Reflect(const Vector4 &vector, const Vector4 &normal, Vector4 &result)
    {
        result = vector - (((2 * Dot(vector, normal)) / normal.LengthSquared()) * normal);
    }

    Vector4 Vector4::Transform(const Vector4 &vector, const Matrix4 &matrix)
    {
        return matrix * vector;
    }

    static void Transform(const Vector4 &vector, const Matrix4 &matrix, Vector4 &result)
    {
        result = matrix * vector;
    }

    static std::vector<Vector4> Transform(const std::vector<Vector4> &source, const Matrix4 &matrix)
    {
        std::vector<Vector4> dest;

        for (auto i = source.begin(); i != source.end(); ++i)
        {
            dest.push_back(Vector4::Transform(*i, matrix));
        }

        return dest;
    }

    static void Transform(const std::vector<Vector4> &source, const Matrix4 &matrix, std::vector<Vector4> &destination)
    {
        auto j = destination.begin();

        for (auto i = source.begin(); i != source.end(); ++i , ++j)
        {
            Vector4::Transform(*i, matrix, *j);
        }
    }

    static std::vector<Vector4> Transform(const std::vector<Vector4> &source, int source_index, const Matrix4 &matrix)
    {
        std::vector<Vector4> dest;

        for (auto i = source.begin() + source_index; i != source.end(); ++i)
        {
            dest.push_back(Vector4::Transform(*i, matrix));
        }

        return dest;
    }

    static void Transform(const std::vector<Vector4> &source, int source_index, const Matrix4 &matrix, std::vector<Vector4> &destination)
    {
        auto j = destination.begin();

        for (auto i = source.begin() + source_index; i != source.end(); ++i , ++j)
        {
            Vector4::Transform(*i, matrix, *j);
        }
    }

    // Operators
    bool Vector4::operator==(const Vector4 &rhs) const
    {
        return Math::IsEqual(x, rhs.x) &&
            Math::IsEqual(y, rhs.y) &&
            Math::IsEqual(z, rhs.z) &&
            Math::IsEqual(w, rhs.w);
    }

    bool Vector4::operator!=(const Vector4 &rhs) const
    {
        return !(*this == rhs);
    }

    Vector4 Vector4::operator+(const Vector4 &rhs) const
    {
        return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vector4 Vector4::operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }

    Vector4 Vector4::operator-(const Vector4 &rhs) const
    {
        return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vector4 Vector4::operator*(const Vector4 &rhs) const
    {
        return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    Vector4 Vector4::operator*(float rhs) const
    {
        return Vector4(x * rhs, y * rhs, z * rhs, w * rhs);
    }

    Vector4 operator*(float lhs, const Vector4 &rhs)
    {
        return Vector4(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs, rhs.w * lhs);
    }

    Vector4 Vector4::operator/(const Vector4 &rhs) const
    {
        return Vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
    }

    Vector4 Vector4::operator/(float rhs) const
    {
        return Vector4(x / rhs, y / rhs, z / rhs, w / rhs);
    }

    Vector4 &Vector4::operator=(const Vector4 &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return (*this);
    }

    Vector4 &Vector4::operator+=(const Vector4 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;

        return (*this);
    }

    Vector4 &Vector4::operator-=(const Vector4 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;

        return (*this);
    }

    Vector4 &Vector4::operator*=(const Vector4 &rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;

        return (*this);
    }

    Vector4 &Vector4::operator/=(const Vector4 &rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;

        return (*this);
    }

    Vector4 &Vector4::operator/=(float rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;

        return *this;
    }

    template<>
    Json JsonSerializer::Serialize(Vector4 &instance)
    {
        return Json::object{
                { "x", instance.x },
                { "y", instance.y },
                { "z", instance.z },
                { "w", instance.w }
        };
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, Vector4 &out)
    {
        out.x = static_cast<float>(data["x"].number_value());
        out.y = static_cast<float>(data["y"].number_value());
        out.z = static_cast<float>(data["z"].number_value());
        out.w = static_cast<float>(data["w"].number_value());
    }
}