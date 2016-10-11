/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UAnimationReader.h
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
        // TODO: [J] Remove this shit
        namespace pipeline
        {
            class UAnimationImporter;
        }

        class UAnimationReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            UAnimationReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;

        } Meta(
            Enable,
            DisplayName( "UAnimationReader" )
        );
    }
}
