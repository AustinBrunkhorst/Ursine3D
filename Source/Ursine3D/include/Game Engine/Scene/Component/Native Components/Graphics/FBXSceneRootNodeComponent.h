/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FBXSceneRootNodeComponentee.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include <string>
#include "UModelData.h"

#if defined(URSINE_WITH_EDITOR)

#include "EditorConfig.h"
 
#endif

namespace ursine
{
    namespace ecs
    {
        class FBXSceneRootNode
            : public Component
        {
            NATIVE_COMPONENT;

            friend class RenderSystem;

        public:

            EditorButton(
                ImportScene,
                "Import Scene"
            );

            EditorButton(
                GenerateConvexHullForScene,
                "Generate Convex Hull Colliders For Scene"
            );

            EditorButton(
                GenerateBvhTriangleMeshCollidersForScene,
                "Generate BVH Triangle Mesh Colliders For Scene"
            );

            EditorResourceField(
                ursine::resources::UModelData,
                sceneModel,
                GetModel,
                SetModel
            );

            FBXSceneRootNode(void);
            ~FBXSceneRootNode(void);

            Meta(Disable)
            void OnInitialize(void);

            const ursine::resources::ResourceReference &GetModel(void) const;
            void SetModel(const ursine::resources::ResourceReference &model);

        private:
            ursine::resources::ResourceReference m_modelResource;
            graphics::GfxHND m_modelHandle;
            
        #if defined(URSINE_WITH_EDITOR)

            static void recursClearChildren(const std::vector< Handle<Transform> > &children);
            void clearChildren(void);
            void importScene(void);
            void invalidateModel(bool unload = true);

            bool m_notificationPresent;

        #endif

        } Meta(Enable, DisplayName( "FBXSceneRootNode" ));
    }
}