#include "UrsinePrecompiled.h"

#include "FBXSceneRootNodeComponent.h"

#include "GfxAPI.h"

#include "Model3DComponent.h"
#include "ConvexHullColliderComponent.h"
#include "BvhTriangleMeshColliderComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(FBXSceneRootNode);

        FBXSceneRootNode::FBXSceneRootNode(void)
            : BaseComponent( )
            , m_sceneName( "" )
			, m_notificationPresent( false )
        {
        }

        FBXSceneRootNode::~FBXSceneRootNode(void)
        {

        }

        void FBXSceneRootNode::OnInitialize(void)
        {
        }

        const std::string & FBXSceneRootNode::GetSceneName(void) const
        {
            return m_sceneName;
        }

        void FBXSceneRootNode::SetSceneName(const std::string & map)
        {
            m_sceneName = map;
        }

	#if defined(URSINE_WITH_EDITOR)

        void FBXSceneRootNode::importScene(void)
        {
            auto *gfx = GetCoreSystem( graphics::GfxAPI );

            auto *model = gfx->ResourceMgr.GetModelResource( m_sceneName );

            auto *world = GetOwner( )->GetWorld( );

            auto *transform = GetOwner( )->GetTransform( );

            if (model != nullptr)
            {
                auto &meshVec = model->GetMeshArray( );

                int childIndex = 0;

                for (auto &x : meshVec)
                {
                    // Create an entity
                    auto *newEntity = world->CreateEntity( x->GetName( ) );

                    // Add model3d
                    auto *modelComp = newEntity->AddComponent<Model3D>( );

                    // Set the mesh to this mesh
                    modelComp->SetModelResourceName( m_sceneName );

                    // Set its mesh index
                    modelComp->SetMeshIndex( childIndex++ );

                    // We need to grab the data from this... Well shit
                    transform->AddChild( newEntity->GetTransform( ) );
                }
            }
        }

		void FBXSceneRootNode::recursClearChildren(std::vector<Transform *> children)
        {
	        for (auto &child : children)
			{
				recursClearChildren( child->GetChildren( ) );

				child->GetOwner( )->Delete( );
			}
        }

        void FBXSceneRootNode::clearChildren(void)
        {
			recursClearChildren( GetOwner( )->GetTransform( )->GetChildren( ) );

			// Clear the deletion queue if the scene is paused
			EditorClearDeletionQueue( );
        }

		void FBXSceneRootNode::ImportScene(void)
        {
	        auto owner = GetOwner( );
			auto *children = owner->GetChildren( );

			if (children->size( ) > 0)
			{
				NotificationConfig config;

				config.type = NOTIFY_QUESTION;
				config.header = "Confirmation Of Destructive Action";
				config.message = "This action will delete all of the FBXSceneRootNode's children. Continue?";
				config.dismissible = false;
				config.duration = 0;

				NotificationButton yes, no;

				yes.text = "Confirm Action";
				yes.onClick = [=] (Notification &notification) {
					notification.Close( );
					m_notificationPresent = false;

					// Main thread operation
					Timer::Create( 0 ).Completed( [=] {
						clearChildren( );
						importScene( );
					} );
				};

				no.text = "Cancel Action";
				no.onClick = [=] (Notification &notification) {
					notification.Close( );
					m_notificationPresent = false;
				};

				config.buttons = { yes, no };

				EditorPostNotification( config );
				m_notificationPresent = true;
			}
			else
			{
				// Main thread operation
				Timer::Create( 0 ).Completed( [=] {
					importScene( );
				} );
			}
        }

		void FBXSceneRootNode::GenerateConvexHullForScene(void)
        {
	        auto models = GetOwner( )->GetComponentsInChildren<Model3D>( );

			for (auto &model : models)
			{
				auto entity = model->GetOwner( );

				if (!entity->HasComponent<ConvexHullCollider>( ))
					entity->AddComponent<ConvexHullCollider>( );

				auto convexHull = entity->GetComponent<ConvexHullCollider>( );

				convexHull->GenerateConvexHull( model );
			}
        }

		void FBXSceneRootNode::GenerateBvhTriangleMeshCollidersForScene(void)
        {
	        auto models = GetOwner( )->GetComponentsInChildren<Model3D>( );

			Timer::Create( 0 ).Completed( [=] {
				for (auto &model : models)
				{
					auto entity = model->GetOwner( );

					if (!entity->HasComponent<BvhTriangleMeshCollider>( ))
						entity->AddComponent<BvhTriangleMeshCollider>( );

					auto bvhTriangleMesh = entity->GetComponent<BvhTriangleMeshCollider>( );

					bvhTriangleMesh->GenerateBvhTriangleMesh( model );
				}
			} );
        }

	#endif
    }
}