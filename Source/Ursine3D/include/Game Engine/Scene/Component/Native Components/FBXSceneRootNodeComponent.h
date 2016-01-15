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
            bool m_invalidated;

			void updateChildren(void);
            void clearChildren(void);

        } Meta(Enable, DisplayName("FBXSceneRootNode"));
    }
}