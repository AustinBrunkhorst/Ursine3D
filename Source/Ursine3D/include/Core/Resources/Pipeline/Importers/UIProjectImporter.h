/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIProjectImporter.h
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
            class UIProjectImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                UIProjectImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "UIProjectImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "uiproj" },
                    typeof( ursine::rp::PassThroughProcessor )
                ),
                ResourceSyncConfig(
                    FileExclusionList {
                        // whitelist non build/preprocess files
                        SYNC_EXPR_EXCLUDE_NOT( "ui|js|html|css|png|svg|svgz|ttf|woff2|ogg" )
                    }
                )
            );
        }
    }
}