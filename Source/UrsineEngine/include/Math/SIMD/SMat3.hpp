

#include <string.h>

namespace ursine
{
	// Constructors
	INLINE SMat3::SMat3(void)
		: SMat3( Identity( ) )
	{
	}

	INLINE SMat3::SMat3(const SMat3 &other)
	{
		memcpy( m, other.m, sizeof(m) );
	}

	INLINE SMat3::SMat3(const SVec3 &r0, const SVec3 &r1, const SVec3 &r2)
	{
		m[ 0 ] = r0;
		m[ 1 ] = r1;
		m[ 2 ] = r2;
	}

	INLINE SMat3::SMat3(float m00, float m01, float m02,
	                    float m10, float m11, float m12,
	                    float m20, float m21, float m22)
	{
		Set(
			m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22
		);
	}

	INLINE SMat3::SMat3(float degrees)
	{
		Rotation( *this, degrees );
	}

	INLINE SMat3::SMat3(float x_scalar, float y_scalar)
	{
		Set(
			x_scalar, 0.0f, 0.0f,
			0.0f, y_scalar, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	INLINE SMat3::SMat3(const Vec2 &translation)
	{
		Set(
			1.0f, 0.0f, translation.X( ),
			0.0f, 1.0f, translation.Y( ),
			0.0f, 0.0f, 1.0f
		);
	}

	INLINE SMat3::SMat3(const Vec2 &translation, float degrees, const Vec2 &scale)
	{
		TRS( *this, translation, degrees, scale );
	}

	INLINE SMat3::SMat3(const SVec3 &scale)
	{
		Set(
			scale.X( ), 0.0f, 0.0f,
			0.0f, scale.Y( ), 0.0f,
			0.0f, 0.0f, scale.Z( )
		);
	}

	INLINE SMat3::SMat3(float z_degrees, float x_degrees, float y_degrees)
	{
		RotationZXY( *this, z_degrees, x_degrees, y_degrees );
	}

	// Properties
	INLINE const SMat3 &SMat3::Identity(void)
	{
		static const SMat3 identity( 1.0f, 0.0f, 0.0f,
		                             0.0f, 1.0f, 0.0f,
		                             0.0f, 0.0f, 1.0f );

		return identity;
	}

	// Private Methods
	INLINE float SMat3::dotCol0(const SVec3 &row) const
	{
		return row.Dot( SVec3( m[ 0 ][ 0 ], m[ 1 ][ 0 ], m[ 2 ][ 0 ] ) );
	}

	INLINE float SMat3::dotCol1(const SVec3 &row) const
	{
		return row.Dot( SVec3( m[ 0 ][ 1 ], m[ 1 ][ 1 ], m[ 2 ][ 1 ] ) );
	}

	INLINE float SMat3::dotCol2(const SVec3 &row) const
	{
		return row.Dot( SVec3( m[ 0 ][ 2 ], m[ 1 ][ 2 ], m[ 2 ][ 2 ] ) );
	}

	// Public Methods
	INLINE void SMat3::Clean(void)
	{
		m[ 0 ].Clean( );
		m[ 1 ].Clean( );
		m[ 2 ].Clean( );
	}

	INLINE void SMat3::Set(float m00, float m01, float m02,
	                       float m10, float m11, float m12,
	                       float m20, float m21, float m22)
	{
		m[ 0 ].Set( m00, m01, m02 );
		m[ 1 ].Set( m10, m11, m12 );
		m[ 2 ].Set( m20, m21, m22 );
	}

	INLINE void SMat3::Translate(const Vec2 &translation)
	{
		Translate( *this, translation );
	}

	INLINE void SMat3::Translate(SMat3 &mat, const Vec2 &translation)
	{
		mat.Set(
			1.0f, 0.0f, translation.X( ),
			0.0f, 1.0f, translation.Y( ),
			0.0f, 0.0f, 1.0f
		);
	}

	INLINE void SMat3::Rotation(float degrees)
	{
		Rotation( *this, degrees );
	}

	INLINE void SMat3::Rotation(SMat3 &mat, float degrees)
	{
		float s, c;
		math::SinCos( math::DegreesToRadians( degrees ), s, c );

		mat.Set(
			c, -s, 0.0f,
			s, c, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	INLINE void SMat3::RotationZXY(float z_degrees, float x_degrees, float y_degrees)
	{
		RotationZXY( *this, z_degrees, x_degrees, y_degrees );
	}

	INLINE void SMat3::RotationZXY(SMat3 &mat, float z_degrees, float x_degrees, float y_degrees)
	{
		float cx, sx, cy, sy, cz, sz;

		float x = math::DegreesToRadians( x_degrees );
		float y = math::DegreesToRadians( y_degrees );
		float z = math::DegreesToRadians( z_degrees );

		math::SinCos( x, sx, cx );
		math::SinCos( y, sy, cy );
		math::SinCos( z, sz, cz );

		float cycz = cy * cz;
		float sxsy = sx * sy;
		float szcy = sz * cy;

		mat.Set(
			cycz + sxsy * sz, cz * sxsy - szcy, cx * sx,
			cx * sz, cx * cz, -sx,
			szcy * sx - cz * sy, cycz * sx + sy * sz, cx * cy
		);
	}

	INLINE SVec3 SMat3::GetRotationXYZ( ) const
	{
		float x, y, z;

		if (m[ 2 ][ 1 ] < 1.f)
		{
			if (m[ 2 ][ 1 ] > -1.f)
			{
				x = asin( m[ 2 ][ 1 ] );
				z = atan2( -m[ 0 ][ 1 ], m[ 1 ][ 1 ] );
				y = atan2( -m[ 2 ][ 0 ], m[ 2 ][ 2 ] );
			}
			else
			{
				// Not a unique solution.
				x = -math::PI / 2.f;
				z = -atan2( -m[ 0 ][ 2 ], m[ 0 ][ 0 ] );
				y = 0;
			}
		}
		else
		{
			// Not a unique solution.
			x = math::PI / 2.f;
			z = atan2( m[ 0 ][ 2 ], m[ 0 ][ 0 ] );
			y = 0;
		}

		return {
			math::RadiansToDegrees( x ),
			math::RadiansToDegrees( y ),
			math::RadiansToDegrees( z )
		};
	}

	INLINE void SMat3::Scale(const Vec2 &scale)
	{
		Scale( *this, scale );
	}

	INLINE void SMat3::Scale(SMat3 &mat, const Vec2 &scale)
	{
		mat.Set(
			scale.X( ), 0.0f, 0.0f,
			0.0f, scale.Y( ), 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	INLINE void SMat3::Scale(const SVec3 &scale)
	{
		Scale( *this, scale );
	}

	INLINE void SMat3::Scale(SMat3 &mat, const SVec3 &scale)
	{
		mat.Set(
			scale.X( ), 0.0f, 0.0f,
			0.0f, scale.Y( ), 0.0f,
			0.0f, 0.0f, scale.Z( )
		);
	}

	INLINE void SMat3::TRS(const Vec2 &translation, float degrees, const Vec2 &scale)
	{
		TRS( *this, translation, degrees, scale );
	}

	INLINE void SMat3::TRS(SMat3 &mat, const Vec2 &translation, float degrees, const Vec2 &scale)
	{
		float scale_x = scale.X( ), scale_y = scale.Y( );
		float s, c;
		math::SinCos( math::DegreesToRadians( degrees ), s, c );

		mat.Set(
			scale_x * c, -scale_y * s, translation.X( ),
			scale_x * s, scale_y * c, translation.Y( ),
			0.0f, 0.0f, 1.0f
		);
	}

	INLINE void SMat3::SetLookAt(const SVec3 &targetDirection, const SVec3 &localForward, const SVec3 &localUp, const SVec3 &worldUp)
	{
		// Generate the third basis vector in the local space.
		SVec3 localRight = SVec3::Cross( localUp, localForward );
		localRight.Normalize( );

		// A. Now we have an orthonormal linear basis { localRight, localUp, localForward } for the object local space.

		// Generate the third basis vector for the world space.
		SVec3 worldRight = SVec3::Cross( worldUp, targetDirection );
		worldRight.Normalize( );

		// Since the input worldUp vector is not necessarily perpendicular to the targetDirection vector,
		// we need to compute the real world space up vector that the "head" of the object will point
		// towards when the model is looking towards the desired target direction.
		SVec3 perpWorldUp = SVec3::Cross( targetDirection, worldRight );
		perpWorldUp.Normalize( );

		// B. Now we have an orthonormal linear basis { worldRight, perpWorldUp, targetDirection } for the desired target orientation.

		// We want to build a matrix M that performs the following mapping:
		// 1. localRight must be mapped to worldRight.        (M * localRight = worldRight)
		// 2. localUp must be mapped to perpWorldUp.          (M * localUp = perpWorldUp)
		// 3. localForward must be mapped to targetDirection. (M * localForward = targetDirection)
		// i.e. we want to map the basis A to basis B.

		// This matrix M exists, and it is an orthonormal rotation matrix with a determinant of +1, because
		// the bases A and B are orthonormal with the same handedness.

		// Below, use the notation that (a,b,c) is a 3x3 matrix with a as its first column, b second, and c third.

		// By algebraic manipulation, we can rewrite conditions 1, 2 and 3 in a matrix form:
		//        M * (localRight, localUp, localForward) = (worldRight, perpWorldUp, targetDirection)
		// or     M = (worldRight, perpWorldUp, targetDirection) * (localRight, localUp, localForward)^{-1}.
		// or     M = m1 * m2, where

		// m1 equals (worldRight, perpWorldUp, targetDirection):
		SMat3 m1;
		m1.SetColumns( worldRight, perpWorldUp, targetDirection );

		// and m2 equals (localRight, localUp, localForward)^{-1}:
		SMat3 m2( localRight, localUp, localForward );
		// Above we used the shortcut that for an orthonormal matrix M, M^{-1} = M^T. So set the rows
		// and not the columns to directly produce the transpose, i.e. the inverse of (localRight, localUp, localForward).

		// Compute final M.
		*this = m1 * m2;

		// And fix any numeric stability issues by re-orthonormalizing the result.
		Orthonormalize( );
	}

	INLINE void SMat3::SetLookAt(const SVec3 &targetDirection, const SVec3 &worldUp)
	{
		SetLookAt( targetDirection, SVec3::UnitZ( ), SVec3::UnitY( ), worldUp );
	}

	INLINE void SMat3::Transpose(void)
	{
		Transpose( *this );
	}

	INLINE void SMat3::Transpose(SMat3 &mat)
	{
		// swap(1, 3)
		float temp = mat.m[ 0 ][ 1 ];
		mat.m[ 0 ][ 1 ] = mat.m[ 1 ][ 0 ];
		mat.m[ 1 ][ 0 ] = temp;

		// swap (2, 6)
		temp = mat.m[ 0 ][ 2 ];
		mat.m[ 0 ][ 2 ] = mat.m[ 2 ][ 0 ];
		mat.m[ 2 ][ 0 ] = temp;

		// swap(5, 7)
		temp = mat.m[ 1 ][ 2 ];
		mat.m[ 1 ][ 2 ] = mat.m[ 2 ][ 1 ];
		mat.m[ 2 ][ 1 ] = temp;
	}

	INLINE SMat3 SMat3::Transpose(void) const
	{
		return Transpose( *this );
	}

	INLINE SMat3 SMat3::Transpose(const SMat3 &mat)
	{
		return {
			mat.m[ 0 ][ 0 ], mat.m[ 1 ][ 0 ], mat.m[ 2 ][ 0 ],
			mat.m[ 0 ][ 1 ], mat.m[ 1 ][ 1 ], mat.m[ 2 ][ 1 ],
			mat.m[ 0 ][ 2 ], mat.m[ 1 ][ 2 ], mat.m[ 2 ][ 2 ]
		};
	}

	INLINE void SMat3::Inverse(void)
	{
		Inverse( *this );
	}

	INLINE void SMat3::Inverse(SMat3 &mat)
	{
		// compute determinant
		float cofactor0 = mat.m[ 1 ][ 1 ] * mat.m[ 2 ][ 2 ] - mat.m[ 1 ][ 2 ] * mat.m[ 2 ][ 1 ];
		float cofactor3 = mat.m[ 0 ][ 2 ] * mat.m[ 2 ][ 1 ] - mat.m[ 0 ][ 1 ] * mat.m[ 2 ][ 2 ];
		float cofactor6 = mat.m[ 0 ][ 1 ] * mat.m[ 1 ][ 2 ] - mat.m[ 0 ][ 2 ] * mat.m[ 1 ][ 1 ];

		float det = mat.m[ 0 ][ 0 ] * cofactor0 + mat.m[ 1 ][ 0 ] * cofactor3 + mat.m[ 2 ][ 0 ] * cofactor6;

		if (det == 0.0f)
			return;

		// create adjoint matrix and multiply by 1/det to get inverse
		float invDet = 1.0f / det;
		float N10 = mat.m[ 1 ][ 2 ] * mat.m[ 2 ][ 0 ] - mat.m[ 1 ][ 0 ] * mat.m[ 2 ][ 2 ],
			N11 = mat.m[ 0 ][ 0 ] * mat.m[ 2 ][ 2 ] - mat.m[ 0 ][ 2 ] * mat.m[ 2 ][ 0 ],
			N12 = mat.m[ 0 ][ 2 ] * mat.m[ 1 ][ 0 ] - mat.m[ 0 ][ 0 ] * mat.m[ 1 ][ 2 ],
			N20 = mat.m[ 1 ][ 0 ] * mat.m[ 2 ][ 1 ] - mat.m[ 1 ][ 1 ] * mat.m[ 2 ][ 0 ],
			N21 = mat.m[ 0 ][ 1 ] * mat.m[ 2 ][ 0 ] - mat.m[ 0 ][ 0 ] * mat.m[ 2 ][ 1 ],
			N22 = mat.m[ 0 ][ 0 ] * mat.m[ 1 ][ 1 ] - mat.m[ 0 ][ 1 ] * mat.m[ 1 ][ 0 ];

		mat.Set(
			invDet * cofactor0, invDet * cofactor3, invDet * cofactor6,
			invDet * N10, invDet * N11, invDet * N12,
			invDet * N20, invDet * N21, invDet * N22
		);
	}

	INLINE SMat3 SMat3::Inverse(void) const
	{
		return Inverse( *this );
	}

	INLINE SMat3 SMat3::Inverse(const SMat3 &mat)
	{
		SMat3 new_mat = mat;

		new_mat.Inverse( );

		return new_mat;
	}

	INLINE float SMat3::Determinant(void) const
	{
		return Determinant( *this );
	}

	INLINE float SMat3::Determinant(const SMat3 &mat)
	{
		float cofactor0 = mat.m[ 1 ][ 1 ] * mat.m[ 2 ][ 2 ] - mat.m[ 1 ][ 2 ] * mat.m[ 2 ][ 1 ];
		float cofactor3 = mat.m[ 0 ][ 2 ] * mat.m[ 2 ][ 1 ] - mat.m[ 0 ][ 1 ] * mat.m[ 2 ][ 2 ];
		float cofactor6 = mat.m[ 0 ][ 1 ] * mat.m[ 1 ][ 2 ] - mat.m[ 0 ][ 2 ] * mat.m[ 1 ][ 1 ];

		return mat.m[ 0 ][ 0 ] * cofactor0 + mat.m[ 1 ][ 0 ] * cofactor3 + mat.m[ 2 ][ 0 ] * cofactor6;
	}

	INLINE void SMat3::Orthonormalize(void)
	{
		SVec3 c0, c1, c2;

		GetColumns( c0, c1, c2 );

		SVec3::Orthonormalize( c0, c1, c2 );

		SetColumns( c0, c1, c2 );
	}

	INLINE void SMat3::SetRows(const SVec3 &r0, const SVec3 &r1, const SVec3 &r2)
	{
		m[ 0 ] = r0;
		m[ 1 ] = r1;
		m[ 2 ] = r2;
	}

	INLINE void SMat3::GetRows(SVec3 &r0, SVec3 &r1, SVec3 &r2) const
	{
		r0 = m[ 0 ];
		r1 = m[ 1 ];
		r2 = m[ 2 ];
	}

	INLINE void SMat3::SetRow(uint i, const SVec3 &row)
	{
		m[ i ] = row;
	}

	INLINE SVec3 SMat3::GetRow(uint i) const
	{
		return m[ i ];
	}

	INLINE void SMat3::SetColumns(const SVec3 &c0, const SVec3 &c1, const SVec3 &c2)
	{
		Set(
			c0.X( ), c1.X( ), c2.X( ),
			c0.Y( ), c1.Y( ), c2.Y( ),
			c0.Z( ), c1.Z( ), c2.Z( )
		);
	}

	INLINE void SMat3::GetColumns(SVec3 &c0, SVec3 &c1, SVec3 &c2) const
	{
		c0.Set( m[ 0 ][ 0 ], m[ 1 ][ 0 ], m[ 2 ][ 0 ] );
		c1.Set( m[ 0 ][ 1 ], m[ 1 ][ 1 ], m[ 2 ][ 1 ] );
		c2.Set( m[ 0 ][ 2 ], m[ 1 ][ 2 ], m[ 2 ][ 2 ] );
	}

	INLINE void SMat3::SetColumn(uint i, const SVec3 &column)
	{
		auto &mat = *this;

		mat( 0, i ) = column.X( );
		mat( 1, i ) = column.Y( );
		mat( 2, i ) = column.Z( );
	}

	INLINE SVec3 SMat3::GetColumn(uint i) const
	{
		auto &mat = *this;

		return { mat( 0, i ), mat( 1, i ), mat( 2, i ) };
	}

	INLINE Vec2 SMat3::TransformVector(const Vec2 &vector) const
	{
		float x = vector.X( );
		float y = vector.Y( );

		return {
			m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y,
			m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y
		};
	}

	INLINE void SMat3::TransformVectorInplace(Vec2 &vector) const
	{
		float x = vector.X( );
		float y = vector.Y( );

		vector.X( ) = m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y;
		vector.Y( ) = m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y;
	}

	INLINE Vec2 SMat3::TransformPoint(const Vec2 &point) const
	{
		float x = point.X( );
		float y = point.Y( );

		return {
			m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ],
			m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ]
		};
	}

	INLINE void SMat3::TransformPointInplace(Vec2 &point) const
	{
		float x = point.X( );
		float y = point.Y( );

		point.X( ) = m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ];
		point.Y( ) = m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ];
	}

	INLINE Vec2 SMat3::TransformPointAndDiv(const Vec2 &point) const
	{
		float x = point.X( );
		float y = point.Y( );
		float z = m[ 2 ][ 0 ] * x + m[ 2 ][ 1 ] * y + m[ 2 ][ 2 ];
		float inv_div = math::IsZero( z ) ? 1.0f : 1.0f / z;

		return {
			inv_div * ( m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ] ),
			inv_div * ( m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ] )
		};
	}

