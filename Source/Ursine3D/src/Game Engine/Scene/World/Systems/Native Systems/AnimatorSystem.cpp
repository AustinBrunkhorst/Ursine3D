/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "AnimationRig.h"

#include "EntityEvent.h"

namespace
{
    std::vector<ursine::SMat4> bonePalette( 100 );
    std::vector<ursine::SVec3> bonePoints( 100 );
}

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
                updateAnimator( animator, dt );
        }

    #if defined(URSINE_WITH_EDITOR)

        void AnimatorSystem::onEditorUpdate(EVENT_HANDLER(World))
        {
            auto dt = Application::Instance->GetDeltaTime( );

            for (auto &animator : m_animators)
            {
                // update only if selected
                if (animator->GetOwner( )->HasComponent<Selected>( ))
                    updateAnimator( animator, dt );
            }
        }

    #endif

        void AnimatorSystem::updateAnimator(Animator *animator, float dt)
        {
            if (!animator->m_playing)
                return;

            // Grab the current and future state
            AnimationState *currentState = nullptr;
            AnimationState *futureState = nullptr;

            for (auto &x : animator->stArray)
            {
                if (x.GetStateName( ) == "")
                    continue;
                if (x.GetStateName( ) == animator->m_curStName)
                    currentState = &x;
                if ( (x.GetStateName( ) == animator->m_futStName) 
                    && (animator->m_curStName != animator->m_futStName) )
                    futureState = &x;
            }

            // If the current state doesn't exist and the future state does, just set the future state to current
            if (!currentState && futureState)
            {
                currentState = futureState;
                futureState = nullptr;
                animator->m_curStName = animator->m_futStName;
                animator->m_futStName = "";
            }

            // If we still don't have a current state, nope out
            if (!currentState)
                return;

            // If the clip we're trying to play hasn't been loaded, load it
            if (!currentState->m_loaded)
                animator->loadStateAnimation( currentState );

            if (futureState && !futureState->m_loaded)
                animator->loadStateAnimation( futureState );

            // Get the animation pointer from the states
            const Animation *currentAnimation = nullptr == currentState ? nullptr : currentState->GetAnimation( );
            const Animation *futureAnimation = nullptr == futureState ? nullptr : futureState->GetAnimation( );

            ///// FOR INSTATE ANIMATION
            //const std::vector<Animation*> currAnimVec = currentState->GetAnimationVector();
            //const std::vector<Animation*> futAnimVec = futureState->GetAnimationVector();

            // Get the rig we're using
            auto *rig = AnimationBuilder::GetAnimationRigByName( animator->m_rig );

            // If anything we need is null, opt out.
            if (nullptr == currentAnimation || nullptr == rig)
                return;
            
            // Set the rig's transform pointers 
            // (this is needed because a rig is instanced, and the transform pointers need to be set)
            animator->setRigTransformPointers( );

            // If the bone count of the animation doesn't match the rig's bone count, nope out
            if (currentAnimation->GetDesiredBoneCount( ) != rig->GetBoneCount( ))
                return;
            ///// FOR INSTATE ANIMATION
            ///// added
            //if (nullptr == rig)
            //    return;
            //
            ///// added
            //for (auto &iter : currAnimVec)
            //{
            //    if (nullptr == iter)
            //        return;
            //    
            //    if(iter->GetDesiredBoneCount( ) != rig->GetBoneCount( ))
            //        return;
            //}

            // If we have a future state
            if (nullptr != futureAnimation)
            {
                // Check to see if that clip's bone count matches the rig too
                if (futureAnimation->GetDesiredBoneCount( ) != rig->GetBoneCount( ))
                    return;
            }
            
            ///// FOR INSTATE ANIMATION
            ///// added
            //for (auto &iter : futAnimVec)
            //{
            //    if ( ( nullptr == iter ) 
            //        || ( iter->GetDesiredBoneCount( ) != rig->GetBoneCount( ) ) )
            //    {
            //        animator->m_transFactor = 0.0f;
            //        return;
            //    }
            //}

            // Get the model's matrix palette
            auto &matrixPalette = animator->GetOwner( )->GetComponent<Model3D>( )->getMatrixPalette( );
            auto &matrixPaletteIT = animator->GetOwner( )->GetComponent<Model3D>( )->getMatrixPaletteIT( );

            // blending / playing animation should take place in here
            animator->updateState( 
                &currentState, 
                &currentAnimation, 
                &futureState, 
                &futureAnimation, 
                //currAnimVec,
                //futAnimVec,
                dt, 
                animator->m_transFactor
            );

            // dispatch the events
            animator->sendAvailableEvents( 
                currentState->GetStateName( ), currentState->GetRatio( )
            );

            if (futureState)
                animator->sendAvailableEvents( 
                    futureState->GetStateName( ), futureState->GetRatio( )
                );

            // generate the matrices
            AnimationBuilder::GenerateAnimationData(
                currentState,
                futureState,
                rig,
                matrixPalette,
                bonePalette,
                animator->m_transFactor
            );

            if (animator->m_enableBoneManipulation)
            {
                animator->GetOwner( )->Dispatch( ENTITY_ANIMATION_BONE_MANIPULATION_VALID, nullptr );

                // Set the matrix pallet from the rig
                auto boneCount = rig->GetBoneCount( );
                auto &offsetMatrices = rig->GetOffsetMatrices( );
                auto parentTrans = animator->GetOwner( )->GetTransform( )->GetWorldToLocalMatrix( );

                for (uint i = 0; i < boneCount; ++i)
                {
                    auto bone = rig->GetBone( i );

                    matrixPalette[ i ] = 
                        parentTrans * 
                        bone->m_transform->GetLocalToWorldMatrix( ) *
                        offsetMatrices[ i ];

                    SMat4 boneIT = matrixPalette[i];
                    boneIT.Inverse();
                    boneIT.Transpose();
                    matrixPaletteIT[i] = boneIT;
                }
            }

            ////////////////////////////////////////////////////////////////
            // TEMPORARY DEBUG STUFF
            // render the debug data
            if (animator->m_debug)
                debugDraw( animator, rig, bonePalette );
        }

        void AnimatorSystem::debugDraw(Animator *animator, const AnimationRig *rig, const std::vector<SMat4> &vec) const
        {
            auto *graphics = GetCoreSystem(graphics::GfxAPI);

            URSINE_TODO("Remove this when we implement proper animation clips and stuf")
            
            auto &hierarchy = rig->GetHierarchyTable( );
            auto &worldTransform = animator->GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );
            
            int maxNodeDistance = 0;
            
            size_t boneCount = hierarchy.size( );
            
            // calculate max distance for colors, calculate bone position
            for (size_t x = 0; x < boneCount; ++x)
            {
                // distance
                int distance = 0;
                int walker = hierarchy[x];
            
                while (walker != -1)
                {
                    walker = hierarchy[walker];
                    distance++;
                }
            
                if (distance > maxNodeDistance) maxNodeDistance = distance;
            
                // bone values
                bonePoints[x] = worldTransform.TransformPoint(vec[x].TransformPoint(SVec3(0, 0, 0)));
            }
            
            // render points
            graphics->DrawingMgr.SetOverdraw(true);
            
            for (unsigned x = 0; x < boneCount; ++x)
            {
                int distance = 0;
                int walker = hierarchy[x];
            
                while (walker != -1)
                {
                    walker = hierarchy[walker];
                    distance++;
                }
            
                float interp = (float)distance / (float)maxNodeDistance;
                //draw points
            
                graphics->DrawingMgr.SetColor(Color(1 * interp, (1.f - interp), 0, 1));
            
                if (distance == 0)
                    graphics->DrawingMgr.SetColor(0, 0, 1, 1);
            
                graphics->DrawingMgr.SetSize(10);
                SVec3 &p = bonePoints[x];
                graphics->DrawingMgr.DrawPoint(p);
            }
            
            // render lines
            for (size_t x = boneCount - 1; x >= 1; --x)
            {
                SVec3 &p1 = bonePoints[x];
                SVec3 &p2 = bonePoints[hierarchy[x]];
            
                graphics->DrawingMgr.DrawLine(p1, p2);
            }
            
            graphics->DrawingMgr.SetOverdraw(false);
        }
    }
}