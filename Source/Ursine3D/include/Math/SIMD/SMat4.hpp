

#include <string.h>

namespace ursine
{
	// Constructors
	INLINE SMat4::SMat4(void)
		: SMat4( Identity( ) )
	{
	}

	INLINE SMat4::SMat4(const SMat4 &other)
	{
		memcpy( m, other.m, sizeof(m) );
	}

	INLINE SMat4::SMat4(const SVec4 &r0, const SVec4 &r1, const SVec4 &r2, const SVec4 &r3)
	{
		m[ 0 ] = r0;
		m[ 1 ] = r1;
		m[ 2 ] = r2;
		m[ 3 ] = r3;
	}

	INLINE SMat4::SMat4(float M00, float M01, float M02, float M03,
	                    float M10, float M11, float M12, float M13,
	                    float M20, float M21, float M22, float M23,
	                    float M30, float M31, float M32, float M33)
	{
		Set(
			M00, M01, M02, M03,
			M10, M11, M12, M13,
			M20, M21, M22, M23,
			M30, M31, M32, M33
		);
	}

	INLINE SMat4::SMat4(float x_scalar, float y_scalar, float z_scalar)
	{
		Set(
			x_scalar, 0.0f, 0.0f, 0.0f,
			0.0f, y_scalar, 0.0f, 0.0f,
			0.0f, 0.0f, z_scalar, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	INLINE SMat4::SMat4(const SVec3 &translation)
	{
		Set(
			1.0f, 0.0f, 0.0f, translation.X( ),
			0.0f, 1.0f, 0.0f, translation.Y( ),
			0.0f, 0.0f, 1.0f, translation.Z( ),
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	INLINE SMat4::SMat4(const SVec3 &translation, const SQuat &rotation, const SVec3 &scale)
	{
		setTRS( translation, rotation, scale );
	}

	// Properties
	INLINE const SMat4 &SMat4::Identity(void)
	{
		static const SMat4 identity( 1.0f, 0.0f, 0.0f, 0.0f,
		                             0.0f, 1.0f, 0.0f, 0.0f,
		                             0.0f, 0.0f, 1.0f, 0.0f,
		                             0.0f, 0.0f, 0.0f, 1.0f );

		return identity;
	}

	// Private Methods
	INLINE float SMat4::dotCol0(const SVec4 &row) const
	{
		return row.Dot( SVec4( m[ 0 ][ 0 ], m[ 1 ][ 0 ], m[ 2 ][ 0 ], m[ 3 ][ 0 ] ) );
	}

	INLINE float SMat4::dotCol1(const SVec4 &row) const
	{
		return row.Dot( SVec4( m[ 0 ][ 1 ], m[ 1 ][ 1 ], m[ 2 ][ 1 ], m[ 3 ][ 1 ] ) );
	}

	INLINE float SMat4::dotCol2(const SVec4 &row) const
	{
		return row.Dot( SVec4( m[ 0 ][ 2 ], m[ 1 ][ 2 ], m[ 2 ][ 2 ], m[ 3 ][ 2 ] ) );
	}

	INLINE float SMat4::dotCol3(const SVec4 &row) const
	{
		return row.Dot( SVec4( m[ 0 ][ 3 ], m[ 1 ][ 3 ], m[ 2 ][ 3 ], m[ 3 ][ 3 ] ) );
	}

	INLINE void SMat4::invert(const float *src, float *dest)
	{
#ifdef USE_SSE
		// Source: ftp://download.intel.com/design/PentiumIII/sml/24504301.pdf

		SIMDvec
			minor0 = _mm_set1_ps( 0.0f ),
			minor1 = _mm_set1_ps( 0.0f ),
			minor2 = _mm_set1_ps( 0.0f ),
			minor3 = _mm_set1_ps( 0.0f ),
			row0 = _mm_set1_ps( 0.0f ),
			row1 = _mm_set1_ps( 0.0f ),
			row2 = _mm_set1_ps( 0.0f ),
			row3 = _mm_set1_ps( 0.0f ),
			det = _mm_set1_ps( 0.0f ),
			tmp1 = _mm_set1_ps( 0.0f );

		tmp1 = _mm_loadh_pi( _mm_loadl_pi( tmp1, ( __m64* )( src ) ), ( __m64* )( src + 4 ) );
		row1 = _mm_loadh_pi( _mm_loadl_pi( row1, ( __m64* )( src + 8 ) ), ( __m64* )( src + 12 ) );
		row0 = _mm_shuffle_ps( tmp1, row1, 0x88 );
		row1 = _mm_shuffle_ps( row1, tmp1, 0xDD );
		tmp1 = _mm_loadh_pi( _mm_loadl_pi( tmp1, ( __m64* )( src + 2 ) ), ( __m64* )( src + 6 ) );
		row3 = _mm_loadh_pi( _mm_loadl_pi( row3, ( __m64* )( src + 10 ) ), ( __m64* )( src + 14 ) );
		row2 = _mm_shuffle_ps( tmp1, row3, 0x88 );
		row3 = _mm_shuffle_ps( row3, tmp1, 0xDD );
		// -----------------------------------------------
		tmp1 = _mm_mul_ps( row2, row3 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0xB1 );
		minor0 = _mm_mul_ps( row1, tmp1 );
		minor1 = _mm_mul_ps( row0, tmp1 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0x4E );
		minor0 = _mm_sub_ps( _mm_mul_ps( row1, tmp1 ), minor0 );
		minor1 = _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor1 );
		minor1 = _mm_shuffle_ps( minor1, minor1, 0x4E );
		// -----------------------------------------------
		tmp1 = _mm_mul_ps( row1, row2 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0xB1 );
		minor0 = _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor0 );
		minor3 = _mm_mul_ps( row0, tmp1 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0x4E );
		minor0 = _mm_sub_ps( minor0, _mm_mul_ps( row3, tmp1 ) );
		minor3 = _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor3 );
		minor3 = _mm_shuffle_ps( minor3, minor3, 0x4E );
		// -----------------------------------------------
		tmp1 = _mm_mul_ps( _mm_shuffle_ps( row1, row1, 0x4E ), row3 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0xB1 );
		row2 = _mm_shuffle_ps( row2, row2, 0x4E );
		minor0 = _mm_add_ps( _mm_mul_ps( row2, tmp1 ), minor0 );
		minor2 = _mm_mul_ps( row0, tmp1 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0x4E );
		minor0 = _mm_sub_ps( minor0, _mm_mul_ps( row2, tmp1 ) );
		minor2 = _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor2 );
		minor2 = _mm_shuffle_ps( minor2, minor2, 0x4E );
		// -----------------------------------------------
		tmp1 = _mm_mul_ps( row0, row1 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0xB1 );
		minor2 = _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor2 );
		minor3 = _mm_sub_ps( _mm_mul_ps( row2, tmp1 ), minor3 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0x4E );
		minor2 = _mm_sub_ps( _mm_mul_ps( row3, tmp1 ), minor2 );
		minor3 = _mm_sub_ps( minor3, _mm_mul_ps( row2, tmp1 ) );
		// -----------------------------------------------
		tmp1 = _mm_mul_ps( row0, row3 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0xB1 );
		minor1 = _mm_sub_ps( minor1, _mm_mul_ps( row2, tmp1 ) );
		minor2 = _mm_add_ps( _mm_mul_ps( row1, tmp1 ), minor2 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0x4E );
		minor1 = _mm_add_ps( _mm_mul_ps( row2, tmp1 ), minor1 );
		minor2 = _mm_sub_ps( minor2, _mm_mul_ps( row1, tmp1 ) );
		// -----------------------------------------------
		tmp1 = _mm_mul_ps( row0, row2 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0xB1 );
		minor1 = _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor1 );
		minor3 = _mm_sub_ps( minor3, _mm_mul_ps( row1, tmp1 ) );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, 0x4E );
		minor1 = _mm_sub_ps( minor1, _mm_mul_ps( row3, tmp1 ) );
		minor3 = _mm_add_ps( _mm_mul_ps( row1, tmp1 ), minor3 );
		// -----------------------------------------------
		det = _mm_mul_ps( row0, minor0 );
		det = _mm_add_ps( _mm_shuffle_ps( det, det, 0x4E ), det );
		det = _mm_add_ss( _mm_shuffle_ps( det, det, 0xB1 ), det );
		tmp1 = _mm_rcp_ss( det );
		det = _mm_sub_ss( _mm_add_ss( tmp1, tmp1 ), _mm_mul_ss( det, _mm_mul_ss( tmp1, tmp1 ) ) );
		det = _mm_shuffle_ps( det, det, 0x00 );
		minor0 = _mm_mul_ps( det, minor0 );
		_mm_storel_pi( ( __m64* )( dest ), minor0 );
		_mm_storeh_pi( ( __m64* )( dest + 2 ), minor0 );
		minor1 = _mm_mul_ps( det, minor1 );
		_mm_storel_pi( ( __m64* )( dest + 4 ), minor1 );
		_mm_storeh_pi( ( __m64* )( dest + 6 ), minor1 );
		minor2 = _mm_mul_ps( det, minor2 );
		_mm_storel_pi( ( __m64* )( dest + 8 ), minor2 );
		_mm_storeh_pi( ( __m64* )( dest + 10 ), minor2 );
		minor3 = _mm_mul_ps( det, minor3 );
		_mm_storel_pi( ( __m64* )( dest + 12 ), minor3 );
		_mm_storeh_pi( ( __m64* )( dest + 14 ), minor3 );
