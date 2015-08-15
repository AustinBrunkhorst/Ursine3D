/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Vector2.cpp
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
    Vector2::Vector2(float X, float Y)
        : x(X)
        , y(Y) {}

    Vector2::Vector2(float value)
        : x(value)
        , y(value) {}

    Vector2::Vector2(const Vector3 &value)
        : x(value.x)
        , y(value.y) {}

    Vector2::Vector2(const Vector2 &value)
        : x(value.x)
        , y(value.y) {}

    Vector2::Vector2()
        : x(0.0f)
        , y(0.0f) {}

    const Vector2 &Vector2::Zero()
    {
        const static Vector2 zero = Vector2();
        return zero;
    }

    const Vector2 &Vector2::One()
    {
        const static Vector2 one = Vector2(1);
        return one;
    }

    const Vector2 &Vector2::UnitX()
    {
        const static Vector2 unit_x = Vector2(1, 0);
        return unit_x;
    }

    const Vector2 &Vector2::UnitY()
    {
        const static Vector2 unit_y = Vector2(0, 1);
        return unit_y;
    }

    const Vector2 &Vector2::Up()
    {
        const static Vector2 up = Vector2(0, 1);
        return up;
    }

    const Vector2 &Vector2::Down()
    {
        const static Vector2 down = Vector2(0, -1);
        return down;
    }

    const Vector2 &Vector2::Right()
    {
        const static Vector2 right = Vector2(1, 0);
        return right;
    }

    const Vector2 &Vector2::Left()
    {
        const static Vector2 left = Vector2(-1, 0);
        return left;
    }

    float &Vector2::operator[](unsigned int index)
    {
        return (&x)[index];
    }

    const float &Vector2::operator[](unsigned int index) const
    {
        return (&x)[index];
    }

    // Public Methods
    void Vector2::Clean()
    {
        x = Math::IsZero(x) ? 0.0f : x;
        y = Math::IsZero(y) ? 0.0f : y;
    }

    void Vector2::Set(int x, int y)
    {
        this->x = static_cast<float>(x);
        this->y = static_cast<float>(y);
    }

    void Vector2::Set(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Vector2 Vector2::Clamp(const Vector2 &value, const Vector2 &min, const Vector2 &max)
    {
        return Vector2(
            Math::Clamp(value.x, min.x, max.x),
            Math::Clamp(value.y, min.y, max.y));
    }

    void Vector2::Clamp(const Vector2 &value, const Vector2 &min, const Vector2 &max, Vector2 &result)
    {
        result.x = Math::Clamp(value.x, min.x, max.x);
        result.y = Math::Clamp(value.x, min.y, max.y);
    }

    float Vector2::Cross(const Vector2 &vector1, const Vector2 &vector2)
    {
        return vector1.x * vector2.y - vector1.y * vector2.x;
    }

    Vector2 Vector2::Cross(const Vector2 &vector, float scalar)
    {
        return Vector2(scalar * vector.y, -scalar * vector.x);
    }
    
    Vector2 Vector2::Cross(float scalar, const Vector2 &vector)
    {
        return Vector2(-scalar * vector.y, scalar * vector.x);
    }

    Vector2 Vector2::TripleProduct(Vector2 const& a, Vector2 const& b, Vector2 const& c)
    {
        // shortcut for triple cross product
        // (A x B) x C == B * A.Dot(C) - C * A.Dot(B)
        return (b * Dot(a, c)) - (c * Dot(a, b));
    }

    float Vector2::Distance(const Vector2 &vector1, const Vector2 &vector2)
    {
        return Math::fastSqrt(DistanceSquared(vector1, vector2));
    }

    void Vector2::Distance(const Vector2 &vector1, const Vector2 &vector2, float &result)
    {
        result = Math::fastSqrt(DistanceSquared(vector1, vector2));
    }

    float Vector2::DistanceSquared(const Vector2 &vector1, const Vector2 &vector2)
    {
        float a = vector1.x - vector2.x, b = vector1.y - vector2.y;

        return a * a + b * b;
    }

    void Vector2::DistanceSquared(const Vector2 &vector1, const Vector2 &vector2, float &result)
    {
        result = DistanceSquared(vector1, vector2);
    }

    float Vector2::Dot(const Vector2 &vector1, const Vector2 &vector2)
    {
        return vector1.x * vector2.x + vector1.y * vector2.y;
    }

    void Vector2::Dot(const Vector2 &vector1, const Vector2 &vector2, float &result)
    {
        result = Dot(vector1, vector2);
    }

    float Vector2::Length() const
    {
        float lenSquared = LengthSquared();
        return lenSquared > 0 ? sqrt(lenSquared) : 0.0f;
    }

    float Vector2::LengthSquared() const
    {
        return x * x + y * y;
    }

    float Vector2::Angle(void) const
    {
        float angle = atan2f(y, x);
        
        angle = Math::Wrap(angle, 0.0f, Math::PI_2);

        return angle;
    }

    Vector2 Vector2::AngleVec(float radians)
    {
        radians = Math::Wrap(radians, 0.0f, Math::PI_2);

        return{ cos(radians), sin(radians) };
    }

    Vector2 Vector2::Max(const Vector2 &vector1, const Vector2 &vector2)
    {
        return Vector2(
            Math::Max(vector1.x, vector2.x),
            Math::Max(vector1.y, vector2.y));
    }

    void Vector2::Max(const Vector2 &vector1, const Vector2 &vector2, Vector2 &result)
    {
        result.x = Math::Max(vector1.x, vector2.x);
        result.y = Math::Max(vector1.y, vector2.y);
    }

    Vector2 Vector2::Min(const Vector2 &vector1, const Vector2 &vector2)
    {
        return Vector2(
            Math::Min(vector1.x, vector2.x),
            Math::Min(vector1.y, vector2.y));
    }

    void Vector2::Min(const Vector2 &vector1, const Vector2 &vector2, Vector2 &result)
    {
        result.x = Math::Min(vector1.x, vector2.x);
        result.y = Math::Min(vector1.y, vector2.y);
    }

    Vector2 &Vector2::Normalize()
    {
        float len = Length();
        if (len != 0.f)
        {
            (*this) /= Length();
        }
        return (*this);
    }

    Vector2 Vector2::Normalize(const Vector2 &vector)
    {
        return vector / vector.Length();
    }

    void Vector2::Normalize(const Vector2 &vector, Vector2 &result)
    {
        result = vector / vector.Length();
    }

    Vector2 Vector2::Reflect(const Vector2 &vector, const Vector2 &normal)
    {
        return Vector2(vector - (((2 * Dot(vector, normal)) / normal.LengthSquared()) * normal));
    }

    void Vector2::Reflect(const Vector2 &vector, const Vector2 &normal, Vector2 &result)
    {
        result = vector - (((2 * Dot(vector, normal)) / normal.LengthSquared()) * normal);
    }

    Vector2 Vector2::Transform(const Vector2 &vector, bool is_point, const Matrix3 &matrix)
    {
        if (is_point)
            return matrix.TransformPoint(vector);
        else
            return matrix.TransformVector(vector);
    }

    void Vector2::Transform(const Vector2 &vector, bool is_point, const Matrix3 &matrix, Vector2 &result)
    {
        if (is_point)
            result = matrix.TransformPoint(vector);
        else
            result = matrix.TransformVector(vector);
    }

    std::vector<Vector2> Vector2::Transform(const std::vector<Vector2> &source, bool is_point, const Matrix3 &matrix)
    {
        std::vector<Vector2> dest;
        auto TransformFunc = is_point ? &Matrix3::TransformPoint : &Matrix3::TransformVector;

        for (auto i = source.begin(); i != source.end(); ++i)
        {
            dest.push_back((matrix .* TransformFunc)(*i));
        }

        return dest;
    }

    void Vector2::Transform(const std::vector<Vector2> &source, bool is_point, const Matrix3 &matrix, std::vector<Vector2> &destination)
    {
        auto TransformFunc = is_point ? &Matrix3::TransformPoint : &Matrix3::TransformVector;
        destination.clear();

        for (auto i = source.begin(); i != source.end(); ++i)
        {
            destination.push_back((matrix .* TransformFunc)(*i));
        }
    }

    std::vector<Vector2> Vector2::Transform(const std::vector<Vector2> &source, bool is_point, int source_index, const Matrix3 &matrix)
    {
        auto TransformFunc = is_point ? &Matrix3::TransformPoint : &Matrix3::TransformVector;
        std::vector<Vector2> dest;

        for (auto i = source.begin() + source_index; i != source.end(); ++i)
        {
            dest.push_back((matrix .* TransformFunc)(*i));
        }

        return dest;
    }

    void Vector2::Transform(const std::vector<Vector2> &source, bool is_point, int source_index, const Matrix3 &matrix, std::vector<Vector2> &destination)
    {
        auto TransformFunc = is_point ? &Matrix3::TransformPoint : &Matrix3::TransformVector;
        destination.clear();

        for (auto i = source.begin() + source_index; i != source.end(); ++i)
        {
            destination.push_back((matrix .* TransformFunc)(*i));
        }
    }

    bool Vector2::SameDirection(Vector2 const& vector_1, Vector2 const& vector_2)
    {
        return Vector2::Dot(vector_1, vector_2) > 0;
    }

    bool Vector2::OppositeDirection(Vector2 const& vector_1, Vector2 const& vector_2)
    {
        return Vector2::Dot(vector_1, vector_2) < 0;
    }

    // Operators
    bool Vector2::operator==(const Vector2 &rhs) const
    {
        return Math::IsEqual(x, rhs.x) &&
            Math::IsEqual(y, rhs.y);
    }

    bool Vector2::operator!=(const Vector2 &rhs) const
    {
        return !(*this == rhs);
    }

    Vector2 Vector2::operator+(const Vector2 &rhs) const
    {
        return Vector2(x + rhs.x, y + rhs.y);
    }

    Vector2 Vector2::operator-() const
    {
        return Vector2(-x, -y);
    }

    Vector2 Vector2::operator-(const Vector2 &rhs) const
    {
        return Vector2(x - rhs.x, y - rhs.y);
    }

    Vector2 Vector2::operator*(const Vector2 &rhs) const
    {
        return Vector2(x * rhs.x, y * rhs.y);
    }

    Vector2 Vector2::operator*(float rhs) const
    {
        return Vector2(x * rhs, y * rhs);
    }

    Vector2 operator*(float lhs, const Vector2 &rhs)
    {
        return Vector2(rhs.x * lhs, rhs.y * lhs);
    }

    Vector2 Vector2::operator/(const Vector2 &rhs) const
    {
        return Vector2(x / rhs.x, y / rhs.y);
    }

    Vector2 Vector2::operator/(const float rhs) const
    {
        return Vector2(x / rhs, y / rhs);
    }

    Vector2 &Vector2::operator=(const Vector2 &rhs)
    {
        x = rhs.x;
        y = rhs.y;

        return (*this);
    }

    Vector2 &Vector2::operator+=(const Vector2 &rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return (*this);
    }

    Vector2 &Vector2::operator-=(const Vector2 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;

        return (*this);
    }

    Vector2 &Vector2::operator*=(const Vector2 &rhs)
    {
        x *= rhs.x;
        y *= rhs.y;

        return (*this);
    }

    Vector2 &Vector2::operator/=(const Vector2 &rhs)
    {
        x /= rhs.x;
        y /= rhs.y;

        return (*this);
    }

    Vector2 &Vector2::operator/=(float rhs)
    {
        x /= rhs;
        y /= rhs;

        return *this;
    }

    Ursine::Vector2 Vector2::Abs(const Vector2 &value)
    {
        return { abs(value.x), abs(value.y) };
    }

    template<>
    Json JsonSerializer::Serialize(Vector2 &instance)
    {
        return Json::object{
                { "x", instance.x },
                { "y", instance.y }
        };
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, Vector2 &out)
    {
        out.x = static_cast<float>(data["x"].number_value());
        out.y = static_cast<float>(data["y"].number_value());
    }
}