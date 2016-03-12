#include "UrsinePrecompiled.h"

#include "UIScreenData.h"
#include "UIScreenReader.h"

namespace ursine
{
    namespace resources
    {
        UIScreenData::UIScreenData(void) { }

        void UIScreenData::Write(pipeline::ResourceWriter &output)
        {
            
        }

        meta::Type UIScreenData::GetReaderType(void)
        {
            return typeof( UIScreenReader );
        }
    }
}