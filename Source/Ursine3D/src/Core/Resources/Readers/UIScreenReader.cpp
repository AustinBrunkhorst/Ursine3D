#include "UrsinePrecompiled.h"

#include "UIScreenReader.h"
#include "UIScreenData.h"

namespace ursine
{
    namespace resources
    {
        UIScreenReader::UIScreenReader(void) { }

        ResourceData::Handle UIScreenReader::Read(ResourceReader &input)
        {
            return std::make_shared<UIScreenData>( );
        }
    }
}