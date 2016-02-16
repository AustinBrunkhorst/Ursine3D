/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Curves.hpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    INLINE SVec3 Curves::CatmullRomSpline(const SVec3 &p0, const SVec3 &p1, 
                                          const SVec3 &p2, const SVec3 &p3, float t)
    {
        // Result = ((-t^3 + 2 * t^2 - t) * Position0 +
        //           (3 * t^3 - 5 * t^2 + 2) * Position1 +
        //           (-3 * t^3 + 4 * t^2 + t) * Position2 +
        //           (t^3 - t^2) * Position3) * 0.5

#ifdef USE_SSE

        float t2 = t * t;
        float t3 = t * t2;

        auto T0 = _mm_set_ps1( (-t3 + 2.0f * t2 - t) * 0.5f );
        auto T1 = _mm_set_ps1( (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f );
        auto T2 = _mm_set_ps1( (-3.0f * t3 + 4.0f * t2 + t) * 0.5f );
        auto T3 = _mm_set_ps1( (t3 - t2) * 0.5f );

        T0 = _mm_mul_ps( T0, reinterpret_cast<const SIMDvec&>( p0 ) );
        T1 = _mm_mul_ps( T1, reinterpret_cast<const SIMDvec&>( p1 ) );
        T2 = _mm_mul_ps( T2, reinterpret_cast<const SIMDvec&>( p2 ) );
        T3 = _mm_mul_ps( T3, reinterpret_cast<const SIMDvec&>( p3 ) );
        T0 = _mm_add_ps( T0, T1 );
        T2 = _mm_add_ps( T2, T3 );
        T0 = _mm_add_ps( T0, T2 );

        return SVec3( T0 );

#else

        float t2 = t * t;
        float t3 = t * t2;

        return ((-t3 + 2.0f * t2 - t) * p0 +
                (3.0f * t3 - 5.0f * t2 + 2.0f) * p1 +
                (-3.0f * t3 + 4.0f * t2 + t) * p2 +
                (t3 - t2) * p3) * 0.5f;

#endif
    }
}
