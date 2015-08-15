/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Vector2.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <vector>

#include "JsonSerializer.h"

namespace Ursine
{
    // Forward declaration
    class Matrix3;
    class Vector3;

    class Vector2
    {
    public:
        float x, y;

        // Constructors
        Vector2(float X, float Y);
        explicit Vector2(float value);
        explicit Vector2(const Vector3 &value);
        Vector2(const Vector2 &value);
        Vector2();

        // Properties
        const static Vector2 &Zero();
        const static Vector2 &One();
        const static Vector2 &UnitX();
        const static Vector2 &UnitY();
        const static Vector2 &Up();
        const static Vector2 &Down();
        const static Vector2 &Right();
        const static Vector2 &Left();

        // Public Methods
        void Clean(); // Sets near zero elements to zero
        void Set(int x, int y);
        void Set(float x, float y);
        static Vector2 Abs(const Vector2 &value);
        static Vector2 Clamp(const Vector2 &value, const Vector2 &min, const Vector2 &max);
        static void Clamp(const Vector2 &value, const Vector2 &min, const Vector2 &max, Vector2 &result);
        static float Cross(const Vector2 &vector1, const Vector2 &vector2);
        static Vector2 Cross(const Vector2 &vector, float scalar);
        static Vector2 Cross(float scalar, const Vector2 &vector);
        static Vector2 TripleProduct(const Vector2 &a, const Vector2 &b, const Vector2 &c);
        static float Distance(const Vector2 &vector1, const Vector2 &vector2);
        static void Distance(const Vector2 &vector1, const Vector2 &vector2, float &result);
        static float DistanceSquared(const Vector2 &vector1, const Vector2 &vector2);
        static void DistanceSquared(const Vector2 &vector1, const Vector2 &vector2, float &result);
        static float Dot(const Vector2 &vector1, const Vector2 &vector2);
        static void Dot(const Vector2 &vector1, const Vector2 &vector2, float &result);
        float Length() const;
        float LengthSquared() const;
        float Angle(void) const;
        static Vector2 AngleVec(float radians);
        static Vector2 Max(const Vector2 &vector1, const Vector2 &vector2);
        static void Max(const Vector2 &vector1, const Vector2 &vector2, Vector2 &result);
        static Vector2 Min(const Vector2 &vector1, const Vector2 &vector2);
        static void Min(const Vector2 &vector1, const Vector2 &vector2, Vector2 &result);
        Vector2 &Normalize();
        static Vector2 Normalize(const Vector2 &vector);
        static void Normalize(const Vector2 &vector, Vector2 &result);
        static Vector2 Reflect(const Vector2 &vector, const Vector2 &normal);
        static void Reflect(const Vector2 &vector, const Vector2 &normal, Vector2 &result);
        static Vector2 Transform(const Vector2 &vector, bool is_point, const Matrix3 &matrix);
        static void Transform(const Vector2 &vector, bool is_point, const Matrix3 &matrix, Vector2 &result);
        static std::vector<Vector2> Transform(const std::vector<Vector2> &source, bool is_point, const Matrix3 &matrix);
        static void Transform(const std::vector<Vector2> &source, bool is_point, const Matrix3 &matrix, std::vector<Vector2> &destination);
        static std::vector<Vector2> Transform(const std::vector<Vector2> &source, bool is_point, int source_index, const Matrix3 &matrix);
        static void Transform(const std::vector<Vector2> &source, bool is_point, int source_index, const Matrix3 &matrix, std::vector<Vector2> &destination);
        static bool SameDirection(const Vector2 &vector_1, const Vector2 &vector_2);
        static bool OppositeDirection(const Vector2 &vector_1, const Vector2 &vector_2);

        // Accessors
        float &operator[](unsigned int index);
        const float &operator[](unsigned int index) const;

        // Operators
        bool operator==(const Vector2 &rhs) const;
        bool operator!=(const Vector2 &rhs) const;
        Vector2 operator+(const Vector2 &rhs) const;
        Vector2 operator-() const;
        Vector2 operator-(const Vector2 &rhs) const;
        Vector2 operator*(const Vector2 &rhs) const;
        Vector2 operator*(float rhs) const;
        friend Vector2 operator*(float lhs, const Vector2 &rhs);
        Vector2 operator/(const Vector2 &rhs) const;
        Vector2 operator/(float rhs) const;
        Vector2 &operator=(const Vector2 &rhs);
        Vector2 &operator+=(const Vector2 &rhs);
        Vector2 &operator-=(const Vector2 &rhs);
        Vector2 &operator*=(const Vector2 &rhs);
        Vector2 &operator/=(const Vector2 &rhs);
        Vector2 &operator/=(float rhs);
    };

    extern template
    Json JsonSerializer::Serialize(Vector2 &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, Vector2 &out);
}