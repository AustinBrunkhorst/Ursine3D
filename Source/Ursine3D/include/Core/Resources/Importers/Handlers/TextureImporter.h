#pragma once

#include "ResourceHandler.h"

#include "TextureImportOptions.h"

namespace ursine
{
    class TextureImporter : public ResourceHandler
    {
        RESOURCE_HANDLER;

        // Imports an asset from its original content
        ResourceData *Import(ResourceImportContext &context) override;

        // Writes an imported asset to the output file
        void WriteData(ResourceData *data, ResourceOutputContext &context) override;

        // Loads an imported asset from its output file
        ResourceData *LoadData(ResourceOutputContext &context) override;
    } Meta(
        Enable, 
        ResourceHandlerConfig( { "png", "jpg", "tga" }, typeof( ursine::TextureImportOptions ) )
    );
}