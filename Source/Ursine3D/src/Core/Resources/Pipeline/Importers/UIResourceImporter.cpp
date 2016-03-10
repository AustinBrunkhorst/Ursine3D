#include "UrsinePrecompiled.h"

#include "UIResourceImporter.h"
#include "UIResourceData.h"

namespace ursine
{
    rp::UIResourceImporter::UIResourceImporter(void) { }

    resources::ResourceData::Handle rp::UIResourceImporter::Import(ResourceImportContext &context)
    {
        return std::make_shared<UIResourceData>( );
    }
}