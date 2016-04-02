/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SelectedComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#if defined(URSINE_WITH_EDITOR)

#include "Component.h"

namespace ursine
{
	namespace ecs
	{
		class World;

        class Selected : public Component
        {
            NATIVE_COMPONENT;

        public:
            Meta(Enable)
            Selected(void);
            ~Selected(void);

            void OnInitialize(void) override;

        private:
            void tryDebugModel(bool enabled);
            void tryDebugCamera(bool enabled);

            void onComponentAdded(EVENT_HANDLER(ursine::ecs::World));

        } Meta(
            Enable, 
            WhiteListMethods, 
            HiddenInInspector, 
            DisableSerialization,
            DisplayName( "Selected" )
        );
	}
}

#endif // defined(URSINE_WITH_EDITOR)
