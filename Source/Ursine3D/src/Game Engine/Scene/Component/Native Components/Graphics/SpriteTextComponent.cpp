/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleColorAnimatorComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SpriteTextComponent.h"
#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( SpriteText );

        SpriteText::SpriteText(void)
            : BaseComponent( )
            , m_spriteText( nullptr )
            , m_base( nullptr )
        {
            auto *graphics = GetCoreSystem( graphics::GfxAPI );

            m_base = std::make_shared<RenderableComponentBase>( std::bind( &SpriteText::updateRenderer, this ) );

            m_base->SetHandle( graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_SPRITE_TEXT ) );

            m_spriteText = &graphics->RenderableMgr.GetSpriteText( m_base->GetHandle( ) );

            m_spriteText->SetFontHandle( 0 );
            m_spriteText->SetRenderMask( 0 );
        }

        SpriteText::~SpriteText(void)
        {
            m_base->OnRemove( GetOwner( ) );

            m_spriteText->SetDebug( false );

            GetCoreSystem( graphics::GfxAPI )->RenderableMgr.DestroyRenderable( m_base->GetHandle( ) );
        }

        void SpriteText::OnInitialize(void)
        {
            Component::OnInitialize( );

            auto &owner = GetOwner( );

            m_base->OnInitialize( owner );

            // set the unique id
            m_spriteText->SetEntityID( owner->GetID( ) );

            updateRenderer( );

            bindResourceModification( m_font, &SpriteText::invalidateFont );
        }

        void SpriteText::OnSceneReady(Scene *scene)
        {
            invalidateFont( );
        }

        const resources::ResourceReference &SpriteText::GetFont(void) const
        {
            return m_font;
        }

        void SpriteText::SetFont(const resources::ResourceReference &font)
        {
            m_font = font;

            if (!resourcesAreAvailable( ))
                return;

            invalidateFont( );

            NOTIFY_COMPONENT_CHANGED( "font", m_font );
        }

        float SpriteText::GetSize(void) const
        {
            return m_spriteText->GetSize( );
        }

        void SpriteText::SetSize(float size)
        {
            m_spriteText->SetSize( size );
        }

        float SpriteText::GetWidth(void) const
        {
            return m_spriteText->GetWidth( );
        }

        void SpriteText::SetWidth(float size)
        {
            m_spriteText->SetWidth( size );
        }

        float SpriteText::GetHeight(void) const
        {
            return m_spriteText->GetHeight( );
        }

        void SpriteText::SetHeight(float size)
        {
            m_spriteText->SetHeight( size );
        }

        const SVec3 &SpriteText::GetPosition(void) const
        {
            return m_spriteText->GetPosition( );
        }

        void SpriteText::SetPosition(const SVec3 &position)
        {
            m_spriteText->SetPosition( position );
        }

        const std::string &SpriteText::GetText(void) const
        {
            return m_spriteText->GetText( );
        }

        void SpriteText::SetText(const std::string &text)
        {
            m_spriteText->SetText( text );
        }

        float SpriteText::GetPPU(void) const
        {
            return m_spriteText->GetPPU( );
        }

        void SpriteText::SetPPU(float ppu)
        {
            m_spriteText->SetPPU( ppu );
        }

        TextAlignment SpriteText::GetAlignment(void) const
        {
            return static_cast<TextAlignment>( m_spriteText->GetAlignment( ) );
        }

        void SpriteText::SetAlignment(TextAlignment alignment)
        {
            m_spriteText->SetAlignment( static_cast<graphics::SpriteText::Alignment>( alignment ) );
        }

        bool SpriteText::GetFilter(void) const
        {
            return m_spriteText->GetFilter( );
        }

        void SpriteText::SetFilter(bool useFilter)
        {
            m_spriteText->SetFilter( useFilter );
        }

        const Color &SpriteText::GetColor(void) const
        {
            return m_spriteText->GetColor( );
        }

        void SpriteText::SetColor(const Color &color)
        {
            m_spriteText->SetColor( color );

            NOTIFY_COMPONENT_CHANGED( "textColor", color );
        }

        void SpriteText::SetOverdraw(bool flag)
        {
            m_spriteText->SetOverdraw( flag );
        }

        bool SpriteText::GetOverdraw(void) const
        {
            return m_spriteText->GetOverdraw( );
        }

        void SpriteText::updateRenderer(void)
        {
            auto *transform = GetOwner( )->GetTransform( );

            // manually update our width and height
            m_spriteText->SetWidth( transform->GetWorldScale( ).X( ) );
            m_spriteText->SetHeight( transform->GetWorldScale( ).Y( ) );

            m_spriteText->SetPosition( transform->GetWorldPosition( ) );
        }

        void SpriteText::invalidateFont(void)
        {
            auto data = loadResource<resources::FontData>( m_font );

            if (data == nullptr)
            {
                // default
                m_spriteText->SetFontHandle( 0 );
            }
            else
            {
                auto handle = data->GetFontHandle( );
                GetCoreSystem(graphics::GfxAPI)->ResourceMgr.UnloadFontTextures( m_spriteText->GetFontHandle( ) );
                m_spriteText->SetFontHandle( handle );
                GetCoreSystem(graphics::GfxAPI)->ResourceMgr.LoadFontTextures( handle );
                
            }
        }
    }
}
