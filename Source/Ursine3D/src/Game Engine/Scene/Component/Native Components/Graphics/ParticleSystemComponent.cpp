/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleSystemComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "ParticleSystemComponent.h"

#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( ParticleSystem );

        ParticleSystem::ParticleSystem(void)
            : BaseComponent( )
            , m_particleColor( Color::White )
            , m_systemSpace( SystemSpace::WorldSpace )
            , m_renderMode( RenderMode::Additive )
            , m_updateInEditor( false )
        {
            // store a pointer to the GfxAPI core system
            m_graphics = GetCoreSystem( graphics::GfxAPI );

            m_base = new RenderableComponentBase( std::bind( &ParticleSystem::updateRenderer, this ) );

            m_base->SetHandle(m_graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_PS ) );

            m_base->dirty = true;

            // store a pointer to the model
            m_particleSystem = &m_graphics->RenderableMgr.GetParticleSystem( m_base->GetHandle( ) );

            SetRenderMask( RenderMask::Any );
        }

        ParticleSystem::~ParticleSystem(void)
        {
            m_base->OnRemove( GetOwner( ) );
            m_particleSystem->SetDebug( false );
            GetCoreSystem( graphics::GfxAPI )->RenderableMgr.DestroyRenderable( m_base->GetHandle( ) );

            delete m_base;
        }

        void ParticleSystem::OnInitialize(void)
        {
            Component::OnInitialize( );

            // set the unique id
            m_particleSystem->SetEntityID( GetOwner( )->GetID( ) );

            SetRenderMode( m_renderMode );
            SetSystemSpace( m_systemSpace );
        }

        void ParticleSystem::OnSceneReady(Scene *scene)
        {
            invalidateTexture( false );
        }

        // count
        unsigned ParticleSystem::GetActiveParticleCount(void) const
        {
            return m_particleSystem->GetActiveParticleCount( );
        }
        unsigned ParticleSystem::GetInactiveParticleCount(void) const
        {
            return m_particleSystem->GetInactiveParticleCount( );
        }

        // gpu vector
        std::vector<graphics::Particle_GPU> &ParticleSystem::GetGPUParticleData(void)
        {
            return m_particleSystem->GetGPUParticleData( );
        }

        graphics::Particle_GPU &ParticleSystem::GetGPUParticle(const int index)
        {
            return m_particleSystem->GetGPUParticle( index );
        }

        // cpu vector
        std::vector<graphics::Particle_CPU> &ParticleSystem::GetCPUParticleData(void)
        {
            return m_particleSystem->GetCPUParticleData( );
        }

        graphics::Particle_CPU &ParticleSystem::GetCPUParticle(const int index)
        {
            return m_particleSystem->GetCPUParticle( index );
        }

        // generate particle, returns index
        int ParticleSystem::SpawnParticle(void)
        {
            return m_particleSystem->SpawnParticle( );
        }

        // destroys a particle, given an index
        void ParticleSystem::DestroyParticle(const int index)
        {
            m_particleSystem->DestroyParticle( index );
        }

        void ParticleSystem::updateRenderer(void)
        {
            auto trans = GetOwner( )->GetTransform( );

            m_particleSystem->SetPosition( trans->GetWorldPosition( ) );
            m_particleSystem->SetTransform( trans->GetLocalToWorldMatrix( ) );

            m_base->dirty = true;
        }

        const Color &ParticleSystem::GetColor(void) const
        {
            return m_particleSystem->GetColor( );
        }

        void ParticleSystem::SetColor(const Color &color)
        {
            m_particleSystem->SetColor( color );

            NOTIFY_COMPONENT_CHANGED( "color", color );
        }

        const resources::ResourceReference &ParticleSystem::GetTexture(void) const
        {
            return m_texture;
        }

        void ParticleSystem::SetTexture(const resources::ResourceReference &texture)
        {
            m_texture = texture;

            if (!resourcesAreAvailable( ))
                return;

            invalidateTexture( );

            NOTIFY_COMPONENT_CHANGED( "texture", m_texture );
        }

        SystemSpace ParticleSystem::GetSystemSpace(void) const
        {
            return m_systemSpace;
        }

        void ParticleSystem::SetSystemSpace(const SystemSpace &space)
        {
            m_systemSpace = space;

            if (m_systemSpace == SystemSpace::WorldSpace)
                m_particleSystem->SetSystemSpace( true );
            else
                m_particleSystem->SetSystemSpace( false );
        }

        RenderMode ParticleSystem::GetRenderMode(void) const
        {
            return m_renderMode;
        }

        RenderMask ParticleSystem::GetRenderMask(void) const
        {
            return static_cast<RenderMask>( m_particleSystem->GetRenderMask( ) );
        }

        void ParticleSystem::SetRenderMask(RenderMask mask)
        {
            m_particleSystem->SetRenderMask( static_cast<unsigned long long>( mask ) );
        }

        #if defined(URSINE_WITH_EDITOR)

        bool ParticleSystem::UpdatesInEditor(void) const
        {
            return m_updateInEditor;
        }

        void ParticleSystem::SetUpdatesInEditor(bool updates)
        {
            m_updateInEditor = updates;

            NOTIFY_COMPONENT_CHANGED( "updateInEditor", m_updateInEditor );
        }

        #endif

        void ParticleSystem::SetRenderMode(const RenderMode &renderMode)
        {
            m_renderMode = renderMode;

            if (m_renderMode == RenderMode::Additive)
                m_particleSystem->SetAdditive( true );
            else
                m_particleSystem->SetAdditive( false );
        }

        void ParticleSystem::invalidateTexture(bool unload)
        {
            auto data = loadResource<resources::TextureData>( m_texture );

            if (data == nullptr)
            {
                // default
                m_particleSystem->SetTextureHandle( 0 );
            }
            else
            {
                auto handle = data->GetTextureHandle( );

                if (unload)
                    m_graphics->ResourceMgr.UnloadTexture( m_particleSystem->GetTextureHandle( ) );

                m_graphics->ResourceMgr.LoadTexture( handle );

                m_particleSystem->SetTextureHandle( handle );
            }
        }
    }
}