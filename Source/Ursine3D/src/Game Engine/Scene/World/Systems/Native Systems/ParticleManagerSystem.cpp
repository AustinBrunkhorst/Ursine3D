/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleManagerSystem.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ParticleManagerSystem.h"

#include "ParticleSystemComponent.h"
#include "SelectedComponent.h"

#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION( ParticleManagerSystem );

        ParticleManagerSystem::ParticleManagerSystem(World *world)
            : EntitySystem( world ) { }

        void ParticleManagerSystem::OnInitialize(void)
        {
            m_world->Listener( this )
                .On( WORLD_UPDATE, &ParticleManagerSystem::onUpdate )

        #if defined(URSINE_WITH_EDITOR)

                .On( WORLD_EDITOR_UPDATE, &ParticleManagerSystem::onEditorUpdate )

        #endif

                .On( WORLD_ENTITY_COMPONENT_ADDED, &ParticleManagerSystem::onComponentAdded )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &ParticleManagerSystem::onComponentRemoved );
        }

        void ParticleManagerSystem::OnRemove(void)
        {
            m_world->Listener( this )
                .Off( WORLD_UPDATE, &ParticleManagerSystem::onUpdate )

        #if defined(URSINE_WITH_EDITOR)

                .Off( WORLD_EDITOR_UPDATE, &ParticleManagerSystem::onEditorUpdate )

        #endif

                .Off( WORLD_ENTITY_COMPONENT_ADDED, &ParticleManagerSystem::onComponentAdded )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &ParticleManagerSystem::onComponentRemoved );
        }

        void ParticleManagerSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            auto *component = const_cast<Component*>( args->component );

            if (component->Is<ParticleSystem>( ))
            {
                m_systems.emplace_back(
                    static_cast<ParticleSystem*>( component )
                );
            }
        }

        void ParticleManagerSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            auto *component = const_cast<Component*>( args->component );

            if (component->Is<ParticleSystem>( ))
            {
                auto search = std::find(
                    m_systems.begin( ),
                    m_systems.end( ),
                    static_cast<ParticleSystem*>( component )
                );

                if (search != m_systems.end( ))
                    m_systems.erase( search );
            }
        }

        void ParticleManagerSystem::onUpdate(EVENT_HANDLER(World))
        {
            for (auto *system : m_systems)
                system->GetOwner( )->Dispatch( ENTITY_PARTICLE_UPDATE, EventArgs::Empty );
        }

    #if defined(URSINE_WITH_EDITOR)

        void ParticleManagerSystem::onEditorUpdate(EVENT_HANDLER(World))
        {
            for (auto *system : m_systems)
            {
                auto &owner = system->GetOwner( );

                if (system->UpdatesInEditor( ) || owner->HasComponent<Selected>( ))
                    owner->Dispatch( ENTITY_PARTICLE_UPDATE, EventArgs::Empty );
            }
        }

    #endif
    }
}