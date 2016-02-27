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
            , m_base( new RenderableComponentBase( std::bind( &Billboard2D::updateRenderer, this ) ) )
        {
            m_base->SetHandle( m_graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_BILLBOARD2D ) );

            m_billboard = &m_graphics->RenderableMgr.GetBillboard2D( m_base->GetHandle( ) );
            m_billboard->SetDimensions( 100.0f, 100.0f );
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
            auto *owner = GetOwner( );

            m_base->OnInitialize( owner );

            // set the unique id
            m_billboard->SetEntityUniqueID( owner->GetUniqueID( ) );
        }

        void Billboard2D::OnSceneReady(Scene *scene)
        {
            invalidateTexture( );
        }

        graphics::Billboard2D *Billboard2D::GetBillboard(void)
        {
            return m_billboard;
        }

        RenderMask Billboard2D::GetRenderMask(void) const
        {
            return static_cast<RenderMask>( m_billboard->GetRenderMask( ) & 0xFFFFFFFF );
        }

        void Billboard2D::SetRenderMask(RenderMask mask)
        {
            m_billboard->SetRenderMask( static_cast<unsigned long long>( mask ) );
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

        void Billboard2D::updateRenderer(void)
        {
            auto trans = GetOwner( )->GetTransform( );

            m_billboard->SetPosition( trans->GetWorldPosition( ) );
        }

        void Billboard2D::invalidateTexture(void)
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

                m_graphics->ResourceMgr.LoadTexture( handle );

                m_billboard->SetTextureHandle( handle );
            }
        }
    }
}