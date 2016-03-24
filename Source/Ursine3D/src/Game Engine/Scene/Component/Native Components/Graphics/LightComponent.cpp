/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LightComponent.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "LightComponent.h"

#include "GfxAPI.h"
#include <Game Engine/Scene/Entity/EntityEvent.h>

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Light );

        Light::Light(void)
            : BaseComponent( )
            , m_light( nullptr )
        {
            auto *graphics = GetCoreSystem( graphics::GfxAPI );

            m_base = new RenderableComponentBase([=] {
                updateRenderer( );
            });

            m_base->SetHandle( graphics->
                RenderableMgr.AddRenderable( graphics::RENDERABLE_LIGHT ) );

            m_light = &graphics->
                RenderableMgr.GetLight( m_base->GetHandle( ) );

            m_light->Initialize( );
        }

        Light::~Light(void)
        {
            m_base->OnRemove( GetOwner( ) );
            
            m_light = nullptr;

            GetCoreSystem( graphics::GfxAPI )
                ->RenderableMgr.DestroyRenderable( m_base->GetHandle( ) );

            delete m_base;
        }

        void Light::OnInitialize(void)
        {
            m_base->OnInitialize(GetOwner( ));

            updateRenderer( );
        }
        
        graphics::GfxHND Light::GetHandle(void) const
        {
            return m_base->GetHandle( );
        }

        const graphics::Light *Light::GetLight(void)
        {
            return m_light;
        }

        LightType Light::GetLightType(void)
        {
            return static_cast<LightType>( m_light->GetType( ) );
        }

        void Light::SetLightType(LightType type)
        {
            m_light->SetType( static_cast<graphics::Light::LightType>( type ) );

            NOTIFY_COMPONENT_CHANGED( "type", type );
        }

        const SVec3 &Light::GetDirection(void)
        {
            return m_light->GetDirection( );
        }

        void Light::SetDirection(const SVec3 &direction)
        {
            m_light->SetDirection( direction );

            NOTIFY_COMPONENT_CHANGED( "direction", direction );
        }

        const SVec3 &Light::GetPosition(void)
        {
            return m_light->GetPosition( );
        }

        void Light::SetPosition(const SVec3 &position)
        {
            m_light->SetPosition( position );

            NOTIFY_COMPONENT_CHANGED( "position", position );
        }

        const Color &Light::GetColor(void)
        {
            return m_light->GetColor( );
        }

        void Light::SetColor(const Color &color)
        {
            m_light->SetColor( color );

            NOTIFY_COMPONENT_CHANGED( "color", color );
        }

        float Light::GetRadius(void)
        {
            return m_light->GetRadius( );
        }

        void Light::SetRadius(const float radius)
        {
            m_light->SetRadius( radius );

            NOTIFY_COMPONENT_CHANGED( "radius", radius );
        }

        float Light::GetIntensity(void)
        {
            return m_light->GetIntensity( );
        }

        void Light::SetIntensity(const float intensity)
        {
            m_light->SetIntensity( intensity );

            NOTIFY_COMPONENT_CHANGED( "intensity", intensity );
        }

        const Vec2 &Light::GetSpotlightAngles(void)
        {
            return m_light->GetSpotlightAngles( );
        }

        void Light::SetSpotlightAngles(const Vec2 &angles)
        {
            m_light->SetSpotlightAngles( angles );

            NOTIFY_COMPONENT_CHANGED( "spotlightAngles", angles );
        }

        RenderMask Light::GetRenderMask(void) const
        {
            return static_cast<RenderMask>( m_light->GetRenderMask() & 0xFFFFFFFF );
        }

        void Light::SetRenderMask(RenderMask mask)
        {
            m_light->SetRenderMask( static_cast<unsigned long long>(mask) );
        }

        unsigned Light::GetShadowmapSize(void) const
        {
            return m_light->GetShadowmapWidth( );
        }
        void Light::SetShadowmapSize(unsigned size)
        {
            if(size > 4096)
                size = 4096;
            m_light->SetShadowmapWidth( size );
        }

        void Light::updateRenderer(void)
        {
            auto trans = GetOwner()->GetTransform();
            auto &light = GetCoreSystem(graphics::GfxAPI)->RenderableMgr.GetLight( m_base->GetHandle( ) );

            //set position using the transform
            light.SetPosition(trans->GetWorldPosition());

            //get inner/outer angles
            float outer = light.GetSpotlightAngles().Y() * 3.1415f / 180.f;

            SVec3 scale = trans->GetLocalScale();
            scale.SetX(tanf(outer / 2.f) * scale.Z() * 2.2f); //arbitrary scalar to prevent artifacts from exact sizing
            scale.SetY(tanf(outer / 2.f) * scale.Z() * 2.2f);
            trans->SetLocalScale(scale);

            // get size for spotlight, if needed
            if ( m_light->GetType() == graphics::Light::LIGHT_SPOTLIGHT )
                m_light->SetRadius(scale.Z());

            //update transform for the spotlight
            light.SetSpotlightTransform(trans->GetLocalToWorldMatrix());

            //set our direction using our rotation 
            SVec3 lightDir = SVec3(0, 0, 1);
            lightDir = trans->GetWorldRotation() * lightDir;

            light.SetDirection(lightDir);
        }
    }
}
