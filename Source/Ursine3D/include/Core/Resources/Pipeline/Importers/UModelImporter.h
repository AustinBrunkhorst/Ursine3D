/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelImporter.h
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

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            // TODO: [J] Remove this when we're no longer writing to the source directory
            class UModelImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

                ResourceData::Handle Import(ResourceImportContext &context) override;

            } Meta(
                Enable,
                DisplayName( "UModelImporter" ),
                ResourceImportConfig(
                    ExtensionList{ kResourceTypeModelExtension },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}
