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

#include "AudioData.h"
#include "Array.h"

namespace ursine
{
    namespace ecs
    {
        class AudioConfig : public Component
        {
            NATIVE_COMPONENT;

        public:
            Meta(Enable)
            AudioConfig(void);

        } Meta(
            Enable, 
            WhiteListMethods, 
            DisplayName( "AudioConfig" )
        ) EditorMeta(
            DisableComponentRemoval
        );
    }
}