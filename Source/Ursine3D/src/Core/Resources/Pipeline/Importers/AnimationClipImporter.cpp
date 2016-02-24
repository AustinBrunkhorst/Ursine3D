#include "UrsinePrecompiled.h"

#include "AnimationClipImporter.h"
#include "AnimationClipData.h"

namespace ursine
{
    rp::AnimationClipImporter::AnimationClipImporter(void) { }

    rp::AnimationClipImporter::~AnimationClipImporter(void) { }

    resources::ResourceData::Handle rp::AnimationClipImporter::Import(const ResourceImportContext &context)
    {
        return std::make_shared<AnimationClipData>( );
    }
}