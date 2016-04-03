/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GraphicsSettingsComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GraphicsSettingsComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(GraphicsSettings);

        GraphicsSettings::GraphicsSettings(void)
            : BaseComponent()
        {
        }

        GraphicsSettings::~GraphicsSettings(void)
        {
        }

        void GraphicsSettings::OnInitialize(void)
        {
        }

        void GraphicsSettings::OnSceneReady(Scene *scene)
        {
            invalidateLightmap( false );
        }

        float GraphicsSettings::GetGlobalEmissive(void) const
        {
            return GetCoreSystem(graphics::GfxAPI)->GetGlobalEmissive( );
        }

        void GraphicsSettings::SetGlobalEmissive(float value)
        {
            GetCoreSystem(graphics::GfxAPI)->SetGlobalEmissive( value );
        }

        float GraphicsSettings::GetLightStepValue(void) const
        {
            return GetCoreSystem(graphics::GfxAPI)->GetLightStepValue( );
        }

        void GraphicsSettings::SetLightStepValue(float value)
        {
            GetCoreSystem(graphics::GfxAPI)->SetLightStepValue( value );
        }

        const resources::ResourceReference & GraphicsSettings::GetLightMapTexture() const
        {
            return m_lightMap;
        }

        void GraphicsSettings::SetLightMapTexture(const resources::ResourceReference &texture)
        {
            m_lightMap = texture;

            if (!resourcesAreAvailable( ))
                return;

            invalidateLightmap( );

            NOTIFY_COMPONENT_CHANGED("lightMap", m_lightMap);
        }

        void GraphicsSettings::invalidateLightmap(bool unload)
        {
            auto data = loadResource<resources::TextureData>( m_lightMap );
            auto *gfx = GetCoreSystem(graphics::GfxAPI);

            if (data == nullptr)
            {
                // default
                gfx->SetLightMapTexture( 1 );
            }
            else
            {
                auto handle = data->GetTextureHandle( );
                
                if (unload)
                    gfx->ResourceMgr.UnloadTexture( gfx->GetLightMapTexture( ) );

                gfx->ResourceMgr.LoadTexture( handle );

                gfx->SetLightMapTexture( handle );
            }
        }
    }
}