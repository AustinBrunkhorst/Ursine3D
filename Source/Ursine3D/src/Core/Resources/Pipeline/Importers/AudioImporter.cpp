#include "UrsinePrecompiled.h"

#include "AudioImporter.h"
#include "AudioData.h"

namespace ursine
{
    rp::AudioImporter::AudioImporter(void) { }

    rp::AudioImporter::~AudioImporter(void) { }

    resources::ResourceData::Handle rp::AudioImporter::Import(ResourceImportContext &context)
    {
        auto &fileName = context.resource->GetSourceFileName( );

        std::vector<fs::path> generated { "a.json", "b.json", "c.json", "d.json", "e.json" };

        auto directory = fileName.parent_path( );

        for (auto &name : generated)
        {
            auto path = directory / name;

            fs::WriteAllText( path.string( ), "{}" );

            context.AddGeneratedResource( path );
        }

        return std::make_shared<AudioData>( );
    }
}