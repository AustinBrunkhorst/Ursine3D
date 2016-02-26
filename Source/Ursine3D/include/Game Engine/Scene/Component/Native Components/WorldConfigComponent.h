/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
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
        struct WorldEntitySystem
        {
            EditorMeta(ForceEditorType( "EntitySystemSelector" ))
            std::string type;

            WorldEntitySystem(void) { }
        } Meta(Enable, EnableArrayType, DisplayName( "WorldEntitySystem" ));

        typedef ursine::Array<ursine::ecs::WorldEntitySystem> WorldSystemArray;

		class WorldConfig : public Component
		{
			NATIVE_COMPONENT;

		public:
            EditorField(
                WorldSystemArray systems,
                GetSystems,
                SetSystems
            );

            Meta(Enable)
            WorldConfig(void);
			~WorldConfig(void);

            bool IsInEditorMode(void) const;
            void SetInEditorMode(bool inEditorMode);

            const WorldSystemArray &GetSystems(void) const;
            void SetSystems(const WorldSystemArray &systems);
		private:
            bool m_inEditorMode;

            WorldSystemArray m_systems;

            void OnInitialize(void) override;

		} Meta(
            Enable, 
            WhiteListMethods, 
            DisableComponentRemoval, 
            DisplayName( "WorldConfig" )
        );
	}
}