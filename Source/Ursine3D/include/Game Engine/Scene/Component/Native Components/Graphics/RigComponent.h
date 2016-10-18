/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RigComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "RigData.h"

namespace ursine
{
    namespace ecs
    {
        class Rig : public Component
        {
            NATIVE_COMPONENT;

            friend class RigSystem;

        public:
            typedef std::unordered_map<uint, EntityHandle> BoneMap;

            EditorAnnotate( "Import the rig from the current model." )
            EditorButton(
                ImportRig,
                "Import Rig"
            );

            EditorOnlyField(
                bool debugDraw,
                GetDebugDraw,
                SetDebugDraw
            );

            EditorResourceField(
                ursine::resources::RigData,
                rig,
                GetRig,
                SetRig
            );

            Rig(void);
            ~Rig(void);

            void OnInitialize(void) override;

        #if defined(URSINE_WITH_EDITOR)

            bool GetDebugDraw(void);
            void SetDebugDraw(bool active);

            void DebugDraw(void);

        #endif

            const ursine::resources::ResourceReference &GetRig(void) const;
            void SetRig(const ursine::resources::ResourceReference &rig);

            ursine::ecs::EntityHandle GetBoneEntity(uint boneIndex) const;

            const ursine::ecs::Rig::BoneMap &GetBoneMap(void) const;

            const ursine::SMat4 &GetOffsetMatrix(uint boneIndex);

        private:
            resources::ResourceReference m_rig;

            BoneMap m_boneEntities;

            bool m_debug;

            void importRig(void);

            void onUpdate(EVENT_HANDLER(World));

        } Meta(
            Enable, 
            DisplayName( "Rig" )
        );
    }
}
