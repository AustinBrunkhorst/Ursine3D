#include "UrsinePrecompiled.h"

#include "FBXFileImporter.h"
#include "ModelData.h"

#include "CFBXLoader.h"

namespace ursine
{
    rp::FBXFileImporter::FBXFileImporter(void) { }

    rp::FBXFileImporter::~FBXFileImporter(void) { }

    resources::ResourceData::Handle rp::FBXFileImporter::Import(const ResourceImportContext &context)
    {
        // .../BossRoom.fbx
        auto fileName = context.resource->GetSourceFileName( );

        graphics::CFBXLoader importer;

        UAssert( importer.LoadFBX( fileName.string( ) ),
            "Unable to import FBX file.\nfile: %s",
            fileName.string( ).c_str( )
        );

        // output the animation clipdata resources

        return std::make_shared<ModelData>( importer.GetModelInfo( ) );
    }
}