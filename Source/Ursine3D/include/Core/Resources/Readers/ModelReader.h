/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelReader.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceTypeReader.h"
#include "ModelInfo.h"

namespace ursine
{
    namespace resources
    {
        class ModelReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            ModelReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "ModelReader" )
        );
    }
}