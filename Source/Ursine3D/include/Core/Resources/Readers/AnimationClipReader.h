/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationClipReader.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceTypeReader.h"
#include "AnimationInfo.h"

namespace ursine
{
    namespace resources
    {
        class AnimationClipReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            AnimationClipReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "AnimationClipReader" )
        );
    }
}