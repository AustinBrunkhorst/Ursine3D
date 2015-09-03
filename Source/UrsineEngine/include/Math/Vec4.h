/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Vec4.h
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

namespace ursine
{
	// Foward declaration
	class Vec3;
	class SVec3;
	class SVec4;

	class Vec4
	{
		friend class Vec3;
		friend class SVec3;
		friend class SVec4;

	public:
		// Constructors
		Vec4(void);
		Vec4(const Vec4 &value);
		Vec4(const SVec4 &value);
		explicit Vec4(float value);
		Vec4(float X, float Y, float Z, float W);
		Vec4(const Vec3 &value, float W);
		Vec4(const SVec3 &value, float W);

		// Properties
		static const Vec4 &Zero(void);
		static const Vec4 &One(void);
		static const Vec4 &UnitX(void);
		static const Vec4 &UnitY(void);
		static const Vec4 &UnitZ(void);
		static const Vec4 &UnitW(void);

		// Public Methods
		void Clean(void); // Sets near zero elements to zero

		void Set(float X, float Y, float Z, float W);

		void Clamp(const Vec4 &min, const Vec4 &max);
		static Vec4 Clamp(const Vec4 &value, const Vec4 &min, const Vec4 &max);

		float Distance(const Vec4 &other) const;
		static float Distance(const Vec4 &vec1, const Vec4 &vec2);

		float DistanceSquared(const Vec4 &other) const;
		static float DistanceSquared(const Vec4 &vec1, const Vec4 &vec2);

		float Dot(const Vec4 &other) const;
		static float Dot(const Vec4 &vec1, const Vec4 &vec2);

		float Length(void) const;
		static float Length(const Vec4 &vec);

		float LengthSquared(void) const;
		static float LengthSquared(const Vec4 &vec);

		void Max(const Vec4 &other);
		static Vec4 Max(const Vec4 &vec1, const Vec4 &vec2);

		void Min(const Vec4 &other);
		static Vec4 Min(const Vec4 &vec1, const Vec4 &vec2);

		void Normalize(void);
		static Vec4 Normalize(const Vec4 &vec);

		void Reflect(const Vec4 &normal);
		static Vec4 Reflect(const Vec4 &vec, const Vec4 &normal);

		std::string ToString(void) const;

		// Accessors
		float X(void) const;
		float Y(void) const;
		float Z(void) const;
		float W(void) const;
		float &X(void);
		float &Y(void);
		float &Z(void);
		float &W(void);

		float operator[](uint index) const;
		float &operator[](uint index);

		const float *GetFloatPtr(void) const;

		// Operators
		bool operator==(const Vec4 &rhs) const;
		bool operator!=(const Vec4 &rhs) const;
		Vec4 operator+(const Vec4 &rhs) const;
		Vec4 operator-(void) const;
		Vec4 operator-(const Vec4 &rhs) const;
		Vec4 operator*(const Vec4 &rhs) const;
		Vec4 operator*(float rhs) const;
		friend Vec4 operator*(float lhs, const Vec4 &rhs);
		Vec4 operator/(const Vec4 &rhs) const;
		Vec4 operator/(float rhs) const;
		const Vec4 &operator=(const Vec4 &rhs);
		const Vec4 &operator+=(const Vec4 &rhs);
		const Vec4 &operator-=(const Vec4 &rhs);
		const Vec4 &operator*=(const Vec4 &rhs);
		const Vec4 &operator*=(float rhs);
		const Vec4 &operator/=(const Vec4 &rhs);
		const Vec4 &operator/=(float rhs);

	protected:
		float m_x, m_y, m_z, m_w;
	};
}

#include "Vec4.hpp"
