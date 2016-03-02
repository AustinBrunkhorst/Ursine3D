#include "UrsinePrecompiled.h"

#include "AnimationClipImporter.h"
#include "AnimationClipData.h"

#include "AnimationInfo.h"
#include "CFBXLoader.h"

namespace ursine
{
    rp::AnimationClipImporter::AnimationClipImporter(void) { }

    rp::AnimationClipImporter::~AnimationClipImporter(void) { }

    resources::ResourceData::Handle rp::AnimationClipImporter::Import(ResourceImportContext &context)
    { 
        auto fileName = context.resource->GetSourceFileName( );
                
        ResourceReader clipReader( fileName.string( ) );

        UAssertCatchable( clipReader.IsOpen( ),
            "Unable to open animation clip."
        );

        // create animation info
        auto *animInfo = new graphics::ufmt_loader::AnimInfo( );

        // and read data from Resource Reader
        animInfo->Read( clipReader );

        return std::make_shared<AnimationClipData>( animInfo );
    }
}