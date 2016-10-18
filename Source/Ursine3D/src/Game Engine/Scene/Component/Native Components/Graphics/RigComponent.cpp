/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RigComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RigComponent.h"

#include "RigData.h"
#include "Notification.h"
#include "BoneComponent.h"
#include "DebugSystem.h"
#include "SystemManager.h"
#include "EntityEvent.h"
#include "Model3DComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Rig );

        Rig::Rig(void)
            : BaseComponent( )
            , m_debug( false ) { }

        Rig::~Rig(void)
        {
            auto root = m_boneEntities.find( 0 );

            if (root != m_boneEntities.end( ))
            {
                if (root->second)
                    root->second->EnableDeletion( true );
            }
        }

        void Rig::OnInitialize(void)
        {
            GetOwner( )->GetWorld( )->Listener( this )
            #if defined(URSINE_WITH_EDITOR)
                .On( WORLD_EDITOR_UPDATE, &Rig::onUpdate )
            #endif
                .On( WORLD_UPDATE, &Rig::onUpdate );
        }

    #if defined(URSINE_WITH_EDITOR)

        bool Rig::GetDebugDraw(void)
        {
            return m_debug;
        }

        void Rig::SetDebugDraw(bool active)
        {
            m_debug = active;
        }

        void Rig::DebugDraw(void)
        {
            if (m_rig.GetGUID( ) == kNullGUID)
                return;

            auto rig = loadResource<resources::RigData>( m_rig );

            if (!rig)
                return;

            auto &bones = rig->bones;
            auto debugDrawer = GetOwner( )->GetWorld( )->GetEntitySystem<DebugSystem>( );

            for (auto &pair : m_boneEntities)
            {
                auto index = pair.first;
                auto entity = pair.second;
                auto &bone = bones[ index ];
                auto parent = bone.parent == -1 ? GetOwner( ) : m_boneEntities[ bone.parent ];

                auto entityTrans = entity->GetTransform( );
                auto parentTrans = parent->GetTransform( );

                auto parentPos = parentTrans->GetWorldPosition( );
                auto entityPos = entityTrans->GetWorldPosition( );

                debugDrawer->DrawCone(
                    parentPos, entityPos, SVec3::Length( parentPos - entityPos ), 
                    10.0f, Color::LawnGreen, 0.0f, true
                );
            }
        }

    #endif

        const resources::ResourceReference &Rig::GetRig(void) const
        {
            return m_rig;
        }

        void Rig::SetRig(const resources::ResourceReference &rig)
        {
            auto oldRig = m_rig;
            m_rig = rig;

            if (!resourcesAreAvailable( ))
                return;

            // ask the user if they want to load the rig.
            // if no, revert the rig back
            // TODO: Avoid having the user jumble the set "m_rig" and what entities are actualy created.
            // Have some dialog boxes or some way to do this more explicit

            NOTIFY_COMPONENT_CHANGED( "rig", m_rig );
        }

        EntityHandle Rig::GetBoneEntity(uint boneIndex) const
        {
            auto itr = m_boneEntities.find( boneIndex );

            if (itr == m_boneEntities.end( ))
                return nullptr;
            else
                return itr->second;
        }

        const ursine::ecs::Rig::BoneMap &Rig::GetBoneMap(void) const
        {
            return m_boneEntities;
        }

        const SMat4 &Rig::GetOffsetMatrix(uint boneIndex)
        {
            if (m_rig.GetGUID( ) == kNullGUID)
                return SMat4::Identity( );

            auto rig = loadResource<resources::RigData>( m_rig );

            // TODO: Have all offset matrices in their own array so we
            // don't pull the whole struct into cache
            return rig->bones[ boneIndex ].offset;
        }

    #if defined(URSINE_WITH_EDITOR)

        void Rig::ImportRig(void)
        {
            // If we currently have a generated rig root
            if (m_boneEntities.size( ) > 0)
            {
                auto rigRoot = m_boneEntities[ 0 ];

                NotificationConfig config;

                config.type = NOTIFY_WARNING;
                config.header = "Warning";
                config.message = "This action will delete all of the Rig_Root's children. Continue?";
                config.dismissible = false;
                config.duration = 0;

                NotificationButton yes, no;

                yes.text = "Yes";
                yes.onClick = [=](Notification &notification) {
                    notification.Close( );

                    // Main thread operation
                    Application::PostMainThread( [=] {
                        rigRoot->EnableDeletion( true );
                        rigRoot->Delete( );
                        importRig( );
                    } );
                };

                no.text = "No";
                no.onClick = [=](Notification &notification) {
                    notification.Close( );
                };

                config.buttons = { yes, no };

                EditorPostNotification( config );
                return;
            }

            Application::PostMainThread( [=] {
                importRig( );
            } );
        }

    #endif

        void Rig::importRig(void)
        {
            if (m_rig.GetGUID( ) == kNullGUID)
                return;

            m_boneEntities.clear( );

            auto parent = GetOwner( );
            auto world = parent->GetWorld( );
            auto rig = loadResource<resources::RigData>( m_rig );
            auto &bones = rig->bones;
            auto index = 0;

            for (auto &bone : bones)
            {
                // compinsate for an unset parent
                if (parent == nullptr)
                    parent = m_boneEntities[ bone.parent ];

                // create entity
                auto entity = world->CreateEntity( bone.name );

                entity->AddComponent<Bone>( );
                entity->EnableDeletion( false );
                entity->EnableHierarchyChange( false );

                // set entities local position/rotation/scale
                auto trans = entity->GetTransform( );

                trans->SetLocalPosition( bone.localPosition );
                trans->SetLocalScale( bone.localScale );
                trans->SetLocalRotation( bone.localRotation );

                // add it to it's parent
                parent->GetTransform( )->AddChildAlreadyInLocal( trans );

                // add it to the map
                m_boneEntities[ index ] = entity;

                // set the new parent
                if (bone.numChildren > 0)
                    parent = entity;
                else
                    parent = nullptr;

                ++index;
            }
        }

        void Rig::onUpdate(EVENT_HANDLER(World))
        {
            if (!resourcesAreAvailable( ))
                return;

            GetOwner( )->GetWorld( )->Listener( this )
            #if defined(URSINE_WITH_EDITOR)
                .Off( WORLD_EDITOR_UPDATE, &Rig::onUpdate )
            #endif
                .Off( WORLD_UPDATE, &Rig::onUpdate );

            if (m_rig.GetGUID( ) == kNullGUID)
                return;

            auto rig = loadResource<resources::RigData>( m_rig );

            if (!rig)
                return;

            auto bones = GetOwner( )->GetComponentsInChildren<Bone>( );

            // TODO: This doesn't work for a rig as a child of a rig
            // TODO: This doesn't work if the data gets missmatched
            for (auto &bone : bones)
            {
                auto entity = bone->GetOwner( );
                auto pair = rig->boneMap.find( entity->GetName( ) );

                if (pair == rig->boneMap.end( ))
                {
                    UWarning( 
                        "Failed to find the bone \"%s\" in the rig resource \"%s\".", 
                        entity->GetName( ).c_str( ), rig->name.c_str( )
                    );

                    return;
                }
                else
                    m_boneEntities[ pair->second ] = entity;
            }
        }
    }
}
