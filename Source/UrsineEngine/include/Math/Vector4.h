/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Vector4.h
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
    class Matrix4;
    class Vector3;

    class Vector4
    {
    public:
        float x, y, z, w;

        // Constructors
        Vector4(float X, float Y, float Z, float W);
        explicit Vector4(float value);
        explicit Vector4(const Vector3 &value, float W);
        Vector4(const Vector4 &value);
        Vector4();

        // Properties
        const static Vector4 &Zero();
        const static Vector4 &One();
        const static Vector4 &UnitX();
        const static Vector4 &UnitY();
        const static Vector4 &UnitZ();
        const static Vector4 &UnitW();

        // Public Methods
        void Clean(); // Sets near zero elements to zero
        void Set(int x, int y, int z, int w);
        void Set(float x, float y, float z, float w);
        static Vector4 Clamp(const Vector4 &value, const Vector4 &min, const Vector4 &max);
        static void Clamp(const Vector4 &value, const Vector4 &min, const Vector4 &max, Vector4 &result);
        static float Distance(const Vector4 &vector1, const Vector4 &vector2);
        static void Distance(const Vector4 &vector1, const Vector4 &vector2, float &result);
        static float DistanceSquared(const Vector4 &vector1, const Vector4 &vector2);
        static void DistanceSquared(const Vector4 &vector1, const Vector4 &vector2, float &result);
        static float Dot(const Vector4 &vector1, const Vector4 &vector2);
        static void Dot(const Vector4 &vector1, const Vector4 &vector2, float &result);
        float Length() const;
        float LengthSquared() const;
        static Vector4 Max(const Vector4 &vector1, const Vector4 &vector2);
        static void Max(const Vector4 &vector1, const Vector4 &vector2, Vector4 &result);
        static Vector4 Min(const Vector4 &vector1, const Vector4 &vector2);
        static void Min(const Vector4 &vector1, const Vector4 &vector2, Vector4 &result);
        Vector4 &Normalize();
        static Vector4 Normalize(const Vector4 &vector);
        static void Normalize(const Vector4 &vector, Vector4 &result);
        static Vector4 Reflect(const Vector4 &vector, const Vector4 &normal);
        static void Reflect(const Vector4 &vector, const Vector4 &normal, Vector4 &result);
        static Vector4 Transform(const Vector4 &vector, const Matrix4 &matrix);
        static void Transform(const Vector4 &vector, const Matrix4 &matrix, Vector4 &result);
        static std::vector<Vector4> Transform(const std::vector<Vector4> &source, const Matrix4 &matrix);
        static void Transform(const std::vector<Vector4> &source, const Matrix4 &matrix, std::vector<Vector4> &destination);
        static std::vector<Vector4> Transform(const std::vector<Vector4> &source, int source_index, const Matrix4 &matrix);
        static void Transform(const std::vector<Vector4> &source, int source_index, const Matrix4 &matrix, std::vector<Vector4> &destination);

        // Accessors
        float &operator[](unsigned int index);
        const float &operator[](unsigned int index) const;

        // Operators
        bool operator==(const Vector4 &rhs) const;
        bool operator!=(const Vector4 &rhs) const;
        Vector4 operator+(const Vector4 &rhs) const;
        Vector4 operator-() const;
        Vector4 operator-(const Vector4 &rhs) const;
        Vector4 operator*(const Vector4 &rhs) const;
        Vector4 operator*(float rhs) const;
        friend Vector4 operator*(float lhs, const Vector4 &rhs);
        Vector4 operator/(const Vector4 &rhs) const;
        Vector4 operator/(float rhs) const;
        Vector4 &operator=(const Vector4 &rhs);
        Vector4 &operator+=(const Vector4 &rhs);
        Vector4 &operator-=(const Vector4 &rhs);
        Vector4 &operator*=(const Vector4 &rhs);
        Vector4 &operator/=(const Vector4 &rhs);
        Vector4 &operator/=(float rhs);
    };

    extern template
    Json JsonSerializer::Serialize(Vector4 &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, Vector4 &out);
}