	INLINE void SMat3::TransformPointAndDivInplace(Vec2 &point) const
	{
		float x = point.X( );
		float y = point.Y( );
		float z = m[ 2 ][ 0 ] * x + m[ 2 ][ 1 ] * y + m[ 2 ][ 2 ];
		float inv_div = math::IsZero( z ) ? 1.0f : 1.0f / z;

		point.X( ) = inv_div * ( m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ] );
		point.Y( ) = inv_div * ( m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ] );
	}

	INLINE void SMat3::SetWorldToCamera(float width, float height, float rotation, const Vec2 &trans)
	{
		float c, s;
		math::SinCos( -rotation, s, c );

		float c_2 = c * c, s_2 = s * s
			, A = c / ( c_2 + s_2 )
			, B = s / ( c_2 + s_2 );

		float inv_w = 1.0f / width
			, inv_h = 1.0f / height;

		SetColumns(
			SVec3( A * inv_w, -B * inv_h, 0 ),
			SVec3( B * inv_w, A * inv_h, 0 ),
			SVec3( ( ( -A * trans.X( ) ) - B * trans.Y( ) ) * inv_w,
			       ( B * trans.X( ) - A * trans.Y( ) ) * inv_h, 1 )
		);
	}

	// Accessors
	INLINE float &SMat3::operator()(uint row, uint column)
	{
		return m[ row ][ column ];
	}

	INLINE float SMat3::operator()(uint row, uint column) const
	{
		return m[ row ][ column ];
	}

	INLINE const float *SMat3::GetFloatPtr(void) const
	{
		return m[ 0 ].GetFloatPtr( );
	}

	// Operators
	INLINE SMat3 SMat3::operator*(const SMat3 &rhs) const
	{
		return {
			rhs.dotCol0( m[ 0 ] ), rhs.dotCol1( m[ 0 ] ), rhs.dotCol2( m[ 0 ] ),
			rhs.dotCol0( m[ 1 ] ), rhs.dotCol1( m[ 1 ] ), rhs.dotCol2( m[ 1 ] ),
			rhs.dotCol0( m[ 2 ] ), rhs.dotCol1( m[ 2 ] ), rhs.dotCol2( m[ 2 ] )
		};
	}

	INLINE SMat3 SMat3::operator*(float rhs) const
	{
		return {
			m[ 0 ] * rhs,
			m[ 1 ] * rhs,
			m[ 2 ] * rhs
		};
	}

	INLINE SVec3 SMat3::operator*(const SVec3 &rhs) const
	{
		return {
			m[ 0 ].Dot( rhs ),
			m[ 1 ].Dot( rhs ),
			m[ 2 ].Dot( rhs )
		};
	}

	INLINE SMat3 SMat3::operator+(const SMat3 &rhs) const
	{
		auto &rhs_m = rhs.m;

		return {
			m[ 0 ] + rhs_m[ 0 ],
			m[ 1 ] + rhs_m[ 1 ],
			m[ 2 ] + rhs_m[ 2 ]
		};
	}

	INLINE SMat3 SMat3::operator-(const SMat3 &rhs) const
	{
		auto &rhs_m = rhs.m;

		return {
			m[ 0 ] - rhs_m[ 0 ],
			m[ 1 ] - rhs_m[ 1 ],
			m[ 2 ] - rhs_m[ 2 ]
		};
	}

	INLINE const SMat3 &SMat3::operator=(const SMat3 &rhs)
	{
		auto &rhs_m = rhs.m;

		m[ 0 ] = rhs_m[ 0 ];
		m[ 1 ] = rhs_m[ 1 ];
		m[ 2 ] = rhs_m[ 2 ];

		return *this;
	}

	INLINE const SMat3 &SMat3::operator*=(const SMat3 &rhs)
	{
		Set(
			rhs.dotCol0( m[ 0 ] ), rhs.dotCol1( m[ 0 ] ), rhs.dotCol2( m[ 0 ] ),
			rhs.dotCol0( m[ 1 ] ), rhs.dotCol1( m[ 1 ] ), rhs.dotCol2( m[ 1 ] ),
			rhs.dotCol0( m[ 2 ] ), rhs.dotCol1( m[ 2 ] ), rhs.dotCol2( m[ 2 ] )
		);

		return *this;
	}

	INLINE const SMat3 &SMat3::operator*=(float rhs)
	{
		m[ 0 ] *= rhs;
		m[ 1 ] *= rhs;
		m[ 2 ] *= rhs;

		return *this;
	}

	INLINE const SMat3 &SMat3::operator+=(const SMat3 &rhs)
	{
		auto &rhs_m = rhs.m;

		m[ 0 ] += rhs_m[ 0 ];
		m[ 1 ] += rhs_m[ 1 ];
		m[ 2 ] += rhs_m[ 2 ];

		return *this;
	}

	INLINE const SMat3 &SMat3::operator-=(const SMat3 &rhs)
	{
		auto &rhs_m = rhs.m;

		m[ 0 ] -= rhs_m[ 0 ];
		m[ 1 ] -= rhs_m[ 1 ];
		m[ 2 ] -= rhs_m[ 2 ];

		return *this;
	}

	INLINE bool SMat3::operator==(const SMat3 &rhs) const
	{
		bool result = true;
		auto &rhs_m = rhs.m;

		result = result && m[ 0 ] == rhs_m[ 0 ];
		result = result && m[ 1 ] == rhs_m[ 1 ];
		result = result && m[ 2 ] == rhs_m[ 2 ];

		return result;
	}

	INLINE bool SMat3::operator!=(const SMat3 &rhs) const
	{
		return !( *this == rhs );
	}
}
