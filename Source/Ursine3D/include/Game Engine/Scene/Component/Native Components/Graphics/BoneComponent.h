/* ----------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BoneComponent.h
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
        class Bone : public Component
        {
            NATIVE_COMPONENT;

        public:
            Bone(void);

        } Meta(
            Enable, 
            DisplayName( "Bone" ),
            HiddenInSelector,
            DisableComponentRemoval
        );
    }
}
