/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Vec3.h
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

#if defined(URSINE_DEPENDENCY_BulletPhysics)

// Forward Declaration
class btVector3;

#endif

namespace ursine
{
	// Forward declaration
	class Vec2;
	class SVec3;
	class Vec4;
	class SVec4;

	class Vec3
	{
		friend class Vec2;
		friend class SVec3;
		friend class Vec4;
		friend class SVec4;

	public:
        Meta(ExplicitGetter( "(float (ursine::Vec3::*)(void) const) &ursine::Vec3::X" ))
        EditorField(float x, X, SetX);

        Meta(ExplicitGetter( "(float (ursine::Vec3::*)(void) const) &ursine::Vec3::Y" ))
        EditorField(float y, Y, SetY);

        Meta(ExplicitGetter( "(float (ursine::Vec3::*)(void) const) &ursine::Vec3::Z" ))
        EditorField(float z, Z, SetZ);

		// Constructors
        Meta(Enable)
		Vec3(void);
		Vec3(const Vec3 &value);
		Vec3(const SVec3 &value);
		explicit Vec3(float value);
		Vec3(float X, float Y, float Z);
		Vec3(const Vec2 &value, float Z);

		explicit Vec3(const Vec4 &value);
		explicit Vec3(const SVec4 &value);

		#if defined(URSINE_DEPENDENCY_BulletPhysics)

		explicit Vec3(const btVector3 &vec);

		btVector3 ToBullet(void) const;

		#endif

		// Properties
		static const Vec3 &Zero(void);
		static const Vec3 &One(void);
		static const Vec3 &UnitX(void);
		static const Vec3 &UnitY(void);
		static const Vec3 &UnitZ(void);

		// Public Methods
		void Clean(void); // sets all near zero elements to zero

		void Set(float X, float Y, float Z);

		void Clamp(const Vec3 &min, const Vec3 &max);
		static Vec3 Clamp(const Vec3 &value, const Vec3 &min, const Vec3 &max);

		void Cross(const Vec3 &other);
		static Vec3 Cross(const Vec3 &vec1, const Vec3 &vec2);

		static float TripleProduct(const Vec3 &a, const Vec3 &b, const Vec3 &c);

		float Distance(const Vec3 &other) const;
		static float Distance(const Vec3 &vec1, const Vec3 &vec2);

		float DistanceSquared(const Vec3 &other) const;
		static float DistanceSquared(const Vec3 &vec1, const Vec3 &vec2);

		float Dot(const Vec3 &other) const;
		static float Dot(const Vec3 &vec1, const Vec3 &vec2);

		float Length(void) const;
		static float Length(const Vec3 &vec);

		float LengthSquared(void) const;
		static float LengthSquared(const Vec3 &vec);

		void Max(const Vec3 &other);
		static Vec3 Max(const Vec3 &vec1, const Vec3 &vec2);

		void Min(const Vec3 &other);
		static Vec3 Min(const Vec3 &vec1, const Vec3 &vec2);

		void Normalize(void);
		static Vec3 Normalize(const Vec3 &vec);

		void Reflect(const Vec3 &normal);
		static Vec3 Reflect(const Vec3 &vec, const Vec3 &normal);

        void GenerateOrthogonalVectors(Vec3 &u, Vec3 &v) const;
        static void GenerateOrthogonalVectors(const Vec3 &inputVec, Vec3 &u, Vec3 &v);

		std::string ToString(void) const;

		// Accessors
		float X(void) const;
		float Y(void) const;
		float Z(void) const;

		float &X(void);
		float &Y(void);
		float &Z(void);

        void SetX(float x);
        void SetY(float y);
        void SetZ(float z);

		float operator[](uint index) const;
		float &operator[](uint index);

		const float *GetFloatPtr(void) const;

		// Operators
		bool operator==(const Vec3 &rhs) const;
		bool operator!=(const Vec3 &rhs) const;
		Vec3 operator+(const Vec3 &rhs) const;
		Vec3 operator-(void) const;
		Vec3 operator-(const Vec3 &rhs) const;
		Vec3 operator*(const Vec3 &rhs) const;
		Vec3 operator*(float rhs) const;
		friend Vec3 operator*(float lhs, const Vec3 &rhs);
		Vec3 operator/(const Vec3 &rhs) const;
		Vec3 operator/(float rhs) const;
		const Vec3 &operator=(const Vec3 &rhs);
		const Vec3 &operator+=(const Vec3 &rhs);
		const Vec3 &operator-=(const Vec3 &rhs);
		const Vec3 &operator*=(const Vec3 &rhs);
		const Vec3 &operator*=(float rhs);
		const Vec3 &operator/=(const Vec3 &rhs);
		const Vec3 &operator/=(float rhs);

	private:
		float m_x, m_y, m_z;
	} Meta(Enable, EnableArrayType, WhiteListMethods);
}

#include "Vec3.hpp"

