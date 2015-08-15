/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Vector3.cpp
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
    Vector3::Vector3(float X, float Y, float Z)
        : x(X)
        , y(Y)
        , z(Z) {}

    Vector3::Vector3(float value)
        : x(value)
        , y(value)
        , z(value) {}

    Vector3::Vector3(const Vector2 &value, float Z)
        : x(value.x)
        , y(value.y)
        , z(Z) {}

    Vector3::Vector3(const Vector3 &value)
        : x(value.x)
        , y(value.y)
        , z(value.z) {}

    Vector3::Vector3()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f) {}

    const Vector3 &Vector3::Zero()
    {
        const static Vector3 zero = Vector3();
        return zero;
    }

    const Vector3 &Vector3::One()
    {
        const static Vector3 one = Vector3(1);
        return one;
    }

    const Vector3 &Vector3::UnitX()
    {
        const static Vector3 unit_x = Vector3(1, 0, 0);
        return unit_x;
    }

    const Vector3 &Vector3::UnitY()
    {
        const static Vector3 unit_y = Vector3(0, 1, 0);
        return unit_y;
    }

    const Vector3 &Vector3::UnitZ()
    {
        const static Vector3 unit_z = Vector3(0, 0, 1);
        return unit_z;
    }

    const Vector3 &Vector3::Up()
    {
        const static Vector3 up = Vector3(0, 1, 0);
        return up;
    }

    const Vector3 &Vector3::Down()
    {
        const static Vector3 down = Vector3(0, -1, 0);
        return down;
    }

    const Vector3 &Vector3::Right()
    {
        const static Vector3 right = Vector3(1, 0, 0);
        return right;
    }

    const Vector3 &Vector3::Left()
    {
        const static Vector3 left = Vector3(-1, 0, 0);
        return left;
    }

    const Vector3 &Vector3::Forward()
    {
        const static Vector3 forward = Vector3(0, 0, 1);
        return forward;
    }

    const Vector3 &Vector3::Backward()
    {
        const static Vector3 backward = Vector3(0, 0, -1);
        return backward;
    }

    float &Vector3::operator[](unsigned int index)
    {
        return (&x)[index];
    }

    const float &Vector3::operator[](unsigned int index) const
    {
        return (&x)[index];
    }

    // Public Methods
    void Vector3::Clean()
    {
        x = Math::IsZero(x) ? 0.0f : x;
        y = Math::IsZero(y) ? 0.0f : y;
        z = Math::IsZero(z) ? 0.0f : z;
    }

    void Vector3::Set(int x, int y, int z)
    {
        this->x = static_cast<float>(x);
        this->y = static_cast<float>(y);
        this->z = static_cast<float>(z);
    }

    void Vector3::Set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3 Vector3::Clamp(const Vector3 &value, const Vector3 &min, const Vector3 &max)
    {
        return Vector3(
            Math::Clamp(value.x, min.x, max.x),
            Math::Clamp(value.y, min.y, max.y),
            Math::Clamp(value.z, min.z, max.z));
    }

    void Vector3::Clamp(const Vector3 &value, const Vector3 &min, const Vector3 &max, Vector3 &result)
    {
        result.x = Math::Clamp(value.x, min.x, max.x);
        result.y = Math::Clamp(value.y, min.y, max.y);
        result.z = Math::Clamp(value.z, min.z, max.z);
    }

    Vector3 Vector3::Cross(const Vector3 &vector1, const Vector3 &vector2)
    {
        return Vector3(
            vector1.y * vector2.z - vector2.y * vector1.z,
            vector1.z * vector2.x - vector2.z * vector1.x,
            vector1.x * vector2.y - vector2.x * vector1.y);
    }

    void Vector3::Cross(const Vector3 &vector1, const Vector3 &vector2, Vector3 &result)
    {
        result.x = vector1.y * vector2.z - vector2.y * vector1.z;
        result.y = vector1.x * vector2.z - vector2.x * vector1.z;
        result.z = vector1.x * vector2.y - vector2.x * vector1.y;
    }

    float Vector3::Distance(const Vector3 &vector1, const Vector3 &vector2)
    {
        return Math::fastSqrt(DistanceSquared(vector1, vector2));
    }

    void Vector3::Distance(const Vector3 &vector1, const Vector3 &vector2, float &result)
    {
        result = Math::fastSqrt(DistanceSquared(vector1, vector2));
    }

    float Vector3::DistanceSquared(const Vector3 &vector1, const Vector3 &vector2)
    {
        float a = vector1.x - vector2.x,
            b = vector1.y - vector2.y,
            c = vector1.z - vector2.z;

        return a * a + b * b + c * c;
    }

    void Vector3::DistanceSquared(const Vector3 &vector1, const Vector3 &vector2, float &result)
    {
        result = DistanceSquared(vector1, vector2);
    }

    float Vector3::Dot(const Vector3 &vector1, const Vector3 &vector2)
    {
        return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
    }

    void Vector3::Dot(const Vector3 &vector1, const Vector3 &vector2, float &result)
    {
        result = Dot(vector1, vector2);
    }

    float Vector3::Length() const
    {
        return Math::fastSqrt(LengthSquared());
    }

    float Vector3::LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    Vector3 Vector3::Max(const Vector3 &vector1, const Vector3 &vector2)
    {
        return Vector3(
            Math::Max(vector1.x, vector2.x),
            Math::Max(vector1.y, vector2.y),
            Math::Max(vector1.z, vector2.z));
    }

    void Vector3::Max(const Vector3 &vector1, const Vector3 &vector2, Vector3 &result)
    {
        result.x = Math::Max(vector1.x, vector2.x);
        result.y = Math::Max(vector1.y, vector2.y);
        result.z = Math::Max(vector1.z, vector2.z);
    }

    Vector3 Vector3::Min(const Vector3 &vector1, const Vector3 &vector2)
    {
        return Vector3(
            Math::Min(vector1.x, vector2.x),
            Math::Min(vector1.y, vector2.y),
            Math::Min(vector1.z, vector2.z));
    }

    void Vector3::Min(const Vector3 &vector1, const Vector3 &vector2, Vector3 &result)
    {
        result.x = Math::Min(vector1.x, vector2.x);
        result.y = Math::Min(vector1.y, vector2.y);
        result.z = Math::Min(vector1.z, vector2.z);
    }

    Vector3 &Vector3::Normalize()
    {
        (*this) /= Length();
        return (*this);
    }

    Vector3 Vector3::Normalize(const Vector3 &vector)
    {
        return vector / vector.Length();
    }

    void Vector3::Normalize(const Vector3 &vector, Vector3 &result)
    {
        result = vector / vector.Length();
    }

    Vector3 Vector3::Reflect(const Vector3 &vector, const Vector3 &normal)
    {
        return Vector3(vector - (((2 * Dot(vector, normal)) / normal.LengthSquared()) * normal));
    }

    void Vector3::Reflect(const Vector3 &vector, const Vector3 &normal, Vector3 &result)
    {
        result = vector - (((2 * Dot(vector, normal)) / normal.LengthSquared()) * normal);
    }

    Vector3 Vector3::Transform(const Vector3 &vector, const Matrix3 &matrix)
    {
        return matrix * vector;
    }

    static void Transform(const Vector3 &vector, const Matrix3 &matrix, Vector3 &result)
    {
        result = matrix * vector;
    }

    static std::vector<Vector3> Transform(const std::vector<Vector3> &source, const Matrix3 &matrix)
    {
        std::vector<Vector3> dest;

        for (auto i = source.begin(); i != source.end(); ++i)
        {
            dest.push_back(Vector3::Transform(*i, matrix));
        }

        return dest;
    }

    static void Transform(const std::vector<Vector3> &source, const Matrix3 &matrix, std::vector<Vector3> &destination)
    {
        auto j = destination.begin();

        for (auto i = source.begin(); i != source.end(); ++i , ++j)
        {
            Vector3::Transform(*i, matrix, *j);
        }
    }

    static std::vector<Vector3> Transform(const std::vector<Vector3> &source, int source_index, const Matrix3 &matrix)
    {
        std::vector<Vector3> dest;

        for (auto i = source.begin() + source_index; i != source.end(); ++i)
        {
            dest.push_back(Vector3::Transform(*i, matrix));
        }

        return dest;
    }

    static void Transform(const std::vector<Vector3> &source, int source_index, const Matrix3 &matrix, std::vector<Vector3> &destination)
    {
        auto j = destination.begin();

        for (auto i = source.begin() + source_index; i != source.end(); ++i , ++j)
        {
            Vector3::Transform(*i, matrix, *j);
        }
    }

    // Operators
    bool Vector3::operator==(const Vector3 &rhs) const
    {
        return Math::IsEqual(x, rhs.x) &&
            Math::IsEqual(y, rhs.y) &&
            Math::IsEqual(z, rhs.z);
    }

    bool Vector3::operator!=(const Vector3 &rhs) const
    {
        return !(*this == rhs);
    }

    Vector3 Vector3::operator+(const Vector3 &rhs) const
    {
        return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vector3 Vector3::operator-() const
    {
        return Vector3(-x, -y, -z);
    }

    Vector3 Vector3::operator-(const Vector3 &rhs) const
    {
        return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vector3 Vector3::operator*(const Vector3 &rhs) const
    {
        return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    Vector3 Vector3::operator*(float rhs) const
    {
        return Vector3(x * rhs, y * rhs, z * rhs);
    }

    Vector3 operator*(float lhs, const Vector3 &rhs)
    {
        return Vector3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
    }

    Vector3 Vector3::operator/(const Vector3 &rhs) const
    {
        return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    Vector3 Vector3::operator/(float rhs) const
    {
        return Vector3(x / rhs, y / rhs, z / rhs);
    }

    Vector3 &Vector3::operator=(const Vector3 &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;

        return (*this);
    }

    Vector3 &Vector3::operator+=(const Vector3 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return (*this);
    }

    Vector3 &Vector3::operator-=(const Vector3 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

        return (*this);
    }

    Vector3 &Vector3::operator*=(const Vector3 &rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;

        return (*this);
    }

    Vector3 &Vector3::operator/=(const Vector3 &rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;

        return (*this);
    }

    Vector3 &Vector3::operator/=(float rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;

        return *this;
    }

    template<>
    Json JsonSerializer::Serialize(Vector3 &instance)
    {
        return Json::object{
                { "x", instance.x },
                { "y", instance.y },
                { "z", instance.z }
        };
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, Vector3 &out)
    {
        out.x = static_cast<float>(data["x"].number_value());
        out.y = static_cast<float>(data["y"].number_value());
        out.z = static_cast<float>(data["z"].number_value());
    }
}