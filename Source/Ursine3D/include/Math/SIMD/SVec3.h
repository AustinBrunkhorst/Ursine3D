/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** SVec3.h
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

#ifdef URSINE_GRAPHICS_DIRECTX
namespace DirectX
{
	// Forward Declaration
	struct XMFLOAT3;
}
#endif

namespace ursine
{
	// Forward declaration
	class Vec2;
	class Vec3;
	class SVec4;
	class Vec4;
	class SMat4;

	ALIGNED16(class) SVec3
	{
		friend class Vec2;
		friend class Vec3;
		friend class SVec4;
		friend class Vec4;
		friend class SMat4;

	public:
		// Constructors
		SVec3(void);
		SVec3(const SVec3 &value);
		SVec3(const Vec3 &value);
		explicit SVec3(float value);
		SVec3(float X, float Y, float Z);
		SVec3(const Vec2 &value, float Z);
		explicit SVec3(const SVec4 &value);
		explicit SVec3(const Vec4 &value);

#ifdef URSINE_GRAPHICS_DIRECTX
		explicit SVec3(const DirectX::XMFLOAT3 &vec);

		DirectX::XMFLOAT3 ToD3D(void) const;
#endif

		// Properties
		static const SVec3 &Zero(void);
		static const SVec3 &One(void);
		static const SVec3 &UnitX(void);
		static const SVec3 &UnitY(void);
		static const SVec3 &UnitZ(void);

		// Public Methods
		void Clean(void); // sets all near zero elements to zero

		void Set(float X, float Y, float Z);

		void Clamp(const SVec3 &min, const SVec3 &max);
		static SVec3 Clamp(const SVec3 &value, const SVec3 &min, const SVec3 &max);

		void Cross(const SVec3 &other);
		static SVec3 Cross(const SVec3 &vec1, const SVec3 &vec2);

		static float TripleProduct(const SVec3 &a, const SVec3 &b, const SVec3 &c);

		float Distance(const SVec3 &other) const;
		static float Distance(const SVec3 &vec1, const SVec3 &vec2);

		float DistanceSquared(const SVec3 &other) const;
		static float DistanceSquared(const SVec3 &vec1, const SVec3 &vec2);

		float Dot(const SVec3 &other) const;
		static float Dot(const SVec3 &vec1, const SVec3 &vec2);

		float Length(void) const;
		static float Length(const SVec3 &vec);

		float LengthSquared(void) const;
		static float LengthSquared(const SVec3 &vec);

		void Max(const SVec3 &other);
		static SVec3 Max(const SVec3 &vec1, const SVec3 &vec2);

		void Min(const SVec3 &other);
		static SVec3 Min(const SVec3 &vec1, const SVec3 &vec2);

		void Normalize(void);
		static SVec3 Normalize(const SVec3 &vec);

		static void Orthonormalize(SVec3 &vec1, SVec3 &vec2, SVec3 &vec3);

		void Reflect(const SVec3 &normal);
		static SVec3 Reflect(const SVec3 &vec, const SVec3 &normal);

		// This vector must be normalized
		void ProjectToNorm(const SVec3 &normal);

		// Vector must be normalized
		static SVec3 ProjectToNorm(const SVec3 &vec, const SVec3 &normal);

		std::string ToString(void) const;

		// Accessors
		float X(void) const;
		float Y(void) const;
		float Z(void) const;
		float &X(void);
		float &Y(void);
		float &Z(void);

		float operator[](uint index) const;
		float &operator[](uint index);

		const float *GetFloatPtr(void) const;

		// Operators
		bool operator==(const SVec3 &rhs) const;
		bool operator!=(const SVec3 &rhs) const;
		SVec3 operator+(const SVec3 &rhs) const;
		SVec3 operator-(void) const;
		SVec3 operator-(const SVec3 &rhs) const;
		SVec3 operator*(const SVec3 &rhs) const;
		SVec3 operator*(float rhs) const;
		friend SVec3 operator*(float lhs, const SVec3 &rhs);
		SVec3 operator/(const SVec3 &rhs) const;
		SVec3 operator/(float rhs) const;
		const SVec3 &operator=(const SVec3 &rhs);
		const SVec3 &operator+=(const SVec3 &rhs);
		const SVec3 &operator-=(const SVec3 &rhs);
		const SVec3 &operator*=(const SVec3 &rhs);
		const SVec3 &operator*=(float rhs);
		const SVec3 &operator/=(const SVec3 &rhs);
		const SVec3 &operator/=(float rhs);

		ALLOW_ALIGNED_ALLOC(16);

	private:
#ifdef USE_SSE
		union
		{
			SIMDvec m_128;

			struct
			{
				float m_x, m_y, m_z, m_w;
			};
		};

		explicit SVec3(const SIMDvec &value);
#else
		float m_x, m_y, m_z;
#endif
	};
}

#include "SVec3.hpp"

