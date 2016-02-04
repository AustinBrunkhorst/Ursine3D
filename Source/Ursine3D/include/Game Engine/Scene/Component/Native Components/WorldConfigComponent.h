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
            Meta(ForceEditorType( "EntitySystemSelector" ))
            std::string type;

            WorldEntitySystem(void) { }
        } Meta(Enable, DisplayName( "WorldEntitySystem" ));

		class WorldConfig : public Component
		{
			NATIVE_COMPONENT;

		public:
            ursine::Array<ursine::ecs::WorldEntitySystem> systems;

            Meta(Enable)
            WorldConfig(void);
			~WorldConfig(void);

            bool IsInEditorMode(void) const;
            void SetInEditorMode(bool inEditorMode);
		private:
            bool m_inEditorMode;

            void OnInitialize(void) override;

		} Meta(
            Enable, 
            WhiteListMethods, 
            DisableComponentRemoval, 
            DisplayName( "WorldConfig" )
        );
	}
}