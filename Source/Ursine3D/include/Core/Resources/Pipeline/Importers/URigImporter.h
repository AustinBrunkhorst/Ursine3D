/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** URigImporter.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceImporter.h"
#include "BuiltInResourceConfig.h"
#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            // TODO: [J] Remove this when we're no longer writting to the source directory
            class URigImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                URigImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;

            } Meta(
                Enable,
                DisplayName( "URigImporter" ),
                ResourceImporterConfig(
                    ExtensionList{ ursine::rp::kResourceTypeRigExtension },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}
