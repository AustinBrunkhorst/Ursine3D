#include "UrsinePrecompiled.h"

#include "FBXImporter.h"
#include "ModelData.h"

namespace ursine
{
    rp::FBXImporter::FBXImporter(void) { }

    rp::FBXImporter::~FBXImporter(void) { }

    resources::ResourceData::Handle rp::FBXImporter::Import(const ResourceImportContext &context)
    {
		// .../BossRoom.fbx
		auto fileName = context.resource->GetSourceFileName( );

		// read the file
		// construct our model data
		// output the animation clipdata resources

        return std::make_shared<ModelData>( );
    }
}