/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RigComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        class Rig : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorAnnotate( "Import the rig from the current model." )
            EditorButton(
                ImportRig,
                "Import Rig"
            );

            EditorResourceField(
                ursine::resources::URigData,
                rig,
                GetRig,
                SetRig
            );

            Rig(void);
            ~Rig(void);

            const resources::ResourceReference &GetRig(void) const;
            void SetRig(const resources::ResourceReference &rig);

        private:
            resources::ResourceReference m_rig;

            std::unordered_map<uint, EntityHandle> m_boneEntities;

            void importRig(void);

        } Meta(
            Enable, 
            DisplayName( "Rig" )
        ) EditorMeta( 
            HiddenInSelector,
            DisableComponentRemoval
        );
    }
}
