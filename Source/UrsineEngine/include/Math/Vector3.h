/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Vector3.h
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
    class Vector2;

    class Vector3
    {
    public:
        float x, y, z;

        // Constructors
        Vector3(float X, float Y, float Z);
        explicit Vector3(float value);
        Vector3(const Vector2 &value, float Z);
        Vector3(const Vector3 &value);
        Vector3();

        // Properties
        const static Vector3 &Zero();
        const static Vector3 &One();
        const static Vector3 &UnitX();
        const static Vector3 &UnitY();
        const static Vector3 &UnitZ();
        const static Vector3 &Up();
        const static Vector3 &Down();
        const static Vector3 &Right();
        const static Vector3 &Left();
        const static Vector3 &Forward();
        const static Vector3 &Backward();

        // Public Methods
        void Clean(); // Sets near zero elements to zero
        void Set(int x, int y, int z);
        void Set(float x, float y, float z);
        static Vector3 Clamp(const Vector3 &value, const Vector3 &min, const Vector3 &max);
        static void Clamp(const Vector3 &value, const Vector3 &min, const Vector3 &max, Vector3 &result);
        static Vector3 Cross(const Vector3 &vector1, const Vector3 &vector2);
        static void Cross(const Vector3 &vector1, const Vector3 &vector2, Vector3 &result);
        static float Distance(const Vector3 &vector1, const Vector3 &vector2);
        static void Distance(const Vector3 &vector1, const Vector3 &vector2, float &result);
        static float DistanceSquared(const Vector3 &vector1, const Vector3 &vector2);
        static void DistanceSquared(const Vector3 &vector1, const Vector3 &vector2, float &result);
        static float Dot(const Vector3 &vector1, const Vector3 &vector2);
        static void Dot(const Vector3 &vector1, const Vector3 &vector2, float &result);
        float Length() const;
        float LengthSquared() const;
        static Vector3 Max(const Vector3 &vector1, const Vector3 &vector2);
        static void Max(const Vector3 &vector1, const Vector3 &vector2, Vector3 &result);
        static Vector3 Min(const Vector3 &vector1, const Vector3 &vector2);
        static void Min(const Vector3 &vector1, const Vector3 &vector2, Vector3 &result);
        Vector3 &Normalize();
        static Vector3 Normalize(const Vector3 &vector);
        static void Normalize(const Vector3 &vector, Vector3 &result);
        static Vector3 Reflect(const Vector3 &vector, const Vector3 &normal);
        static void Reflect(const Vector3 &vector, const Vector3 &normal, Vector3 &result);
        static Vector3 Transform(const Vector3 &vector, const Matrix3 &matrix);
        static void Transform(const Vector3 &vector, const Matrix3 &matrix, Vector3 &result);
        static std::vector<Vector3> Transform(const std::vector<Vector3> &source, const Matrix3 &matrix);
        static void Transform(const std::vector<Vector3> &source, const Matrix3 &matrix, std::vector<Vector3> &destination);
        static std::vector<Vector3> Transform(const std::vector<Vector3> &source, int source_index, const Matrix3 &matrix);
        static void Transform(const std::vector<Vector3> &source, int source_index, const Matrix3 &matrix, std::vector<Vector3> &destination);

        // Accessors
        float &operator[](unsigned int index);
        const float &operator[](unsigned int index) const;

        // Operators
        bool operator==(const Vector3 &rhs) const;
        bool operator!=(const Vector3 &rhs) const;
        Vector3 operator+(const Vector3 &rhs) const;
        Vector3 operator-() const;
        Vector3 operator-(const Vector3 &rhs) const;
        Vector3 operator*(const Vector3 &rhs) const;
        Vector3 operator*(float rhs) const;
        friend Vector3 operator*(float lhs, const Vector3 &rhs);
        Vector3 operator/(const Vector3 &rhs) const;
        Vector3 operator/(float rhs) const;
        Vector3 &operator=(const Vector3 &rhs);
        Vector3 &operator+=(const Vector3 &rhs);
        Vector3 &operator-=(const Vector3 &rhs);
        Vector3 &operator*=(const Vector3 &rhs);
        Vector3 &operator/=(const Vector3 &rhs);
        Vector3 &operator/=(float rhs);
    };

    extern template
    Json JsonSerializer::Serialize(Vector3 &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, Vector3 &out);
};