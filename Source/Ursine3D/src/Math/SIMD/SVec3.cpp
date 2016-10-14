/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** SVec3.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#ifdef URSINE_GRAPHICS_DIRECTX

#include "DirectXMath.h"

#endif

#if defined(URSINE_DEPENDENCY_WWise)

#include "Ak/SoundEngine/Common/AkTypes.h"

#endif

#if defined(URSINE_DEPENDENCY_BulletPhysics)

#include "LinearMath/btVector3.h"

#endif

namespace ursine
{
    SVec3::SVec3(const Vec2 &value, float Z)
        : m_x( value.X( ) )
        , m_y( value.Y( ) )
        , m_z( Z )
    {
#ifdef USE_SSE
        m_w = 0.0f;
#endif
    }

    SVec3::SVec3(const Vec3 &value)
        : m_x( value.X( ) )
        , m_y( value.Y( ) )
        , m_z( value.Z( ) )
    {
#ifdef USE_SSE
        m_w = 0.0f;
#endif
    }

    SVec3::SVec3(const SVec4 &value)
        : m_x( value.X( ) )
        , m_y( value.Y( ) )
        , m_z( value.Z( ) )
    {
#ifdef USE_SSE
        m_w = 0.0f;
#endif
    }

    SVec3::SVec3(const Vec4 &value)
        : m_x( value.X( ) )
        , m_y( value.Y( ) )
        , m_z( value.Z( ) )
    {
#ifdef USE_SSE
        m_w = 0.0f;
#endif
    }

#ifdef URSINE_GRAPHICS_DIRECTX
    SVec3::SVec3(const DirectX::XMFLOAT3 &vec)
        : m_x( vec.x )
        , m_y( vec.y )
        , m_z( vec.z )
    {
#ifdef USE_SSE
        m_w = 0.0f;
#endif
    }

    DirectX::XMFLOAT3 SVec3::ToD3D(void) const
    {
        return{ m_x, m_y, m_z };
    }
#endif

#if defined(URSINE_DEPENDENCY_WWise)
    SVec3::SVec3(const AkVector & vec)
        : m_x( vec.X )
        , m_y( vec.Y )
        , m_z( vec.Z )
    {
#ifdef USE_SSE
        m_w = 0.0f;
#endif
    }

    AkVector SVec3::ToWwise(void) const
    {
        AkVector vec;

        vec.X = m_x;
        vec.Y = m_y;
        vec.Z = m_z;

        return vec;
    }
#endif

#if defined(URSINE_DEPENDENCY_BulletPhysics)
    SVec3::SVec3(const btVector3 &vec)
        : m_x( vec.getX( ) )
        , m_y( vec.getY( ) )
        , m_z( vec.getZ( ) )
    {
#ifdef USE_SSE
        m_w = 0.0f;
#endif
    }

    btVector3 SVec3::ToBullet(void) const
    {
        return btVector3( m_x, m_y, m_z );
    }
#endif

#if defined(URSINE_DEPENDENCY_Assimp)
    SVec3::SVec3(const aiVector3D &vec)
        : m_x( vec.x )
        , m_y( vec.y )
        , m_z( vec.z )
    {
#ifdef USE_SSE
        m_w = 0.0f;
#endif
    }

    aiVector3D SVec3::ToAssimp(void) const
    {
        return aiVector3D( m_x, m_y, m_z );
    }
#endif

    // Public Methods
    void SVec3::Clean(void)
    {
        m_x = math::IsZero( m_x ) ? 0.0f : m_x;
        m_y = math::IsZero( m_y ) ? 0.0f : m_y;
        m_z = math::IsZero( m_z ) ? 0.0f : m_z;
    }

    std::string SVec3::ToString(void) const
    {
        std::ostringstream x_cvt;
        x_cvt << m_x;

        std::ostringstream y_cvt;
        y_cvt << m_y;

        std::ostringstream z_cvt;
        z_cvt << m_z;

        return { "{" +
            x_cvt.str( ) + ", " +
            y_cvt.str( ) + ", " +
            z_cvt.str( ) +
            "}" };
    }
}
