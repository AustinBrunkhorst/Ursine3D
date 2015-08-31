/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Vec2.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineTypes.h"
#include "SIMD.h"
#include <string>

namespace Ursine
{
    // Forward declaration
    class Vec3;
	class SVec3;
	class Vec4;
	class SVec4;

    class Vec2
    {
        friend class Vec3;
		friend class SVec3;
        friend class Vec4;
		friend class SVec4;

        float x, y;

    public:
        // Constructors
        Vec2(void);
        Vec2(const Vec2 &value);
        Vec2(float X, float Y);
        explicit Vec2(float value);
        explicit Vec2(const Vec3 &value);
		explicit Vec2(const SVec3 &value);
        
        // Properties
        static const Vec2 &Zero(void);
        static const Vec2 &One(void);
        static const Vec2 &UnitX(void);
        static const Vec2 &UnitY(void);

        // Public Methods
        void Clean(void); // sets all near zero elements to zero

        void Set(float X, float Y);

        void Clamp(const Vec2 &min, const Vec2 &max);
        static Vec2 Clamp(const Vec2 &value, const Vec2 &min, const Vec2 &max);

        float Cross(const Vec2 &other) const;
        void Cross(float value);
        static float Cross(const Vec2 &vec1, const Vec2 &vec2);
        static Vec2 Cross(float value, const Vec2 &vec);

		static Vec2 TripleProduct(const Vec2 &vec1, const Vec2 &vec2, const Vec2 &vec3);

        float Distance(const Vec2 &other) const;
        static float Distance(const Vec2 &vec1, const Vec2 &vec2);

        float DistanceSquared(const Vec2 &other) const;
        static float DistanceSquared(const Vec2 &vec1, const Vec2 &vec2);

        float Dot(const Vec2 &other) const;
        static float Dot(const Vec2 &vec1, const Vec2 &vec2);

        float Length(void) const;
        static float Length(const Vec2 &vec);

        float LengthSquared(void) const;
        static float LengthSquared(const Vec2 &vec);

        float Angle(void) const;
        static float Angle(const Vec2 &vec);

        static Vec2 AngleVec(float radians);

        void Max(const Vec2 &other);
        static Vec2 Max(const Vec2 &vec1, const Vec2 &vec2);

        void Min(const Vec2 &other);
        static Vec2 Min(const Vec2 &vec1, const Vec2 &vec2);

        void Normalize(void);
        static Vec2 Normalize(const Vec2 &vec);

        void Reflect(const Vec2 &normal);
        static Vec2 Reflect(const Vec2 &vec, const Vec2 &normal);

        bool SameDirection(const Vec2 &other) const;
        static bool SameDirection(const Vec2 &vec1, const Vec2 &vec2);

        bool OppositeDirection(const Vec2 &other) const;
        static bool OppositeDirection(const Vec2 &vec1, const Vec2 &vec2);

        std::string ToString(void) const;

        // Accessors
        float X(void) const;
        float Y(void) const;
        float &X(void);
        float &Y(void);

        float operator[](uint index) const;
        float &operator[](uint index);

        const float *GetFloatPtr(void) const;

        // Operators
        bool operator==(const Vec2 &rhs) const;
        bool operator!=(const Vec2 &rhs) const;
        Vec2 operator+(const Vec2 &rhs) const;
        Vec2 operator-(void) const;
        Vec2 operator-(const Vec2 &rhs) const;
        Vec2 operator*(const Vec2 &rhs) const;
        Vec2 operator*(float rhs) const;
        friend Vec2 operator*(float lhs, const Vec2 &rhs);
        Vec2 operator/(const Vec2 &rhs) const;
        Vec2 operator/(float rhs) const;
        const Vec2 &operator=(const Vec2 &rhs);
        const Vec2 &operator+=(const Vec2 &rhs);
        const Vec2 &operator-=(const Vec2 &rhs);
        const Vec2 &operator*=(const Vec2 &rhs);
        const Vec2 &operator*=(float rhs);
        const Vec2 &operator/=(const Vec2 &rhs);
        const Vec2 &operator/=(float rhs);
    };
}

#include "Vec2.hpp"

