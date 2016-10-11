/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UAnimationImporter.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceImporter.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            // TODO: [J] Remove this when we're no longer writting to the source directory
            class UAnimationImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                UAnimationImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;

            } Meta(
                Enable,
                DisplayName( "UAnimationImporter" ),
                ResourceImporterConfig(
                    ExtensionList{ ursine::rp::kResourceTypeExtension },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}