#else
		// Source: http://www.mesa3d.org/
        const float *m = src;

        float inv[16], det;
        int i;

        inv[0] = m[5] * m[10] * m[15] -
            m[5] * m[11] * m[14] -
            m[9] * m[6] * m[15] +
            m[9] * m[7] * m[14] +
            m[13] * m[6] * m[11] -
            m[13] * m[7] * m[10];

        inv[4] = -m[4] * m[10] * m[15] +
            m[4] * m[11] * m[14] +
            m[8] * m[6] * m[15] -
            m[8] * m[7] * m[14] -
            m[12] * m[6] * m[11] +
            m[12] * m[7] * m[10];

        inv[8] = m[4] * m[9] * m[15] -
            m[4] * m[11] * m[13] -
            m[8] * m[5] * m[15] +
            m[8] * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

        inv[12] = -m[4] * m[9] * m[14] +
            m[4] * m[10] * m[13] +
            m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

        inv[1] = -m[1] * m[10] * m[15] +
            m[1] * m[11] * m[14] +
            m[9] * m[2] * m[15] -
            m[9] * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

        inv[5] = m[0] * m[10] * m[15] -
            m[0] * m[11] * m[14] -
            m[8] * m[2] * m[15] +
            m[8] * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

        inv[9] = -m[0] * m[9] * m[15] +
            m[0] * m[11] * m[13] +
            m[8] * m[1] * m[15] -
            m[8] * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

        inv[13] = m[0] * m[9] * m[14] -
            m[0] * m[10] * m[13] -
            m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

        inv[2] = m[1] * m[6] * m[15] -
            m[1] * m[7] * m[14] -
            m[5] * m[2] * m[15] +
            m[5] * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

        inv[6] = -m[0] * m[6] * m[15] +
            m[0] * m[7] * m[14] +
            m[4] * m[2] * m[15] -
            m[4] * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

        inv[10] = m[0] * m[5] * m[15] -
            m[0] * m[7] * m[13] -
            m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

        inv[14] = -m[0] * m[5] * m[14] +
            m[0] * m[6] * m[13] +
            m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

        det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		// TODO: UAssert(det != 0.0f, "Matrix is uninvertable.");

        det = 1.0f / det;

        for (i = 0; i < 16; i++)
            dest[i] = inv[i] * det;
