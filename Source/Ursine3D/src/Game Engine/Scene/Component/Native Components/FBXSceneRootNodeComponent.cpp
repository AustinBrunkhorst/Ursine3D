#include "UrsinePrecompiled.h"

#include "FBXSceneRootNodeComponent.h"

#include "GfxAPI.h"

#include "Model3DComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(FBXSceneRootNode);

        FBXSceneRootNode::FBXSceneRootNode(void)
            : BaseComponent( )
            , m_sceneName( "" )
            , m_invalidated( false )
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
            m_invalidated = true;
        }

        void FBXSceneRootNode::updateChildren(void)
        {
            if (!m_invalidated)
                return;

            m_invalidated = false;
            auto *gfx = GetCoreSystem( graphics::GfxAPI );

            auto *model = gfx->ResourceMgr.GetModel( m_sceneName );

            auto *world = GetOwner( )->GetWorld( );

            auto *transform = GetOwner( )->GetTransform( );

            if (model != nullptr)
            {
                clearChildren( );

                auto &meshVec = model->GetMeshArray( );

                int childIndex = 0;

                for (auto &x : meshVec)
                {
                    // Create an entity
                    auto *newEntity = world->CreateEntity( x->GetName( ) );

                    // Add model3d
                    auto *modelComp = newEntity->AddComponent<Model3D>( );

                    // Set the mesh to this mesh
                    modelComp->SetModel( m_sceneName );

                    // Set its mesh index
                    modelComp->SetMeshIndex( childIndex++ );

                    // We need to grab the data from this... Well shit
                    transform->AddChild( newEntity->GetTransform( ) );
                }
            }
        }

        void FBXSceneRootNode::clearChildren(void)
        {
			auto *owner = GetOwner( );
			auto *world = owner->GetWorld( );
            auto *childrenVector = owner->GetChildren( );

            for (auto &x : *childrenVector)
            {
				world->GetEntity( x )->Delete( );
            }
        }
    }
}