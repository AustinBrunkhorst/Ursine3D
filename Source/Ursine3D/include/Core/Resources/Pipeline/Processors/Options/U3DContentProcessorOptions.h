/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** U3DContentProcessorOptions.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceProcessorOptions.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class U3DContentProcessorOptions : public ResourceProcessorOptions
            {
                RESOURCE_PROCESSOR_OPTIONS;

            public:
                U3DContentProcessorOptions(void);

                bool processRig;
                bool processModel;
                bool processAnimations;

            } Meta(Enable);
        }
    }
}