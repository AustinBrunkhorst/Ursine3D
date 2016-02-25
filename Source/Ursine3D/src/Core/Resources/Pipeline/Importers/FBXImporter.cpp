#include "UrsinePrecompiled.h"

#include "FBXImporter.h"
#include "ModelData.h"

#include "CFBXLoader.h"

namespace ursine
{
    rp::FBXImporter::FBXImporter(void) { }

    rp::FBXImporter::~FBXImporter(void) { }

    resources::ResourceData::Handle rp::FBXImporter::Import(ResourceImportContext &context)
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