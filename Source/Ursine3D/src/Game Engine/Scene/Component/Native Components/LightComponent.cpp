#include "UrsinePrecompiled.h"

#include "LightComponent.h"
#include "RenderableComponent.h"

#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Light );

        Light::Light(void)
            : BaseComponent( )
            , m_light( nullptr )
            , m_handle( 0 ) { }

        Light::~Light(void)
        {
            // TODO:
            /*GetCoreSystem( graphics::GfxAPI )->
                RenderableMgr.DestroyRenderable( m_handle );*/

            m_light = nullptr;
        }

        void Light::OnInitialize(void)
        {
            m_handle = GetCoreSystem( graphics::GfxAPI )->
                RenderableMgr.AddRenderable( graphics::RENDERABLE_LIGHT );

            m_light = &GetCoreSystem( graphics::GfxAPI )->
                RenderableMgr.GetLight( m_handle );

            m_light->Initialize( );

            auto *owner = GetOwner( );

            Renderable *renderable;

            URSINE_TODO( "Decouple this" );
            if (owner->HasComponent<Renderable>( ))
            {
                renderable = owner->GetComponent<Renderable>( );
            }
            else
            {
                renderable = owner->AddComponent<Renderable>( );
            }

            renderable->SetHandle( m_handle );
        }

        graphics::GfxHND Light::GetHandle(void) const
        {
            return m_handle;
        }

        const graphics::Light *Light::GetLight(void)
        {
            return m_light;
        }

        LightType Light::GetType(void)
        {
            return static_cast<LightType>( m_light->GetType( ) );
        }

        void Light::SetType(LightType type)
        {
            m_light->SetType( static_cast<graphics::Light::LightType>( type ) );

            NOTIFY_COMPONENT_CHANGED( "Type", type );
        }

        const SVec3 &Light::GetDirection(void)
        {
            return m_light->GetDirection( );
        }

        void Light::SetDirection(const SVec3 &direction)
        {
            m_light->SetDirection( direction );

            NOTIFY_COMPONENT_CHANGED( "Direction", direction );
        }

        const SVec3 &Light::GetPosition(void)
        {
            return m_light->GetPosition( );
        }

        void Light::SetPosition(const SVec3 &position)
        {
            m_light->SetPosition( position );

            NOTIFY_COMPONENT_CHANGED( "Position", position );
        }

        const Color &Light::GetColor(void)
        {
            return m_light->GetColor( );
        }

        void Light::SetColor(const Color &color)
        {
            m_light->SetColor( color );

            NOTIFY_COMPONENT_CHANGED( "Color", color );
        }

        float Light::GetRadius(void)
        {
            return m_light->GetRadius( );
        }

        void Light::SetRadius(const float radius)
        {
            m_light->SetRadius( radius );

            NOTIFY_COMPONENT_CHANGED( "Radius", radius );
        }

        float Light::GetIntensity(void)
        {
            return m_light->GetIntensity( );
        }

        void Light::SetIntensity(const float intensity)
        {
            m_light->SetIntensity( intensity );

            NOTIFY_COMPONENT_CHANGED( "Intensity", intensity );
        }

        const Vec2 &Light::GetSpotlightAngles(void)
        {
            return m_light->GetSpotlightAngles( );
        }

        void Light::SetSpotlightAngles(const Vec2 &angles)
        {
            m_light->SetSpotlightAngles( angles );

            NOTIFY_COMPONENT_CHANGED( "SpotLightAngles", angles );
        }
    }
}
