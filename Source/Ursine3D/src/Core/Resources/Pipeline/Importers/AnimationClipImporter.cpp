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
        // how can I import .uanim file?        
        fs::path fName(context.resource->GetSourceFileName());

        auto clipFileName = fName.filename().string();
                
        ResourceReader clipReader(clipFileName);

        // create animation info
        graphics::ufmt_loader::AnimInfo *animInfo = new graphics::ufmt_loader::AnimInfo();

        // and read data from Resource Reader
        animInfo->Read( clipReader );

        return std::make_shared<AnimationClipData>( animInfo );
    }
}