/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#if defined(URSINE_WITH_EDITOR)

#include "Notification.h"

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

            EditorField(
                std::string sceneName,
				GetSceneName,
                SetSceneName
            );

            FBXSceneRootNode(void);
            ~FBXSceneRootNode(void);

            Meta(Disable)
            void OnInitialize(void);

            const std::string &GetSceneName(void) const;
            void SetSceneName(const std::string & map);

        private:
            std::string m_sceneName;

			
		#if defined(URSINE_WITH_EDITOR)

			static void recursClearChildren(std::vector<Transform *> children);
			void clearChildren(void);
			void importScene(void);

			Notification *m_notification;

			void clearNotification(void);

		#endif

        } Meta(Enable, DisplayName("FBXSceneRootNode"));
    }
}