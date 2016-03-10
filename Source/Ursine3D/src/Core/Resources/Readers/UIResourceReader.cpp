#include "UrsinePrecompiled.h"

#include "UIResourceReader.h"
#include "UIResourceData.h"

namespace ursine
{
    namespace resources
    {
        UIResourceReader::UIResourceReader(void) { }

        ResourceData::Handle UIResourceReader::Read(ResourceReader &input)
        {
            return std::make_shared<UIResourceData>( );
        }
    }
}