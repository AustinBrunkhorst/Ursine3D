/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceFormatReader.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceReader.h"
#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class ResourceFormatReader
        {
        public:
            ResourceFormatReader(void);

            ResourceData::Handle Read(const fs::path &resourceFile);

        private:
            ResourceData::Handle read(ResourceReader &reader);
        };
    }
}