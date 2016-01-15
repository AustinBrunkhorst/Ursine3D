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

        public:
            EditorField(
                std::string mapName,
                GetMapName,
                SetMapName
            );

            FBXSceneRootNode(void);
            ~FBXSceneRootNode(void);

            Meta(Disable)
            void OnInitialize(void);

            const std::string &GetMapName(void) const;
            void SetMapName(const std::string & map);

            void UpdateChildMap(void);
        private:
            std::string m_mapName;
            bool m_invalidated;

            
            void ClearChildMap(void);
        } Meta(Enable, DisplayName("FBXSceneRootNode"));
    }
}