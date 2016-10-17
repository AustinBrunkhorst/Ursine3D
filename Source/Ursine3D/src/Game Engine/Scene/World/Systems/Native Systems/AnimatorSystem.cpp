/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimatorSystem.h"

#include "AnimatorComponent.h"
#include "Model3DComponent.h"
#include "SelectedComponent.h"

#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION( AnimatorSystem );

        AnimatorSystem::AnimatorSystem(World *world)
            : EntitySystem( world ) { }

        void AnimatorSystem::OnInitialize(void)
        {
            m_world->Listener( this )
                .On( WORLD_UPDATE, &AnimatorSystem::onUpdate )

        #if defined(URSINE_WITH_EDITOR)

                .On( WORLD_EDITOR_UPDATE, &AnimatorSystem::onEditorUpdate )

        #endif

                .On( WORLD_ENTITY_COMPONENT_ADDED, &AnimatorSystem::onComponentAdded )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &AnimatorSystem::onComponentRemoved );
        }

        void AnimatorSystem::OnRemove(void)
        {
            m_world->Listener( this )
                .Off( WORLD_UPDATE, &AnimatorSystem::onUpdate )

        #if defined(URSINE_WITH_EDITOR)

                .Off( WORLD_EDITOR_UPDATE, &AnimatorSystem::onEditorUpdate )

        #endif

                .Off( WORLD_ENTITY_COMPONENT_ADDED, &AnimatorSystem::onComponentAdded )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &AnimatorSystem::onComponentRemoved );
        }

        void AnimatorSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            auto *component = const_cast<Component*>( args->component );

            if (component->Is<Animator>( ))
            {
                m_animators.emplace_back(
                    static_cast<Animator*>( component )
                );

                auto model = args->entity->GetComponent<Model3D>( );

                if (model)
                    model->clearMatrixPalette( );
            }
        }

        void AnimatorSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            auto *component = const_cast<Component*>( args->component );

            if (component->Is<Animator>( ))
            {
                auto search = std::find(
                    m_animators.begin( ),
                    m_animators.end( ),
                    static_cast<Animator*>( component )
                );

                if (search != m_animators.end( ))
                    m_animators.erase( search );

                auto model = args->entity->GetComponent<Model3D>( );

                if (model)
                    model->clearMatrixPalette( );
            }
        }

        void AnimatorSystem::onUpdate(EVENT_HANDLER(World))
        {
            auto dt = Application::Instance->GetDeltaTime( );

            for (auto &animator : m_animators)
            {
                auto rig = animator->GetOwner( )->GetComponent<Rig>( );

                updateAnimator( animator, rig, dt );
            }
        }

    #if defined(URSINE_WITH_EDITOR)

        void AnimatorSystem::onEditorUpdate(EVENT_HANDLER(World))
        {
            auto dt = Application::Instance->GetDeltaTime( );

            for (auto &animator : m_animators)
            {
                // update only if selected
                auto owner = animator->GetOwner( );
                auto rig = owner->GetComponent<Rig>( );

                if (animator->GetOwner( )->HasComponent<Selected>( ))
                    updateAnimator( animator, rig, dt );
            }
        }

    #endif

        void AnimatorSystem::updateAnimator(Animator *animator, Rig *rig, float dt)
        {
            if (!animator->m_playing ||animator->m_animation.GetGUID( ) == kNullGUID)
                return;

            auto animation = animator->m_animation.Load<resources::UAnimationData>(
                m_world->GetOwner( )->GetResourceManager( )
            );

            if (!animation)
              return;

            animator->m_time += animator->m_direction * dt;

            switch (animator->m_playmode)
            {
            case AnimationPlaymode::OneShot:
                animator->m_time = math::Clamp(
                    animator->m_time, 0.0f, animation->duration
                );
                break;
            case AnimationPlaymode::Loop:
                animator->m_time = math::Wrap(
                    animator->m_time, 0.0f, animation->duration
                );
                break;
            case AnimationPlaymode::PingPong:
                if (animator->m_time < 0.0f || animator->m_time > animation->duration)
                    animator->m_direction *= -1.0f;

                animator->m_time = math::Clamp(
                    animator->m_time, 0.0f, animation->duration
                );
                break;
            }

            animation->Evaluate( rig, animator->m_time );
        }
    }
}
