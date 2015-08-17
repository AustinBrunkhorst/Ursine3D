/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** SIMD.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

// TODO: Aligned allocation

// SIMD setup for Windows environment
#ifdef _WIN32
/* Windows. -----------------------------------------------------------------*/

    #if defined(__MINGW32__) || defined(__CYGWIN__) || (defined (_MSC_VER) && _MSC_VER < 1300)

        // data alignment
        #define ALIGNED16(a) a
        #define ALIGNED64(a) a
        #define ALIGNED128(a) a

        // force inline
        #define INLINE inline

    #else

        // data alignment
        #define ALIGNED16(a) __declspec(align(16)) a
        #define ALIGNED64(a) __declspec(align(64)) a
        #define ALIGNED128(a) __declspec(align(128)) a

        // force inline
        #define INLINE __forceinline

        /* SSE compatable compiler */
        #if (defined (_WIN32) && (_MSC_VER) && _MSC_VER >= 1400)

            #define USE_SSE
            #include <intrin.h>

        #endif

    #endif // !__MINGW32__

/* !Windows. ----------------------------------------------------------------*/

#else
/* Everything else. ---------------------------------------------------------*/

    /* GCC-compatible compiler, targeting x86/x86-64 */
    #if (defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__)))

        // data alignment
        #define ALIGNED16(a) a __attribute__ ((aligned (16)))
        #define ALIGNED64(a) a __attribute__ ((aligned (64)))
        #define ALIGNED128(a) a __attribute__ ((aligned (128)))

        // force inline
        #define INLINE inline __attribute__ ((always_inline))

        #define USE_SSE
        #include <x86intrin.h>

    #endif

/* !Everything else. --------------------------------------------------------*/
#endif


// SIMD data type declaration and helpers
#ifdef USE_SSE

    typedef __m128 SIMDvec;

#define FFF0Mask _mm_castsi128_ps(_mm_set_epi32(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF))
#define SHUFFLE(x, y, z, w) _MM_SHUFFLE(w, z, y, x)
#endif // !USE_SIMD
