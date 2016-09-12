/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureImporter.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceImporter.h"

#include "TextureProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class TextureImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                TextureImporter(void);
                ~TextureImporter(void);

                static void ConvertToDDS(const fs::path &input, const fs::path &tempDirectory, BinaryData &output);

            private:
                unsigned m_importedWidth;
                unsigned m_importedHeight;

                ResourceData::Handle Import(ResourceImportContext &context) override;

                bool BuildPreview(const ResourceImportContext &context, const Vec2 &maxDimension) override;
            } Meta(
                Enable,
                DisplayName( "TextureImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { 
                        "bmp", "jpg", "jpeg", 
                        "png", "dds", "tga", 
                        "tif", "tiff", "wdp", "hdp"
                    },
                    typeof( ursine::rp::TextureProcessor )
                )
            );
        }
    }
}