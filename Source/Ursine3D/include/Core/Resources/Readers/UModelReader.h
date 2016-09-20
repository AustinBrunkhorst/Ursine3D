/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelReader.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class UModelReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            UModelReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "UModelReader" )
        );
    }
}
