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
            : BaseComponent()
            , m_mapName("")
            , m_invalidated(false)
        {
        }

        FBXSceneRootNode::~FBXSceneRootNode(void)
        {

        }

        void FBXSceneRootNode::OnInitialize(void)
        {
        }

        const std::string & FBXSceneRootNode::GetMapName(void) const
        {
            return m_mapName;
        }

        void FBXSceneRootNode::SetMapName(const std::string & map)
        {
            m_mapName = map;
            m_invalidated = true;
        }

        void FBXSceneRootNode::UpdateChildMap(void)
        {
            if ( !m_invalidated )
                return;

            m_invalidated = false;
            auto *gfx = GetCoreSystem(graphics::GfxAPI);

            auto *model = gfx->ResourceMgr.GetModelResource(m_mapName);

            auto *world = GetOwner()->GetWorld();

            auto *transform = GetOwner()->GetTransform();

            if ( model != nullptr )
            {
                ClearChildMap();

                auto &meshVec = model->GetMeshArray();

                int childIndex = 0;

                for ( auto &x : meshVec )
                {
                    // Create an entity
                    auto *newEntity = world->CreateEntity(x->GetName());

                    // Add model3d
                    auto *modelComp = newEntity->AddComponent<Model3D>();

                    // Set the mesh to this mesh
                    modelComp->SetModelResourceName(m_mapName);

                    // Set its mesh index
                    modelComp->SetMeshIndex(childIndex++);

                    // We need to grab the data from this... Well shit
                    transform->AddChild(newEntity->GetTransform());
                }
            }
        }

        void FBXSceneRootNode::ClearChildMap(void)
        {
            auto *childrenVector = GetOwner()->GetChildren();

            for ( auto &x : *childrenVector )
            {
                GetOwner()->GetWorld()->GetEntity(x)->Delete();
            }
        }
    }
}