/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FBXFileImporter.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceImporter.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class FBXFileImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                FBXFileImporter(void);
                ~FBXFileImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "FBXFileImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "fbx" },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}