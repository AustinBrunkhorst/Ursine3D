/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioImporter.h
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
            class AudioImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                AudioImporter(void);
                ~AudioImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "AudioImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "uaudio" },
                    typeof( ursine::rp::PassThroughProcessor )
                ),
                ResourceSyncConfig(
                    FileExclusionList {
                        "testing"
                    }
                )
            );
        }
    }
}