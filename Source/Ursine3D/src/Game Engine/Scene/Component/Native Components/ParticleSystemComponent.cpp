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
        NATIVE_COMPONENT_DEFINITION(ParticleSystem);

        ParticleSystem::ParticleSystem(void)
            : BaseComponent()
        {
            // store a pointer to the GfxAPI core system
            m_graphics = GetCoreSystem(graphics::GfxAPI);

            m_base = new RenderableComponentBase([=] {
                updateRenderer();
            });

            m_base->SetHandle(m_graphics->RenderableMgr.AddRenderable(graphics::RENDERABLE_PS));

            // store a pointer to the model
            m_particleSystem = &m_graphics->RenderableMgr.GetParticleSystem(m_base->GetHandle());
        }
        ParticleSystem::~ParticleSystem(void)
        {

        }

        void ParticleSystem::OnInitialize(void)
        {
            Component::OnInitialize( );

            // set the unique id
            m_particleSystem->SetEntityUniqueID(GetOwner()->GetUniqueID());
        }

        // count
        unsigned ParticleSystem::GetActiveParticleCount(void) const
        {
            return m_particleSystem->GetActiveParticleCount();
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
            return m_particleSystem->GetGPUParticle(index);
        }

        // cpu vector
        std::vector<graphics::Particle_CPU> &ParticleSystem::GetCPUParticleData(void)
        {
            return m_particleSystem->GetCPUParticleData( );
        }

        graphics::Particle_CPU &ParticleSystem::GetCPUParticle(const int index)
        {
            return m_particleSystem->GetCPUParticle(index);
        }

        // generate particle, returns index
        int ParticleSystem::SpawnParticle(void)
        {
            return m_particleSystem->SpawnParticle( );
        }

        // destroys a particle, given an index
        void ParticleSystem::DestroyParticle(const int index)
        {
            m_particleSystem->DestroyParticle(index);
        }

        void ParticleSystem::updateRenderer(void)
        {
            auto trans = GetOwner()->GetTransform();
            m_particleSystem->SetPosition(trans->GetWorldPosition( ));

            GetOwner()->Dispatch(ENTITY_PARTICLE_UPDATE, nullptr);
        }

        const Color &ParticleSystem::GetColor(void) const
        {
            // TODO: insert return statement here
            return m_particleSystem->GetColor( );
        }

        void ParticleSystem::SetColor(const Color &color)
        {
            m_particleSystem->SetColor(color);
            NOTIFY_COMPONENT_CHANGED("color", color);
        }
    }
}