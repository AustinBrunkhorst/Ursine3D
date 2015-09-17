/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** SVec4.h
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
	// forward declaration
	struct XMFLOAT4;
}
#endif

namespace ursine
{
	// Foward declaration
	class SVec3;
	class Vec3;
	class Vec4;

	ALIGNED16(class) SVec4
	{
		friend class SVec3;
		friend class Vec3;
		friend class Vec4;

	public:
		// Constructors
		SVec4(void);
		SVec4(const SVec4 &value);
		SVec4(const Vec4 &value);
		explicit SVec4(float value);
		SVec4(float X, float Y, float Z, float W);
		SVec4(const SVec3 &value, float W);
		SVec4(const Vec3 &value, float W);

#ifdef URSINE_GRAPHICS_DIRECTX
		explicit SVec4(const DirectX::XMFLOAT4 &vec);

		DirectX::XMFLOAT4 ToDxVec(void) const;
#endif

		// Properties
		static const SVec4 &Zero(void);
		static const SVec4 &One(void);
		static const SVec4 &UnitX(void);
		static const SVec4 &UnitY(void);
		static const SVec4 &UnitZ(void);
		static const SVec4 &UnitW(void);

		// Public Methods
		void Clean(void); // Sets near zero elements to zero

		void Set(float X, float Y, float Z, float W);

		void Clamp(const SVec4 &min, const SVec4 &max);
		static SVec4 Clamp(const SVec4 &value, const SVec4 &min, const SVec4 &max);

		float Distance(const SVec4 &other) const;
		static float Distance(const SVec4 &vec1, const SVec4 &vec2);

		float DistanceSquared(const SVec4 &other) const;
		static float DistanceSquared(const SVec4 &vec1, const SVec4 &vec2);

		float Dot(const SVec4 &other) const;
		static float Dot(const SVec4 &vec1, const SVec4 &vec2);

		float Length(void) const;
		static float Length(const SVec4 &vec);

		float LengthSquared(void) const;
		static float LengthSquared(const SVec4 &vec);

		void Max(const SVec4 &other);
		static SVec4 Max(const SVec4 &vec1, const SVec4 &vec2);

		void Min(const SVec4 &other);
		static SVec4 Min(const SVec4 &vec1, const SVec4 &vec2);

		void Normalize(void);
		static SVec4 Normalize(const SVec4 &vec);

		void Reflect(const SVec4 &normal);
		static SVec4 Reflect(const SVec4 &vec, const SVec4 &normal);

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
		bool operator==(const SVec4 &rhs) const;
		bool operator!=(const SVec4 &rhs) const;
		SVec4 operator+(const SVec4 &rhs) const;
		SVec4 operator-(void) const;
		SVec4 operator-(const SVec4 &rhs) const;
		SVec4 operator*(const SVec4 &rhs) const;
		SVec4 operator*(float rhs) const;
		friend SVec4 operator*(float lhs, const SVec4 &rhs);
		SVec4 operator/(const SVec4 &rhs) const;
		SVec4 operator/(float rhs) const;
		const SVec4 &operator=(const SVec4 &rhs);
		const SVec4 &operator+=(const SVec4 &rhs);
		const SVec4 &operator-=(const SVec4 &rhs);
		const SVec4 &operator*=(const SVec4 &rhs);
		const SVec4 &operator*=(float rhs);
		const SVec4 &operator/=(const SVec4 &rhs);
		const SVec4 &operator/=(float rhs);

		ALLOW_ALIGNED_ALLOC(16);

	protected:
#ifdef USE_SSE
		union
		{
			SIMDvec m_128;

			struct
			{
				float m_x, m_y, m_z, m_w;
			};
		};

		explicit SVec4(const SIMDvec &value);
#else
		float m_x, m_y, m_z, m_w;
#endif
	};
}

#include "SVec4.hpp"
