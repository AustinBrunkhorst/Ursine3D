/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Billboard2DComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "Billboard2DComponent.h"
#include "CoreSystem.h"
#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Billboard2D );

        Billboard2D::Billboard2D(void) 
            : BaseComponent( )
            , m_graphics( GetCoreSystem( graphics::GfxAPI ) )
            , m_billboard( nullptr )
            , m_screenSpace( true )
            , m_base( new RenderableComponentBase( std::bind( &Billboard2D::updateRenderer, this ) ) )
        {
            m_base->SetHandle( m_graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_BILLBOARD2D ) );

            m_billboard = &m_graphics->RenderableMgr.GetBillboard2D( m_base->GetHandle( ) );

            SetScale( { 0.1f, 0.1f } );
        } 

        Billboard2D::~Billboard2D(void)
        {
            m_base->OnRemove( GetOwner( ) );

            m_graphics->RenderableMgr.DestroyRenderable( m_base->GetHandle( ) );

            // release resource
            if (m_billboard->GetTextureHandle( ) != 0)
            {
                m_graphics->ResourceMgr.UnloadTexture(
                    m_billboard->GetTextureHandle( )
                );
            }

            delete m_base;
        }

        void Billboard2D::OnInitialize(void) 
        {
            auto &owner = GetOwner( );

            m_base->OnInitialize( owner );

            // set the unique id
            m_billboard->SetEntityID( owner->GetID( ) );

        #if defined(URSINE_WITH_EDITOR)

            bindResourceModification( m_texture, &Billboard2D::onTextureReload );

        #endif
        }

        void Billboard2D::OnSceneReady(Scene *scene)
        {
            invalidateTexture( false );
        }

        graphics::Billboard2D *Billboard2D::GetBillboard(void)
        {
            return m_billboard;
        }

        const resources::ResourceReference &Billboard2D::GetTexture(void) const
        {
            return m_texture;
        }

        void Billboard2D::SetTexture(const resources::ResourceReference &texture)
        {
            m_texture = texture;

            if (!resourcesAreAvailable( ))
                return;

            invalidateTexture( );

            NOTIFY_COMPONENT_CHANGED( "texture", m_texture );
        }

        Vec2 Billboard2D::GetScale(void) const
        {
            return m_scale;
        }

        void Billboard2D::SetScale(const Vec2 &scale)
        {
            m_scale = scale;

            m_base->dirty = true;

            NOTIFY_COMPONENT_CHANGED( "screenRatio", m_scale );
        }

        RenderMask Billboard2D::GetRenderMask(void) const
        {
            return static_cast<RenderMask>( m_billboard->GetRenderMask( ) & 0xFFFFFFFF );
        }

        bool Billboard2D::GetScreenSpace(void) const
        {
            return m_screenSpace;
        }

        void Billboard2D::SetScreenSpace(bool flag)
        {
            m_screenSpace = flag;

            m_base->dirty = true;

            NOTIFY_COMPONENT_CHANGED( "screenSpace", m_screenSpace );
        }

        void Billboard2D::SetRenderMask(RenderMask mask)
        {
            m_billboard->SetRenderMask( static_cast<unsigned long long>( mask ) );
        }

        void Billboard2D::updateRenderer(void)
        {
            // Set the position
            auto trans = GetOwner( )->GetTransform( );

            m_billboard->SetPosition( trans->GetWorldPosition( ) );


            // Set the dimensions (pixels)
            auto scene = GetOwner( )->GetWorld( )->GetOwner( );

            if (!scene)
                return;

            auto handle = scene->GetViewport( );

            auto &viewport = GetCoreSystem( graphics::GfxAPI )->ViewportMgr.GetViewport( handle );

            unsigned width, height;

            viewport.GetDimensions( width, height );

            float fWidth = static_cast<float>( width ),
                  fHeight = static_cast<float>( height );

            Vec2 pixels;

            if (fWidth > fHeight)
            {
                float heightRatio = fWidth / fHeight;

                pixels = m_scale * Vec2( 1.0f, heightRatio ) *
                         Vec2( fWidth, fHeight );
            }
            else
            {
                float widthRatio = fHeight / fWidth;

                pixels = m_scale * Vec2( widthRatio, 1.0f ) *
                         Vec2( fWidth, fHeight );
            }

            m_billboard->SetDimensions( pixels.X( ), pixels.Y( ) );
        }

        void Billboard2D::invalidateTexture(bool unload)
        {
            auto data = loadResource<resources::TextureData>( m_texture );

            if (data == nullptr)
            {
                // default
                m_billboard->SetTextureHandle( 0 );
            }
            else
            {
                auto handle = data->GetTextureHandle( );

                if (unload)
                    m_graphics->ResourceMgr.UnloadTexture( m_billboard->GetTextureHandle( ) );

                m_graphics->ResourceMgr.LoadTexture( handle );

                m_billboard->SetTextureHandle( handle );
            }
        }

        #if defined(URSINE_WITH_EDITOR)

        void Billboard2D::onTextureReload(void)
        {
            if (resourcesAreAvailable( ))
                invalidateTexture( true );
        }

        #endif
    }
}