#endif
	}

	// TODO: Implement this
	/*INLINE void SMat4::invertFast(const float *src, float *dest)
    {
        // TODO: create SIMD accelerated version of this code

        // compute upper left 3x3 matrix determinant
        float cofactor0 = src[5] * src[10] - src[9] * src[6];
        float cofactor4 = src[8] * src[6] - src[4] * src[10];
        float cofactor8 = src[4] * src[8] - src[8] * src[5];
        float det = src[0] * cofactor0 + src[1] * cofactor4 + src[2] * cofactor8;

        // create adjunct matrix and multiply by 1/det to get upper 3x3
        float invDet = 1.0f / det;
        dest[0] = invDet * cofactor0;
        dest[4] = invDet * cofactor4;
        dest[8] = invDet * cofactor8;

        dest[1] = invDet * (src[9] * src[2] - src[1] * src[10]);
        dest[5] = invDet * (src[0] * src[10] - src[8] * src[2]);
        dest[9] = invDet * (src[8] * src[1] - src[0] * src[9]);

        dest[2] = invDet * (src[1] * src[6] - src[5] * src[2]);
        dest[5] = invDet * (src[4] * src[2] - src[0] * src[6]);
        dest[10] = invDet * (src[0] * src[5] - src[4] * src[1]);

        // multiply -translation by inverted 3x3 to get its inverse

        dest[3] = -dest[0] * src[3] - dest[1] * src[7] - dest[2] * src[11];
        dest[6] = -dest[4] * src[3] - dest[5] * src[7] - dest[6] * src[11];
        dest[11] = -dest[8] * src[3] - dest[9] * src[7] - dest[10] * src[11];
    }*/

	INLINE void SMat4::setRotation(const SQuat &quat)
	{
		// assume2(q.IsNormalized(1e-3f), q.ToString(), q.LengthSq());
		const float x = quat.X( );
		const float y = quat.Y( );
		const float z = quat.Z( );
		const float w = quat.W( );
		m[ 0 ][ 0 ] = 1 - 2 * ( y * y + z * z );
		m[ 0 ][ 1 ] = 2 * ( x * y - z * w );
		m[ 0 ][ 2 ] = 2 * ( x * z + y * w );
		m[ 1 ][ 0 ] = 2 * ( x * y + z * w );
		m[ 1 ][ 1 ] = 1 - 2 * ( x * x + z * z );
		m[ 1 ][ 2 ] = 2 * ( y * z - x * w );
		m[ 2 ][ 0 ] = 2 * ( x * z - y * w );
		m[ 2 ][ 1 ] = 2 * ( y * z + x * w );
		m[ 2 ][ 2 ] = 1 - 2 * ( x * x + y * y );
	}

	INLINE void SMat4::setTRS(const SVec3 &translation, const SQuat &rotation, const SVec3 &scale)
	{
		*this = SMat4( rotation ) * SMat4( scale );

		SetColumn( 3, SVec4( translation, 1.0f ) );
	}

	// Public Methods
	INLINE void SMat4::Clean(void)
	{
		m[ 0 ].Clean( );
		m[ 1 ].Clean( );
		m[ 2 ].Clean( );
		m[ 3 ].Clean( );
	}

	INLINE void SMat4::Set(float M00, float M01, float M02, float M03,
	                       float M10, float M11, float M12, float M13,
	                       float M20, float M21, float M22, float M23,
	                       float M30, float M31, float M32, float M33)
	{
		m[ 0 ].Set( M00, M01, M02, M03 );
		m[ 1 ].Set( M10, M11, M12, M13 );
		m[ 2 ].Set( M20, M21, M22, M23 );
		m[ 3 ].Set( M30, M31, M32, M33 );
	}

	INLINE void SMat4::Translate(const SVec3 &translation)
	{
		Translate( *this, translation );
	}

	INLINE void SMat4::Translate(SMat4 &mat, const SVec3 &translation)
	{
		mat.Set(
			1.0f, 0.0f, 0.0f, translation.X( ),
			0.0f, 1.0f, 0.0f, translation.Y( ),
			0.0f, 0.0f, 1.0f, translation.Z( ),
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	INLINE void SMat4::Rotation(const SQuat &quat)
	{
		setRotation( quat );
		SetRow( 3, SVec4( 0, 0, 0, 1 ) );
		SetColumn( 3, SVec4( 0, 0, 0, 1 ) );
	}

	INLINE void SMat4::RotationZXY(float z_degrees, float x_degrees, float y_degrees)
	{
		RotationZXY( *this, z_degrees, x_degrees, y_degrees );
	}

	INLINE void SMat4::RotationZXY(SMat4 &mat, float z_degrees, float x_degrees, float y_degrees)
	{
		float cx, sx, cy, sy, cz, sz;

		math::SinCos( math::DegreesToRadians( x_degrees ), sx, cx );
		math::SinCos( math::DegreesToRadians( y_degrees ), sy, cy );
		math::SinCos( math::DegreesToRadians( z_degrees ), sz, cz );

		float cycz = cy * cz;
		float sxsy = sx * sy;
		float szcy = sz * cy;

		mat.Set(
			cycz + sxsy * sz, cz * sxsy - szcy, cx * sx, 0.0f,
			cx * sz, cx * cz, -sx, 0.0f,
			szcy * sx - cz * sy, cycz * sx + sy * sz, cx * cy, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	INLINE void SMat4::Scale(const SVec3 &scale)
	{
		Scale( *this, scale );
	}

	INLINE void SMat4::Scale(SMat4 &mat, const SVec3 &scale)
	{
		mat.Set(
			scale.X( ), 0.0f, 0.0f, 0.0f,
			0.0f, scale.Y( ), 0.0f, 0.0f,
			0.0f, 0.0f, scale.Z( ), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	INLINE void SMat4::Scale(const SVec4 &scale)
	{
		Scale( *this, scale );
	}

	INLINE void SMat4::Scale(SMat4 &mat, const SVec4 &scale)
	{
		mat.Set(
			scale.X( ), 0.0f, 0.0f, 0.0f,
			0.0f, scale.Y( ), 0.0f, 0.0f,
			0.0f, 0.0f, scale.Z( ), 0.0f,
			0.0f, 0.0f, 0.0f, scale.W( )
		);
	}

	INLINE void SMat4::TRS(const SVec3 &translation, const SQuat &rotation, const SVec3 &scale)
	{
		setTRS( translation, rotation, scale );
	}

	INLINE void SMat4::Transpose(void)
	{
		Transpose( *this );
	}

	INLINE void SMat4::Transpose(SMat4 &mat)
	{
		// swap m[0][1] and m[1][0]
		float temp = mat.m[ 0 ][ 1 ];
		mat.m[ 0 ][ 1 ] = mat.m[ 1 ][ 0 ];
		mat.m[ 1 ][ 0 ] = temp;

		// swap m[0][2] and m[2][0]
		temp = mat.m[ 0 ][ 2 ];
		mat.m[ 0 ][ 2 ] = mat.m[ 2 ][ 0 ];
		mat.m[ 2 ][ 0 ] = temp;

		// swap m[0][3] and m[3][0]
		temp = mat.m[ 0 ][ 3 ];
		mat.m[ 0 ][ 3 ] = mat.m[ 3 ][ 0 ];
		mat.m[ 3 ][ 0 ] = temp;

		// swap m[1][2] and m[2][1]
		temp = mat.m[ 1 ][ 2 ];
		mat.m[ 1 ][ 2 ] = mat.m[ 2 ][ 1 ];
		mat.m[ 2 ][ 1 ] = temp;

		// swap m[1][3] and m[3][1]
		temp = mat.m[ 1 ][ 3 ];
		mat.m[ 1 ][ 3 ] = mat.m[ 3 ][ 1 ];
		mat.m[ 3 ][ 1 ] = temp;

		// swap m[2][3] and m[3][2]
		temp = mat.m[ 2 ][ 3 ];
		mat.m[ 2 ][ 3 ] = mat.m[ 3 ][ 2 ];
		mat.m[ 3 ][ 2 ] = temp;
	}

	INLINE SMat4 SMat4::Transpose(void) const
	{
		return Transpose( *this );
	}

	INLINE SMat4 SMat4::Transpose(const SMat4 &mat)
	{
		return {
			mat.m[ 0 ][ 0 ], mat.m[ 1 ][ 0 ], mat.m[ 2 ][ 0 ], mat.m[ 3 ][ 0 ],
			mat.m[ 0 ][ 1 ], mat.m[ 1 ][ 1 ], mat.m[ 2 ][ 1 ], mat.m[ 3 ][ 1 ],
			mat.m[ 0 ][ 2 ], mat.m[ 1 ][ 2 ], mat.m[ 2 ][ 2 ], mat.m[ 3 ][ 2 ],
			mat.m[ 0 ][ 3 ], mat.m[ 1 ][ 3 ], mat.m[ 2 ][ 3 ], mat.m[ 3 ][ 3 ]
		};
	}

	INLINE void SMat4::Inverse(void)
	{
		Inverse( *this );
	}

	INLINE void SMat4::Inverse(SMat4 &mat)
	{
		invert( &mat.m[ 0 ][ 0 ], &mat.m[ 0 ][ 0 ] );
	}

	INLINE SMat4 SMat4::Inverse(void) const
	{
		return Inverse( *this );
	}

	INLINE SMat4 SMat4::Inverse(const SMat4 &mat)
	{
		SMat4 result;

		invert( mat.m[ 0 ].GetFloatPtr( ), &result.m[ 0 ][ 0 ] );

		return result;
	}

	/*INLINE void SMat4::InverseFast(void)
    {
        InverseFast(*this);
    }

    INLINE void SMat4::InverseFast(SMat4 &mat)
    {
        invertFast(&mat.m[0][0], &mat.m[0][0]);
    }

    INLINE SMat4 SMat4::InverseFast(void) const
    {
        return InverseFast(*this);
    }

    INLINE SMat4 SMat4::InverseFast(const SMat4 &mat)
    {
        SMat4 result;

        invertFast(mat.m[0].GetFloatPtr(), &result.m[0][0]);

        return result;
    }*/

	INLINE float SMat4::Determinant(void) const
	{
		return Determinant( *this );
	}

	INLINE float SMat4::Determinant(const SMat4 &mat)
	{
		// Source: http://www.mesa3d.org/
		const float *m = mat.m[ 0 ].GetFloatPtr( );

		float inv[16];

		inv[ 0 ] = m[ 5 ] * m[ 10 ] * m[ 15 ] -
			m[ 5 ] * m[ 11 ] * m[ 14 ] -
			m[ 9 ] * m[ 6 ] * m[ 15 ] +
			m[ 9 ] * m[ 7 ] * m[ 14 ] +
			m[ 13 ] * m[ 6 ] * m[ 11 ] -
			m[ 13 ] * m[ 7 ] * m[ 10 ];

		inv[ 4 ] = -m[ 4 ] * m[ 10 ] * m[ 15 ] +
			m[ 4 ] * m[ 11 ] * m[ 14 ] +
			m[ 8 ] * m[ 6 ] * m[ 15 ] -
			m[ 8 ] * m[ 7 ] * m[ 14 ] -
			m[ 12 ] * m[ 6 ] * m[ 11 ] +
			m[ 12 ] * m[ 7 ] * m[ 10 ];

		inv[ 8 ] = m[ 4 ] * m[ 9 ] * m[ 15 ] -
			m[ 4 ] * m[ 11 ] * m[ 13 ] -
			m[ 8 ] * m[ 5 ] * m[ 15 ] +
			m[ 8 ] * m[ 7 ] * m[ 13 ] +
			m[ 12 ] * m[ 5 ] * m[ 11 ] -
			m[ 12 ] * m[ 7 ] * m[ 9 ];

		inv[ 12 ] = -m[ 4 ] * m[ 9 ] * m[ 14 ] +
			m[ 4 ] * m[ 10 ] * m[ 13 ] +
			m[ 8 ] * m[ 5 ] * m[ 14 ] -
			m[ 8 ] * m[ 6 ] * m[ 13 ] -
			m[ 12 ] * m[ 5 ] * m[ 10 ] +
			m[ 12 ] * m[ 6 ] * m[ 9 ];

		inv[ 1 ] = -m[ 1 ] * m[ 10 ] * m[ 15 ] +
			m[ 1 ] * m[ 11 ] * m[ 14 ] +
			m[ 9 ] * m[ 2 ] * m[ 15 ] -
			m[ 9 ] * m[ 3 ] * m[ 14 ] -
			m[ 13 ] * m[ 2 ] * m[ 11 ] +
			m[ 13 ] * m[ 3 ] * m[ 10 ];

		inv[ 5 ] = m[ 0 ] * m[ 10 ] * m[ 15 ] -
			m[ 0 ] * m[ 11 ] * m[ 14 ] -
			m[ 8 ] * m[ 2 ] * m[ 15 ] +
			m[ 8 ] * m[ 3 ] * m[ 14 ] +
			m[ 12 ] * m[ 2 ] * m[ 11 ] -
			m[ 12 ] * m[ 3 ] * m[ 10 ];

		inv[ 9 ] = -m[ 0 ] * m[ 9 ] * m[ 15 ] +
			m[ 0 ] * m[ 11 ] * m[ 13 ] +
			m[ 8 ] * m[ 1 ] * m[ 15 ] -
			m[ 8 ] * m[ 3 ] * m[ 13 ] -
			m[ 12 ] * m[ 1 ] * m[ 11 ] +
			m[ 12 ] * m[ 3 ] * m[ 9 ];

		inv[ 13 ] = m[ 0 ] * m[ 9 ] * m[ 14 ] -
			m[ 0 ] * m[ 10 ] * m[ 13 ] -
			m[ 8 ] * m[ 1 ] * m[ 14 ] +
			m[ 8 ] * m[ 2 ] * m[ 13 ] +
			m[ 12 ] * m[ 1 ] * m[ 10 ] -
			m[ 12 ] * m[ 2 ] * m[ 9 ];

		inv[ 2 ] = m[ 1 ] * m[ 6 ] * m[ 15 ] -
			m[ 1 ] * m[ 7 ] * m[ 14 ] -
			m[ 5 ] * m[ 2 ] * m[ 15 ] +
			m[ 5 ] * m[ 3 ] * m[ 14 ] +
			m[ 13 ] * m[ 2 ] * m[ 7 ] -
			m[ 13 ] * m[ 3 ] * m[ 6 ];

		inv[ 6 ] = -m[ 0 ] * m[ 6 ] * m[ 15 ] +
			m[ 0 ] * m[ 7 ] * m[ 14 ] +
			m[ 4 ] * m[ 2 ] * m[ 15 ] -
			m[ 4 ] * m[ 3 ] * m[ 14 ] -
			m[ 12 ] * m[ 2 ] * m[ 7 ] +
			m[ 12 ] * m[ 3 ] * m[ 6 ];

		inv[ 10 ] = m[ 0 ] * m[ 5 ] * m[ 15 ] -
			m[ 0 ] * m[ 7 ] * m[ 13 ] -
			m[ 4 ] * m[ 1 ] * m[ 15 ] +
			m[ 4 ] * m[ 3 ] * m[ 13 ] +
			m[ 12 ] * m[ 1 ] * m[ 7 ] -
			m[ 12 ] * m[ 3 ] * m[ 5 ];

		inv[ 14 ] = -m[ 0 ] * m[ 5 ] * m[ 14 ] +
			m[ 0 ] * m[ 6 ] * m[ 13 ] +
			m[ 4 ] * m[ 1 ] * m[ 14 ] -
			m[ 4 ] * m[ 2 ] * m[ 13 ] -
			m[ 12 ] * m[ 1 ] * m[ 6 ] +
			m[ 12 ] * m[ 2 ] * m[ 5 ];

		inv[ 3 ] = -m[ 1 ] * m[ 6 ] * m[ 11 ] +
			m[ 1 ] * m[ 7 ] * m[ 10 ] +
			m[ 5 ] * m[ 2 ] * m[ 11 ] -
			m[ 5 ] * m[ 3 ] * m[ 10 ] -
			m[ 9 ] * m[ 2 ] * m[ 7 ] +
			m[ 9 ] * m[ 3 ] * m[ 6 ];

		inv[ 7 ] = m[ 0 ] * m[ 6 ] * m[ 11 ] -
			m[ 0 ] * m[ 7 ] * m[ 10 ] -
			m[ 4 ] * m[ 2 ] * m[ 11 ] +
			m[ 4 ] * m[ 3 ] * m[ 10 ] +
			m[ 8 ] * m[ 2 ] * m[ 7 ] -
			m[ 8 ] * m[ 3 ] * m[ 6 ];

		inv[ 11 ] = -m[ 0 ] * m[ 5 ] * m[ 11 ] +
			m[ 0 ] * m[ 7 ] * m[ 9 ] +
			m[ 4 ] * m[ 1 ] * m[ 11 ] -
			m[ 4 ] * m[ 3 ] * m[ 9 ] -
			m[ 8 ] * m[ 1 ] * m[ 7 ] +
			m[ 8 ] * m[ 3 ] * m[ 5 ];

		inv[ 15 ] = m[ 0 ] * m[ 5 ] * m[ 10 ] -
			m[ 0 ] * m[ 6 ] * m[ 9 ] -
			m[ 4 ] * m[ 1 ] * m[ 10 ] +
			m[ 4 ] * m[ 2 ] * m[ 9 ] +
			m[ 8 ] * m[ 1 ] * m[ 6 ] -
			m[ 8 ] * m[ 2 ] * m[ 5 ];

		return m[ 0 ] * inv[ 0 ] + m[ 1 ] * inv[ 4 ] + m[ 2 ] * inv[ 8 ] + m[ 3 ] * inv[ 12 ];
	}

	INLINE void SMat4::SetRows(const SVec4 &r0, const SVec4 &r1, const SVec4 &r2, const SVec4 &r3)
	{
		m[ 0 ] = r0;
		m[ 1 ] = r1;
		m[ 2 ] = r2;
		m[ 3 ] = r3;
	}

	INLINE void SMat4::GetRows(SVec4 &r0, SVec4 &r1, SVec4 &r2, SVec4 &r3) const
	{
		r0 = m[ 0 ];
		r1 = m[ 1 ];
		r2 = m[ 2 ];
		r3 = m[ 3 ];
	}

	INLINE void SMat4::SetRow(uint i, const SVec4 &row)
	{
		m[ i ] = row;
	}

	INLINE SVec4 SMat4::GetRow(uint i) const
	{
		return m[ i ];
	}


	INLINE void SMat4::SetColumns(const SVec4 &c0, const SVec4 &c1, const SVec4 &c2, const SVec4 &c3)
	{
		Set(
			c0.X( ), c1.X( ), c2.X( ), c3.X( ),
			c0.Y( ), c1.Y( ), c2.Y( ), c3.Y( ),
			c0.Z( ), c1.Z( ), c2.Z( ), c3.Z( ),
			c0.W( ), c1.W( ), c2.W( ), c3.W( )
		);
	}

	INLINE void SMat4::GetColumns(SVec4 &c0, SVec4 &c1, SVec4 &c2, SVec4 &c3) const
	{
		c0.Set( m[ 0 ][ 0 ], m[ 1 ][ 0 ], m[ 2 ][ 0 ], m[ 3 ][ 0 ] );
		c1.Set( m[ 0 ][ 1 ], m[ 1 ][ 1 ], m[ 2 ][ 1 ], m[ 3 ][ 1 ] );
		c2.Set( m[ 0 ][ 2 ], m[ 1 ][ 2 ], m[ 2 ][ 2 ], m[ 3 ][ 2 ] );
		c3.Set( m[ 0 ][ 3 ], m[ 1 ][ 3 ], m[ 2 ][ 3 ], m[ 3 ][ 3 ] );
	}

	INLINE void SMat4::SetColumn(uint i, const SVec4 &column)
	{
		auto &mat = *this;

		mat( 0, i ) = column.X( );
		mat( 1, i ) = column.Y( );
		mat( 2, i ) = column.Z( );
		mat( 3, i ) = column.W( );
	}

	INLINE SVec4 SMat4::GetColumn(uint i) const
	{
		auto &mat = *this;

		return { mat( 0, i ), mat( 1, i ), mat( 2, i ), mat( 3, i ) };
	}

	INLINE SVec3 SMat4::TransformVector(const SVec3 &vector) const
	{
		float x = vector.X( );
		float y = vector.Y( );
		float z = vector.Z( );

		return {
			m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ] * z,
			m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ] * z,
			m[ 2 ][ 0 ] * x + m[ 2 ][ 1 ] * y + m[ 2 ][ 2 ] * z
		};
	}

	INLINE void SMat4::TransformVectorInplace(SVec3 &vector) const
	{
		float x = vector.X( );
		float y = vector.Y( );
		float z = vector.Z( );

		vector.X( ) = m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ] * z;
		vector.Y( ) = m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ] * z;
		vector.Z( ) = m[ 2 ][ 0 ] * x + m[ 2 ][ 1 ] * y + m[ 2 ][ 2 ] * z;
	}

	INLINE SVec3 SMat4::TransformPoint(const SVec3 &point) const
	{
		float x = point.X( );
		float y = point.Y( );
		float z = point.Z( );

		return {
			m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ] * z + m[ 0 ][ 3 ],
			m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ] * z + m[ 1 ][ 3 ],
			m[ 2 ][ 0 ] * x + m[ 2 ][ 1 ] * y + m[ 2 ][ 2 ] * z + m[ 2 ][ 3 ]
		};
	}

	INLINE void SMat4::TransformPointInplace(SVec3 &point) const
	{
		float x = point.X( );
		float y = point.Y( );
		float z = point.Z( );

		point.X( ) = m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ] * z + m[ 0 ][ 3 ];
		point.Y( ) = m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ] * z + m[ 1 ][ 3 ];
		point.Z( ) = m[ 2 ][ 0 ] * x + m[ 2 ][ 1 ] * y + m[ 2 ][ 2 ] * z + m[ 2 ][ 3 ];
	}

	INLINE SVec3 SMat4::TransformPointAndDiv(const SVec3 &point) const
	{
		float x = point.X( );
		float y = point.Y( );
		float z = point.Z( );
		float w = m[ 3 ][ 0 ] * x + m[ 3 ][ 1 ] * y + m[ 3 ][ 2 ] * z + m[ 3 ][ 3 ];
		float inv_div = math::IsZero( w ) ? 1.0f : 1.0f / w;

		return {
			inv_div * ( m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ] * z + m[ 0 ][ 3 ] ),
			inv_div * ( m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ] * z + m[ 1 ][ 3 ] ),
			inv_div * ( m[ 2 ][ 0 ] * x + m[ 2 ][ 1 ] * y + m[ 2 ][ 2 ] * z + m[ 2 ][ 3 ] )
		};
	}

	INLINE void SMat4::TransformPointAndDivInplace(SVec3 &point) const
	{
		float x = point.X( );
		float y = point.Y( );
		float z = point.Z( );
		float w = m[ 3 ][ 0 ] * x + m[ 3 ][ 1 ] * y + m[ 3 ][ 2 ] * z + m[ 3 ][ 3 ];
		float inv_div = math::IsZero( w ) ? 1.0f : 1.0f / w;

		point.X( ) = inv_div * ( m[ 0 ][ 0 ] * x + m[ 0 ][ 1 ] * y + m[ 0 ][ 2 ] * z + m[ 0 ][ 3 ] );
		point.Y( ) = inv_div * ( m[ 1 ][ 0 ] * x + m[ 1 ][ 1 ] * y + m[ 1 ][ 2 ] * z + m[ 1 ][ 3 ] );
		point.Z( ) = inv_div * ( m[ 2 ][ 0 ] * x + m[ 2 ][ 1 ] * y + m[ 2 ][ 2 ] * z + m[ 2 ][ 3 ] );
	}

	INLINE void SMat4::D3DOrthoProjLH(float n, float f, float h, float v)
	{
		m[ 0 ][ 0 ] = 2.f / h;
		m[ 0 ][ 1 ] = 0;
		m[ 0 ][ 2 ] = 0;
		m[ 0 ][ 3 ] = 0.f;
		m[ 1 ][ 0 ] = 0;
		m[ 1 ][ 1 ] = 2.f / v;
		m[ 1 ][ 2 ] = 0;
		m[ 1 ][ 3 ] = 0.f;
		m[ 2 ][ 0 ] = 0;
		m[ 2 ][ 1 ] = 0;
		m[ 2 ][ 2 ] = 1.f / ( f - n );
		m[ 2 ][ 3 ] = n / ( n - f );
		m[ 3 ][ 0 ] = 0;
		m[ 3 ][ 1 ] = 0;
		m[ 3 ][ 2 ] = 0.f;
		m[ 3 ][ 3 ] = 1.f;
	}

	INLINE void SMat4::D3DOrthoProjRH(float n, float f, float h, float v)
	{
		m[ 0 ][ 0 ] = 2.f / h;
		m[ 0 ][ 1 ] = 0;
		m[ 0 ][ 2 ] = 0;
		m[ 0 ][ 3 ] = 0.f;
		m[ 1 ][ 0 ] = 0;
		m[ 1 ][ 1 ] = 2.f / v;
		m[ 1 ][ 2 ] = 0;
		m[ 1 ][ 3 ] = 0.f;
		m[ 2 ][ 0 ] = 0;
		m[ 2 ][ 1 ] = 0;
		m[ 2 ][ 2 ] = 1.f / ( n - f );
		m[ 2 ][ 3 ] = n / ( n - f );
		m[ 3 ][ 0 ] = 0;
		m[ 3 ][ 1 ] = 0;
		m[ 3 ][ 2 ] = 0.f;
		m[ 3 ][ 3 ] = 1.f;
	}

	INLINE void SMat4::D3DPerspProjLH(float n, float f, float h, float v)
	{
		m[ 0 ][ 0 ] = 2.f * n / h;
		m[ 0 ][ 1 ] = 0;
		m[ 0 ][ 2 ] = 0;
		m[ 0 ][ 3 ] = 0.f;
		m[ 1 ][ 0 ] = 0;
		m[ 1 ][ 1 ] = 2.f * n / v;
		m[ 1 ][ 2 ] = 0;
		m[ 1 ][ 3 ] = 0.f;
		m[ 2 ][ 0 ] = 0;
		m[ 2 ][ 1 ] = 0;
		m[ 2 ][ 2 ] = f / ( f - n );
		m[ 2 ][ 3 ] = n * f / ( n - f );
		m[ 3 ][ 0 ] = 0;
		m[ 3 ][ 1 ] = 0;
		m[ 3 ][ 2 ] = 1.f;
		m[ 3 ][ 3 ] = 0.f;
	}

	INLINE void SMat4::D3DPerspProjRH(float n, float f, float h, float v)
	{
		m[ 0 ][ 0 ] = 2.f * n / h;
		m[ 0 ][ 1 ] = 0;
		m[ 0 ][ 2 ] = 0;
		m[ 0 ][ 3 ] = 0.f;
		m[ 1 ][ 0 ] = 0;
		m[ 1 ][ 1 ] = 2.f * n / v;
		m[ 1 ][ 2 ] = 0;
		m[ 1 ][ 3 ] = 0.f;
		m[ 2 ][ 0 ] = 0;
		m[ 2 ][ 1 ] = 0;
		m[ 2 ][ 2 ] = f / ( n - f );
		m[ 2 ][ 3 ] = n * f / ( n - f );
		m[ 3 ][ 0 ] = 0;
		m[ 3 ][ 1 ] = 0;
		m[ 3 ][ 2 ] = -1.f;
		m[ 3 ][ 3 ] = 0.f;
	}

	INLINE void SMat4::OGLOrthoProjLH(float n, float f, float h, float v)
	{
		m[ 0 ][ 0 ] = 2.f / h;
		m[ 0 ][ 1 ] = 0;
		m[ 0 ][ 2 ] = 0;
		m[ 0 ][ 3 ] = 0.f;
		m[ 1 ][ 0 ] = 0;
		m[ 1 ][ 1 ] = 2.f / v;
		m[ 1 ][ 2 ] = 0;
		m[ 1 ][ 3 ] = 0.f;
		m[ 2 ][ 0 ] = 0;
		m[ 2 ][ 1 ] = 0;
		m[ 2 ][ 2 ] = 2.f / ( f - n );
		m[ 2 ][ 3 ] = ( f + n ) / ( n - f );
		m[ 3 ][ 0 ] = 0;
		m[ 3 ][ 1 ] = 0;
		m[ 3 ][ 2 ] = 0;
		m[ 3 ][ 3 ] = 1.f;
	}

	INLINE void SMat4::OGLOrthoProjRH(float n, float f, float h, float v)
	{
		m[ 0 ][ 0 ] = 2.f / h;
		m[ 0 ][ 1 ] = 0;
		m[ 0 ][ 2 ] = 0;
		m[ 0 ][ 3 ] = 0.f;
		m[ 1 ][ 0 ] = 0;
		m[ 1 ][ 1 ] = 2.f / v;
		m[ 1 ][ 2 ] = 0;
		m[ 1 ][ 3 ] = 0.f;
		m[ 2 ][ 0 ] = 0;
		m[ 2 ][ 1 ] = 0;
		m[ 2 ][ 2 ] = 2.f / ( n - f );
		m[ 2 ][ 3 ] = ( f + n ) / ( n - f );
		m[ 3 ][ 0 ] = 0;
		m[ 3 ][ 1 ] = 0;
		m[ 3 ][ 2 ] = 0;
		m[ 3 ][ 3 ] = 1.f;
	}

	INLINE void SMat4::OGLPerspProjLH(float n, float f, float h, float v)
	{
		m[ 0 ][ 0 ] = 2.f * n / h;
		m[ 0 ][ 1 ] = 0;
		m[ 0 ][ 2 ] = 0;
		m[ 0 ][ 3 ] = 0.f;
		m[ 1 ][ 0 ] = 0;
		m[ 1 ][ 1 ] = 2.f * n / v;
		m[ 1 ][ 2 ] = 0;
		m[ 1 ][ 3 ] = 0.f;
		m[ 2 ][ 0 ] = 0;
		m[ 2 ][ 1 ] = 0;
		m[ 2 ][ 2 ] = ( n + f ) / ( f - n );
		m[ 2 ][ 3 ] = 2.f * n * f / ( n - f );
		m[ 3 ][ 0 ] = 0;
		m[ 3 ][ 1 ] = 0;
		m[ 3 ][ 2 ] = 1.f;
		m[ 3 ][ 3 ] = 0.f;
	}

	INLINE void SMat4::OGLPerspProjRH(float n, float f, float h, float v)
	{
		m[ 0 ][ 0 ] = 2.f * n / h;
		m[ 0 ][ 1 ] = 0;
		m[ 0 ][ 2 ] = 0;
		m[ 0 ][ 3 ] = 0.f;
		m[ 1 ][ 0 ] = 0;
		m[ 1 ][ 1 ] = 2.f * n / v;
		m[ 1 ][ 2 ] = 0;
		m[ 1 ][ 3 ] = 0.f;
		m[ 2 ][ 0 ] = 0;
		m[ 2 ][ 1 ] = 0;
		m[ 2 ][ 2 ] = ( n + f ) / ( n - f );
		m[ 2 ][ 3 ] = 2.f * n * f / ( n - f );
		m[ 3 ][ 0 ] = 0;
		m[ 3 ][ 1 ] = 0;
		m[ 3 ][ 2 ] = -1.f;
		m[ 3 ][ 3 ] = 0.f;
	}

	// Accessors
	INLINE float &SMat4::operator()(uint row, uint column)
	{
		return m[ row ][ column ];
	}

	INLINE float SMat4::operator()(uint row, uint column) const
	{
		return m[ row ][ column ];
	}

	INLINE const float *SMat4::GetFloatPtr(void) const
	{
		return m[ 0 ].GetFloatPtr( );
	}

	// Operators
	INLINE SMat4 SMat4::operator*(const SMat4 &rhs) const
	{
		return {
			rhs.dotCol0( m[ 0 ] ), rhs.dotCol1( m[ 0 ] ), rhs.dotCol2( m[ 0 ] ), rhs.dotCol3( m[ 0 ] ),
			rhs.dotCol0( m[ 1 ] ), rhs.dotCol1( m[ 1 ] ), rhs.dotCol2( m[ 1 ] ), rhs.dotCol3( m[ 1 ] ),
			rhs.dotCol0( m[ 2 ] ), rhs.dotCol1( m[ 2 ] ), rhs.dotCol2( m[ 2 ] ), rhs.dotCol3( m[ 2 ] ),
			rhs.dotCol0( m[ 3 ] ), rhs.dotCol1( m[ 3 ] ), rhs.dotCol2( m[ 3 ] ), rhs.dotCol3( m[ 3 ] )
		};
	}

	INLINE SMat4 SMat4::operator*(float rhs) const
	{
		return {
			m[ 0 ] * rhs,
			m[ 1 ] * rhs,
			m[ 2 ] * rhs,
			m[ 3 ] * rhs
		};
	}

	INLINE SVec4 SMat4::operator*(const SVec4 &rhs) const
	{
		return {
			m[ 0 ].Dot( rhs ),
			m[ 1 ].Dot( rhs ),
			m[ 2 ].Dot( rhs ),
			m[ 3 ].Dot( rhs )
		};
	}

	INLINE SMat4 SMat4::operator+(const SMat4 &rhs) const
	{
		auto &rhs_m = rhs.m;

		return {
			m[ 0 ] + rhs_m[ 0 ],
			m[ 1 ] + rhs_m[ 1 ],
			m[ 2 ] + rhs_m[ 2 ],
			m[ 3 ] + rhs_m[ 3 ]
		};
	}

	INLINE SMat4 SMat4::operator-(const SMat4 &rhs) const
	{
		auto &rhs_m = rhs.m;

		return {
			m[ 0 ] - rhs_m[ 0 ],
			m[ 1 ] - rhs_m[ 1 ],
			m[ 2 ] - rhs_m[ 2 ],
			m[ 3 ] - rhs_m[ 3 ]
		};
	}

	INLINE const SMat4 &SMat4::operator=(const SMat4 &rhs)
	{
		auto &rhs_m = rhs.m;

		m[ 0 ] = rhs_m[ 0 ];
		m[ 1 ] = rhs_m[ 1 ];
		m[ 2 ] = rhs_m[ 2 ];
		m[ 3 ] = rhs_m[ 3 ];

		return *this;
	}

	INLINE const SMat4 &SMat4::operator*=(const SMat4 &rhs)
	{
		Set(
			rhs.dotCol0( m[ 0 ] ), rhs.dotCol1( m[ 0 ] ), rhs.dotCol2( m[ 0 ] ), rhs.dotCol3( m[ 0 ] ),
			rhs.dotCol0( m[ 1 ] ), rhs.dotCol1( m[ 1 ] ), rhs.dotCol2( m[ 1 ] ), rhs.dotCol3( m[ 1 ] ),
			rhs.dotCol0( m[ 2 ] ), rhs.dotCol1( m[ 2 ] ), rhs.dotCol2( m[ 2 ] ), rhs.dotCol3( m[ 2 ] ),
			rhs.dotCol0( m[ 3 ] ), rhs.dotCol1( m[ 3 ] ), rhs.dotCol2( m[ 3 ] ), rhs.dotCol3( m[ 3 ] )
		);

		return *this;
	}

	INLINE const SMat4 &SMat4::operator*=(float rhs)
	{
		m[ 0 ] *= rhs;
		m[ 1 ] *= rhs;
		m[ 2 ] *= rhs;
		m[ 3 ] *= rhs;

		return *this;
	}

	INLINE const SMat4 &SMat4::operator+=(const SMat4 &rhs)
	{
		auto &rhs_m = rhs.m;

		m[ 0 ] += rhs_m[ 0 ];
		m[ 1 ] += rhs_m[ 1 ];
		m[ 2 ] += rhs_m[ 2 ];
		m[ 3 ] += rhs_m[ 3 ];

		return *this;
	}

	INLINE const SMat4 &SMat4::operator-=(const SMat4 &rhs)
	{
		auto &rhs_m = rhs.m;

		m[ 0 ] -= rhs_m[ 0 ];
		m[ 1 ] -= rhs_m[ 1 ];
		m[ 2 ] -= rhs_m[ 2 ];
		m[ 3 ] -= rhs_m[ 3 ];

		return *this;
	}

	INLINE bool SMat4::operator==(const SMat4 &rhs) const
	{
		bool result = true;
		auto &rhs_m = rhs.m;

		result = result && m[ 0 ] == rhs_m[ 0 ];
		result = result && m[ 1 ] == rhs_m[ 1 ];
		result = result && m[ 2 ] == rhs_m[ 2 ];
		result = result && m[ 3 ] == rhs_m[ 3 ];

		return result;
	}

	INLINE bool SMat4::operator!=(const SMat4 &rhs) const
	{
		return !( *this == rhs );
	}
}
