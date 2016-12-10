/* ----------------------------------------------------------------------------
** Team Bear King
** � 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelImporter.h
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
            // TODO: [J] Remove this when we're no longer writing to the source directory
            class ModelImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                ModelImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;

            } Meta(
                Enable,
                DisplayName( "ModelImporter" ),
                ResourceImporterConfig(
                    ExtensionList{ ursine::rp::kResourceTypeModelExtension },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}